/** @file client.h
  * @brief SDK全局管理，比如登录、注销等
  * @author Captain China
  * @date 2019/01/27
  */

#ifndef _CLIENT_FD1A9425_314E_4337_89BE_B3F537C78A8F_H_
#define _CLIENT_FD1A9425_314E_4337_89BE_B3F537C78A8F_H_

#include "him_sdk_dll.h"
#include <memory>
#include <string>
#include <functional>

namespace him {

	/** @enum ClientState
	  * @brief
	  */
	typedef enum ClientState
	{
		kClientConnectedOk = 0, // 已连接
		kClientDisconnect = 1,  // 连接断开
		kClientReConnect = 2,   // 重连中
	};

#define BIND_CALLBACK_2(func)	std::bind(&func, this, placeholders::_1, placeholders::_2)
	typedef std::function<void(const char* data, int len)> ReceiveDateDelegate; // 接收到数据的回调函数定义

	HIM_SDK_API class IClient
	{
	public:// a

		/** @fn void Init();
		  * @brief 初始化sdk
		  */
		virtual void Init() = 0;

		/** @fn void Uninit()
		  * @brief 退出前调用
		  */
		virtual void Uninit() =0;

	public:// login

		/** @fn void Login(std::string user_name, std::string pwd, std::string serverIp, unsigned short port)
		  * @brief 登录
		  * @param user_name: 用户名
		  * @param pwd: 密码
		  * @param server_ip: 服务器ip地址
		  * @param port: 端口
		  */
		virtual void Login(std::string user_name, std::string pwd, std::string server_ip, unsigned short port) =0;

		/** @fn ClientState GetClientState()
		  * @brief 获取当前客户端连接状态
		  * @return 状态
		  */
		virtual ClientState GetClientState() = 0;

		/** @fn void LoginOut()
		  * @brief 注销
		  */
		virtual void LoginOut() =0;

	public:// send
		/** @fn int Send(int server_id, int msg_id, const char* data, int len)
		  * @brief 发送
		  * @param server_id: 模块ID
		  * @param msg_id: 消息ID
		  * @param data: 数据缓冲区
		  * @param len: 要发送的长度
		  * @return 已发送的数据长度
		  */
		virtual int Send(int server_id, int msg_id, const char* data, int len) =0;
		virtual void SetReceiveDataCallback(ReceiveDateDelegate callback) =0;
	};

	/** @fn std::shared_ptr<IClient> FactoryNew();
	  * @brief 获取Client的新实例
	  * @retrun 新的实例
	  */
	HIM_SDK_API std::shared_ptr<IClient> FactoryNew();
}

#endif//_CLIENT_FD1A9425_314E_4337_89BE_B3F537C78A8F_H_