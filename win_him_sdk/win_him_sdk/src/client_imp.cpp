#include "stdafx.h"
#include "client_imp.h"

#include "api/ihttp_client.h"
#include "api/log.h"

#include "json/json.h"

namespace him {
	std::shared_ptr<IClient> GetClientModule()
	{
		std::shared_ptr<ClientImp> instance = std::make_shared<ClientImp>();
		return std::dynamic_pointer_cast<IClient>(instance);
	}

	ClientImp::ClientImp()
		: server_port_(0)
		, callback_(nullptr)
		, is_init_(false)
		, client_state_(ClientState::kClientDisconnect)
	{
		tcp_cient_ = std::make_shared<boost::asio::ip::tcp::socket>(io_server_);
	}
	ClientImp::~ClientImp()
	{

	}

	void ClientImp::Login(std::string user_name, std::string pwd, std::string server_ip, unsigned short port)
	{
		char urlBuf[128] = { 0 };
		sprintf(urlBuf, "http://%s:%d/msg_server", server_ip.c_str(), port);

		std::shared_ptr<IHttpClient> client = him::GetHttpClientModule();
		std::string response;
		logd("查询可用服务器列表中...\n");
		// 阻塞，默认3秒超时
		UrlCode code = client->HttpGetReq(std::string(urlBuf), response);
		if (code == UrlCode::URLE_OK) {
			Json::Value json_value;
			Json::Reader json_reader;
			if (!json_reader.parse(response, json_value)) {
				loge("查询服务器成功，但是解析返回json结果失败！无法继续登录 \n");
				return;
			}

			std::string msg_server_ip = json_value["priorIP"].asString();
			std::string msg_server_port = json_value["port"].asString();
			logd("查询成功，开始连接消息服务器：%s:%s \n", msg_server_ip.c_str(), msg_server_port.c_str());

			boost::system::error_code code;
			boost::asio::ip::tcp::endpoint e(boost::asio::ip::address_v4::from_string(msg_server_ip), std::stoi(msg_server_port));
			tcp_cient_->connect(e, code);

			if (code) {
				boost::system::system_error err(code);
				loge("connect error:%s \n", err.what());
				return;
			}

			logd("connect success \n");
		}
		else {
			loge("http connect error: %d \n", code);
		}
	}
	ClientState ClientImp::GetClientState()
	{
		return ClientState::kClientDisconnect;
	}
	void ClientImp::LoginOut()
	{

	}

	int ClientImp::Send(int server_id, int msg_id, const char* data, int len)
	{
		return 0;
	}
	void ClientImp::SetReceiveDataCallback(ReceiveDateDelegate callback)
	{

	}
}

