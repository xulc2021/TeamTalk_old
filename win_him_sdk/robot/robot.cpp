// robot.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <list>
#include <thread>
#include <windows.h>
#include "api/iclient.h"

std::list<std::shared_ptr<him::IClient>>	g_client_list_;
int											g_client_connect_count_ = 0;




void login_result(int code, std::string msg)
{
	g_client_connect_count_++;
	printf("[%d]个用户已连接 \n", g_client_connect_count_);
}
void login(std::string user_name) {
	std::shared_ptr<him::IClient> instance = him::NewClientModule();
	auto callback = std::bind(login_result, std::placeholders::_1, std::placeholders::_2);
	// md5(123456) -> e10adc3949ba59abbe56e057f20f883e
	instance->Login(user_name, "e10adc3949ba59abbe56e057f20f883e", "106.14.172.35", 31001, callback);
	// 保存连接
	g_client_list_.push_back(instance);
}
void login_all_out() {
	std::list<std::shared_ptr<him::IClient>>::iterator it;
	for (it = g_client_list_.begin(); it != g_client_list_.end(); it++) {
		std::shared_ptr<him::IClient> client = *it;
		if (client != nullptr) {
			client->LoginOut();
		}
	}
	g_client_list_.clear();
	g_client_connect_count_ = 0;
}



void test_login_robot(int robot_count, int	connnect_time_span = 100) {
	for (int i = 1; i <= robot_count; i++)
	{
		char user[128] = { 0 };
		sprintf(user, "robot%d", 100 + i);

		// 1个用户在线、一个线程
		// 为什么不能通过线程调用，否则就会崩溃？
		//std::thread t([](char* user_name) {
		login(std::string(user));
		//}, user);

		// 测试并发
		Sleep(connnect_time_span);
	}
}
void test_msg_robot(int robot_count, int session_id) {
	// 这些机器人首先得登录
	test_login_robot(robot_count);

	// 定时发送消息

}


int main()
{
	him::GlobalInit();

	const int test_type = 1;
	const int robot_count = 2000;

	switch (test_type)
	{
	case 1:
		// 批量用户登录（频率太高会崩溃，不知道为啥）
		test_login_robot(robot_count, 100);
		break;
	case 2:
		// 批量用户给某个会话发消息
		break;
	}

	system("pause");

	login_all_out();
	him::GlobalUninit();
}