#include "client_imp.h"

namespace him {
	std::shared_ptr<IClient> FactoryNew()
	{
		return std::make_shared<IClient>(new ClientImp());
	}


	ClientImp::ClientImp()
		: server_ip_(nullptr)
		, server_port_(0)
		, user_name_(nullptr)
		, callback_(nullptr)
		, is_init_(false)
		, client_state_(ClientState::kClientDisconnect)
	{

	}
	ClientImp::~ClientImp()
	{

	}


	void ClientImp::Init()
	{
	}

	void ClientImp::Uninit()
	{

	}

	void ClientImp::Login(std::string user_name, std::string pwd, std::string server_ip, unsigned short port)
	{

	}
	ClientState ClientImp::GetClientState()
	{

	}
	void ClientImp::LoginOut()
	{

	}

	int ClientImp::Send(int server_id, int msg_id, const char* data, int len)
	{

	}
	void ClientImp::SetReceiveDataCallback(ReceiveDateDelegate callback)
	{

	}
}

