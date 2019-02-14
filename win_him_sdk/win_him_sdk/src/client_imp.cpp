#include "stdafx.h"
#include "client_imp.h"
#include <list>

#include "api/ihttp_client.h"
#include "api/log.h"

#include "json/json.h"

// 协议
#include "protocol/IM.BaseDefine.pb.h"
#include "protocol/IM.Other.pb.h"
#include "network/PBHeader.h"
#include "network/UtilPdu.h"

// aes加密
#include "security.h"

#define MAX_RECV_BUFFER_LEN 10240	//Tcp数据读取缓冲区

namespace him {
	std::shared_ptr<IClient> NewClientModule()
	{
		std::shared_ptr<ClientImp> instance = std::make_shared<ClientImp>();

		g_client_list_mutex_.lock();
		g_client_list_.push_back(instance);
		g_client_list_mutex_.unlock();

		return std::dynamic_pointer_cast<IClient>(instance);
	}

	ClientImp::ClientImp()
		: server_port_(0)
		, callback_(nullptr)
		, client_state_(ClientState::kClientDisconnect)
		, receive_thread_run_(true)
		, seq_(0)
		, heartbeat_time_(0)
		, send_msg_count_(0)
		, send_msg_success_count_(0)
		, server_time_(0)
		, server_time_offset_(0)
	{
		tcp_client_ = std::make_shared<boost::asio::ip::tcp::socket>(io_server_);
	}
	ClientImp::~ClientImp()
	{
		receive_thread_run_ = false;
		if (receive_thread_ != nullptr) {
			if (receive_thread_->joinable())
				receive_thread_->join();
			receive_thread_ = nullptr;
		}
	}

	void ClientImp::Login(std::string user_name, std::string pwd, std::string server_ip, unsigned short port, const LoginResultCallback &callback)
	{
		callback_login_res_ = callback;
		client_state_ = ClientState::kClientDisconnect;

		char urlBuf[128] = { 0 };
		sprintf(urlBuf, "http://%s:%d/msg_server", server_ip.c_str(), port);

		std::shared_ptr<IHttpClient> client = him::GetHttpClientModule();
		std::string response;
		logd("查询可用服务器列表中...");
		// 阻塞，默认3秒超时
		UrlCode code = client->HttpGetReq(std::string(urlBuf), response);
		if (code == UrlCode::URLE_OK) {
			Json::Value json_value;
			Json::Reader json_reader;
			if (!json_reader.parse(response, json_value)) {
				loge("查询服务器成功，但是解析返回json结果失败！无法继续登录");
				_OnLogin(-1, "server internal error");
				return;
			}

			int json_code = json_value["code"].asInt();
			std::string json_msg = json_value["msg"].asString();
			if (json_code != 0) {
				loge("查询服务器成功，但服务器内部发生错误：%s", json_msg.c_str());
				_OnLogin(-1, json_msg);
				return;
			}

			std::string msg_server_ip = json_value["priorIP"].asString();
			std::string msg_server_port = json_value["port"].asString();
			if (msg_server_ip.empty() || msg_server_port.empty()) {
				loge("查询服务器成功，但是解析返回json结果失败！无法继续登录");
				_OnLogin(-1, "server internal error");
				return;
			}

			logd("查询成功，开始连接消息服务器：%s:%s", msg_server_ip.c_str(), msg_server_port.c_str());

			boost::system::error_code code;
			boost::asio::ip::tcp::endpoint e(boost::asio::ip::address_v4::from_string(msg_server_ip), std::stoi(msg_server_port));
			tcp_client_->connect(e, code);

			if (code) {
				boost::system::system_error err(code);
				loge("connect error:%s", err.what());
				_OnLogin(-1, "无法连接目标服务器");
				return;
			}
			client_state_ = ClientState::kClientConnectedOk;

			// 启动接收线程
			receive_thread_ = std::make_shared<std::thread>(&ClientImp::ReceiveThreadProc, this);

			logd("connect success");
			// 发起认证
			IM::Login::IMLoginReq req;
			req.set_user_name(user_name.c_str());
			req.set_password(pwd.c_str());
			req.set_online_status(IM::BaseDefine::USER_STATUS_ONLINE);
			req.set_client_type(IM::BaseDefine::CLIENT_TYPE_WINDOWS);
			req.set_client_version("v1");

			Send(IM::BaseDefine::SID_LOGIN, IM::BaseDefine::CID_LOGIN_REQ_USERLOGIN, &req);
		}
		else {
			loge("http connect error: %d", code);
		}
	}
	ClientState ClientImp::GetClientState()
	{
		return client_state_;
	}
	void ClientImp::LoginOut()
	{
		// 发送注销请求
		IM::Login::IMLogoutReq req;
		Send(IM::BaseDefine::SID_LOGIN, IM::BaseDefine::CID_LOGIN_REQ_LOGINOUT, &req);

		// 收到响应后注销
		/*tcp_client_->close();
		receive_thread_run_ = false;
		if (receive_thread_ != nullptr && receive_thread_->joinable()) {
			receive_thread_->join();
			receive_thread_ = nullptr;
		}*/
	}

	int ClientImp::Send(int server_id, int msg_id, google::protobuf::MessageLite* pbBody)
	{
		return Send(server_id, msg_id, ThreadSafeGetSeq(), pbBody);
	}
	int ClientImp::Send(int server_id, int msg_id, unsigned int seq, google::protobuf::MessageLite* pbBody)
	{
		// 组装协议头
		unsigned int len = pbBody->ByteSize() + HEADER_LENGTH;
		PBHeader header;
		header.SetModuleId(server_id);
		header.SetCommandId(msg_id);
		header.SetSeqNumber(seq);
		header.SetLength(len);

		std::unique_ptr<unsigned char> buf(new unsigned char[len]);
		::memset(buf.get(), 0, len);
		::memcpy(buf.get(), header.GetSerializeBuffer(), HEADER_LENGTH);
		pbBody->SerializeToArray(buf.get() + HEADER_LENGTH, pbBody->ByteSize());

		if (tcp_client_->is_open()) {
			size_t send_len = tcp_client_->write_some(boost::asio::buffer(buf.get(), len));
			if (send_len < (unsigned int)len) {
				loge("tcp send error,source len=%d B,send len=%d", len, send_len);
			}
			return send_len;
		}

		return -1;
	}


	int ClientImp::Send(int server_id, int msg_id, const unsigned char* data, int len)
	{
		// 组装协议头
		len = len + HEADER_LENGTH;

		PBHeader header;
		header.SetModuleId(server_id);
		header.SetCommandId(msg_id);
		header.SetSeqNumber(ThreadSafeGetSeq());
		header.SetLength(len);

		// 包含数据部
		std::unique_ptr<unsigned char> buf(new unsigned char[len]);
		::memset(buf.get(), 0, len);
		::memcpy(buf.get(), header.GetSerializeBuffer(), HEADER_LENGTH);
		::memcpy(buf.get() + HEADER_LENGTH, data, len - HEADER_LENGTH);

		if (tcp_client_->is_open()) {
			size_t send_len = tcp_client_->write_some(boost::asio::buffer(buf.get(), len));
			if (send_len < (unsigned int)len) {
				loge("tcp send error,source len=%d B,send len=%d", len, send_len);
			}
			return send_len;
		}
		return -1;
	}
	/*
	int ClientImp::Send(int server_id, int msg_id, unsigned int seq, const unsigned char* data, int len)
	{
		// 组装协议头
		PBHeader header;
		header.SetModuleId(server_id);
		header.SetCommandId(msg_id);
		header.SetSeqNumber(seq);
		header.SetLength(len + HEADER_LENGTH);

		// 包含数据部
		::memcpy(write_buffer_, header.GetSerializeBuffer(), HEADER_LENGTH);
		::memcpy(write_buffer_ + HEADER_LENGTH, data, len);

		if (tcp_client_->is_open()) {
			size_t send_len = tcp_client_->write_some(boost::asio::buffer(write_buffer_, len + HEADER_LENGTH));
			if (send_len < (unsigned int)len) {
				loge("tcp send error,source len=%d B,send len=%d", len, send_len);
			}
			return send_len;
		}
		return -1;
	}
	*/

	void ClientImp::SetReceiveDataCallback(ReceiveDateDelegate &callback)
	{
		callback_ = callback;
	}
	void ClientImp::SendTextMsg(unsigned int to_session_id, bool is_group, std::string text, const SendMsgCallback &callback)
	{
		time_t t;
		time(&t);
		unsigned int seq = ThreadSafeGetSeq();

		// 消息内容端对端加密
		char *msg_buf = nullptr;
		unsigned int out_len = 0;
		if (EncryptMsg(text.c_str(), text.length(), &msg_buf, out_len) != 0) {
			loge("消息加密失败！");
			Free(msg_buf);
			return;
		}
		std::string encrypt_msg(msg_buf, out_len);
		Free(msg_buf);

		MessageModel model;
		model.from_user_id = user_info_.user_id();
		model.to_session_id = to_session_id;
		model.msg_id = 0;
		model.msg_type = is_group ? IM::BaseDefine::MSG_TYPE_GROUP_TEXT : IM::BaseDefine::MSG_TYPE_SINGLE_TEXT;
		model.create_time = (unsigned int)t + server_time_offset_;
		model.callback = callback;
		model.msg_data = encrypt_msg;
		model.seq = seq;

		// 添加到ack确认列表中
		msg_mutex_.lock();
		send_msg_map_.insert(make_pair(seq, model));
		send_msg_count_++;
		msg_mutex_.unlock();

		IM::Message::IMMsgData req;
		req.set_from_user_id(model.from_user_id);
		req.set_to_session_id(model.to_session_id);
		req.set_msg_id(model.msg_id);
		req.set_msg_type(model.msg_type);

		req.set_create_time(model.create_time);
		req.set_msg_data(model.msg_data);

		Send(IM::BaseDefine::SID_MSG, IM::BaseDefine::CID_MSG_DATA, model.seq, &req);
		logd("发送一条文字消息。to_sesion_id=%d,seq=%d", model.to_session_id, model.seq);
	}

	void ClientImp::ReceiveThreadProc()
	{
		unsigned char buf[MAX_RECV_BUFFER_LEN] = { 0 };

		while (receive_thread_run_)
		{
			if (tcp_client_->is_open() && client_state_ == ClientState::kClientConnectedOk) {
				boost::system::error_code err_code;
				// 阻塞直到一下次有数据
				size_t len = tcp_client_->read_some(boost::asio::buffer(buf, MAX_RECV_BUFFER_LEN), err_code);

				if (err_code == boost::asio::error::eof) {
					loge("remote has closed connection");
					tcp_client_->close();
				}
				else if (err_code) {
					boost::system::system_error err_desc(err_code);
					loge("receive thread read data error:%s", err_desc.what());
				}
				// 是否存在粘包问题？即有特大的包时，只读取了一半
				if (len > 0) {
					OnReceive(buf, len);
				}
			}
			else {
				Sleep(100);
			}
		}

		logd("socket receive thread has destory");
		tcp_client_->close();
	}

	void ClientImp::OnReceive(unsigned char* buf, int len)
	{
		PBHeader head;
		head.UnSerialize(buf, HEADER_LENGTH);

		// 心跳包
		if (head.GetCommandId() == IM::BaseDefine::CID_OTHER_HEARTBEAT) {
			time(&heartbeat_time_);
			logd("receive heartbeat:%lld", heartbeat_time_);
			return;
		}
		// 登录响应
		else if (head.GetCommandId() == IM::BaseDefine::CID_LOGIN_RES_USERLOGIN) {
			IM::Login::IMLoginRes res;
			res.ParseFromArray(buf + HEADER_LENGTH, len - HEADER_LENGTH);
			_OnLoginRes(res);
			return;
		}
		// 注销
		else if (head.GetCommandId() == IM::BaseDefine::CID_LOGIN_RES_LOGINOUT) {
			receive_thread_run_ = false;
			return;
		}
		// MsgAck回复
		else if (head.GetCommandId() == IM::BaseDefine::CID_MSG_DATA_ACK) {
			IM::Message::IMMsgDataAck res;
			res.ParseFromArray(buf + HEADER_LENGTH, len - HEADER_LENGTH);
			_OnMsgAck(head.GetSeqNumber(), res);
			return;
		}

		logd("receive new msg:moduleId=%d,msgId=%d,seq=%d", head.GetModuleId(), head.GetCommandId(), head.GetSeqNumber());
		// 去掉了协议头的数据部
		unsigned char *temp_buf = new unsigned char[len - HEADER_LENGTH];
		::memcpy(temp_buf, buf + HEADER_LENGTH, len - HEADER_LENGTH);
		if (callback_ != nullptr) {
			callback_(temp_buf, len - HEADER_LENGTH);
		}
	}

	void ClientImp::SendHeartBeat() {
		if (tcp_client_->is_open() && client_state_ == him::ClientState::kClientConnectedOk) {
			IM::Other::IMHeartBeat req;
			Send(IM::BaseDefine::SID_OTHER, IM::BaseDefine::CID_OTHER_HEARTBEAT, &req);
		}
	}
	size_t ClientImp::GetLastHeartBeatTime() {
		return (size_t)heartbeat_time_;
	}

	unsigned int ClientImp::ThreadSafeGetSeq()
	{
		unsigned int temp_seq = 0;
		{
			std::lock_guard<std::mutex> lock(seq_mutex_);
			seq_++;
			temp_seq = seq_;
		}
		return temp_seq;
	}
	void ClientImp::_OnLoginRes(IM::Login::IMLoginRes res)
	{
		logd("收到登录响应，登录结果：%d,描述：%s", res.result_code(), res.result_string().c_str());

		if (res.result_code() == 0) {
			time_t cur_time;
			time(&cur_time);

			server_time_ = res.server_time();
			server_time_offset_ = server_time_ - (unsigned int)cur_time; // 记录时间偏移

			IM::BaseDefine::UserStatType user_online_status = res.online_status();
			user_info_ = res.user_info();
			logd("服务器时间：%d，本地时间：%d，偏移量：%d，用户状态：%d", server_time_, (unsigned int)cur_time, server_time_offset_, user_online_status);
			logd("用户ID=%d,昵称：%s，部门ID=%d", user_info_.user_id(), user_info_.user_nick_name().c_str(), user_info_.department_id());
		}

		_OnLogin((int)res.result_code(), res.result_string());
	}
	void ClientImp::_OnLogin(int code, std::string msg)
	{
		if (callback_login_res_ != nullptr) {
			callback_login_res_(code, msg);
		}
	}
	void ClientImp::_OnMsgAck(unsigned int seq, IM::Message::IMMsgDataAck res)
	{
		std::map<unsigned int, MessageModel>::iterator it;
		{
			std::lock_guard<std::mutex> lock(msg_mutex_);
			send_msg_success_count_++;
			it = send_msg_map_.find(seq);
		}
		if (it != send_msg_map_.end()) {
			MessageModel item = send_msg_map_.at(seq);
			if (item.callback != nullptr) {
				item.callback(seq, true);
			}
			send_msg_map_.erase(seq);
		}
		logd("来自于服务器的消息ack收到回复,seq=%d,msgId=%d,消息计数：[%d / %d]", seq, res.msg_id(), send_msg_success_count_, send_msg_count_);
	}
}

