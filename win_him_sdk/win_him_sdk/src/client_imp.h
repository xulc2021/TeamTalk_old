/** @file client_imp.h
  * @brief 
  * @author Captain China
  * @date 2019/01/27
  */

#ifndef _CLIENT_IMP_FC05AC64_1E43_46D5_9781_F90C2803E96E_H_
#define _CLIENT_IMP_FC05AC64_1E43_46D5_9781_F90C2803E96E_H_

#include "api/client.h"
#include <boost/asio.hpp>

namespace him {

	/** @class Client_Imp
	  * @brief IClient µœ÷
	  */
	class ClientImp : public IClient
	{
	public:
		ClientImp();
		~ClientImp();

	public:
		virtual void Init();
		virtual void Uninit();

		virtual void Login(std::string user_name, std::string pwd, std::string server_ip, unsigned short port);
		virtual ClientState GetClientState();
		virtual void LoginOut();

		virtual int Send(int server_id, int msg_id, const char* data, int len);
		virtual void SetReceiveDataCallback(ReceiveDateDelegate callback);

	private:
		ClientState					client_state_;
		ReceiveDateDelegate			*callback_;
		std::string					server_ip_;
		std::string					user_name_;
		unsigned short				server_port_;
		bool						is_init_;

		// boost
		boost::asio::io_service		io_server_;
		std::shared_ptr<boost::asio::ip::tcp::socket> tcp_cient_;
	};
}

#endif//_CLIENT_IMP_FC05AC64_1E43_46D5_9781_F90C2803E96E_H_