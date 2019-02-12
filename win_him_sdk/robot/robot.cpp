// robot.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>

#include "api/iclient.h"

void login_result(int code, std::string msg)
{
	printf("login result:%d", code);
}

int main()
{
	him::GlobalInit();

	std::shared_ptr<him::IClient> instance = him::GetClientModule();

	auto callback = std::bind(login_result, std::placeholders::_1, std::placeholders::_2);

	//instance->Login("gaozz", "123456", "106.14.172.35", 31001, callback);
	instance->Login("gaozz", "e10adc3949ba59abbe56e057f20f883e", "106.14.172.35", 31001, callback);

	std::cout << "Hello World!\n";

	him::GlobalUninit();

	system("pause");
}