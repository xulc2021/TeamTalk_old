// robot.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>

#include "api/client.h"

int main()
{
	std::shared_ptr<him::IClient> instance = him::FactoryNew();
	instance->Init();

	instance->Login("gaozz", "123456", "106.14.172.35", 31001);

    std::cout << "Hello World!\n"; 
}