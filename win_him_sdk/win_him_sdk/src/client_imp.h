/** @file client_imp.h
  * @brief
  * @author Captain China
  * @date 2019/01/27
  */

#ifndef _CLIENT_IMP_FC05AC64_1E43_46D5_9781_F90C2803E96E_H_
#define _CLIENT_IMP_FC05AC64_1E43_46D5_9781_F90C2803E96E_H_

#include "api/iclient.h"
#include "protocol/IM.Login.pb.h"
#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>
#include <thread>

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
		virtual void Login(std::string user_name, std::string pwd, std::string server_ip, unsigned short port, const LoginResultCallback &callback);
		virtual ClientState GetClientState();
		virtual void LoginOut();

		virtual int Send(int server_id, int msg_id, const unsigned char* data, int len);
		virtual void SetReceiveDataCallback(ReceiveDateDelegate &callback);

	public:
		void ReceiveThreadProc();
		virtual void OnReceive(unsigned char* buf, int len);
	private:
		int ThreadSafeGetSeq();
		void _OnLoginRes(IM::Login::IMLoginRes res);
		void _OnLogin(int code, std::string msg);
	private:
		ClientState					client_state_;
		ReceiveDateDelegate			callback_;
		LoginResultCallback			callback_login_res_;

		std::string					server_ip_;
		std::string					user_name_;
		unsigned short				server_port_;
		unsigned int				seq_;
		unsigned char*				write_buffer_;
		bool						receive_thread_run_;

		// boost
		boost::asio::io_service		io_server_;
		boost::mutex				seq_mutex_;
		std::shared_ptr<boost::asio::ip::tcp::socket>	tcp_client_;
		std::shared_ptr<std::thread>					receive_thread_;
	};
}

#endif//_CLIENT_IMP_FC05AC64_1E43_46D5_9781_F90C2803E96E_H_