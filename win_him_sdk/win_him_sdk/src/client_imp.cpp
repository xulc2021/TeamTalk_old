#include "stdafx.h"
#include "client_imp.h"

namespace him {
	std::shared_ptr<IClient> FactoryNew()
	{
		std::shared_ptr<ClientImp> instance = std::make_shared<ClientImp>();
		return std::dynamic_pointer_cast<IClient>(instance);
	}


	ClientImp::ClientImp()
		: server_port_(0)
		, callback_(nullptr)
		, is_init_(false)
		, client_state_(ClientState::kClientDisconnect)
		, tcp_cient_(nullptr)
	{

	}
	ClientImp::~ClientImp()
	{

	}


	void ClientImp::Init()
	{
		if (!is_init_) {
			tcp_cient_ = std::make_shared<boost::asio::ip::tcp::socket>(io_server_);

			is_init_ = true;
		}
	}

	void ClientImp::Uninit()
	{
		if (is_init_)
		{
			is_init_ = false;
		}
	}

	void ClientImp::Login(std::string user_name, std::string pwd, std::string server_ip, unsigned short port)
	{
		boost::system::error_code code;

		boost::asio::ip::tcp::endpoint e(boost::asio::ip::address_v4::from_string(server_ip), port);
		tcp_cient_->connect(e, code);

		if (code) {
			boost::system::system_error err(code);
			printf("connect error:%s \n", err.what());
			return;
		}

		printf("connect success");
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

