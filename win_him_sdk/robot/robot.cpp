// robot.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <list>
#include "api/iclient.h"

std::list<std::shared_ptr<him::IClient>>	g_client_list_;
int											g_client_connect_count_ = 0;

void login_result(int code, std::string msg)
{
	g_client_connect_count_++;
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
}

int main()
{
	him::GlobalInit();
	
	login("gaozz");
	
	system("pause");

	login_all_out();
	him::GlobalUninit();
}