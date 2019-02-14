// robot.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <list>
#include <thread>
#include <windows.h>
#include "api/iclient.h"

struct TestMsgParam
{
	int to_session_id;
	bool is_group;
};
std::list<std::shared_ptr<him::IClient>>	g_client_list_;
int											g_client_connect_count_ = 0;
std::wstring								g_msg_list_[15] =
{
	L"用我三生烟火，换你一世迷离。\r\n",
	L"长街长，烟花繁，你挑灯回看;短亭短，红尘辗，我把萧再叹。",
	L"终是谁使弦断，花落肩头，恍惚迷离。",
	L"多少红颜悴，多少相思碎，唯留血染墨香哭乱冢。",
	L"苍茫大地一剑尽挽破，何处繁华笙歌落。",
	L"相逢不语，一朵芙蓉著秋雨。",
	L"雨歇微凉，十一年前梦一场。",
	L"肠断月明红豆蔻，月似当时，人似当时否？",
	L"一心待来生，愿来生再续未了缘，可有来生？",
	L"万帐穹庐人醉，星影摇摇欲坠。",
	L"千年望等回身笑，只怨仙姑画中人。",
	L"何如薄幸锦衣郎，比翼连枝当日愿。",
	L"若似月轮终皎洁，不辞冰雪为卿热。",
	L"我是人间惆怅客，知君何事泪纵横。",
	L"西风多少恨，吹不散眉弯。",
};

std::string WChar2Ansi(LPCWSTR pwszSrc)
{
	int nLen = WideCharToMultiByte(CP_UTF8, 0, pwszSrc, -1, NULL, 0, NULL, NULL);

	if (nLen <= 0) return std::string("");

	char* pszDst = new char[nLen];
	if (NULL == pszDst) return std::string("");

	WideCharToMultiByte(CP_UTF8, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
	pszDst[nLen - 1] = 0;

	std::string strTemp(pszDst);
	delete[] pszDst;

	return strTemp;
}
std::string ws2s(std::wstring& inputws) { return WChar2Ansi(inputws.c_str()); }

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
		sprintf(user, "robot%d", 190 + i);

		// 1个用户在线、一个线程
		// 为什么不能通过线程调用，否则就会崩溃？
		//std::thread t([](char* user_name) {
		login(std::string(user));
		//}, user);

		// 测试并发
		std::this_thread::sleep_for(std::chrono::milliseconds(connnect_time_span));
	}
}


DWORD WINAPI msg_thread_proc(LPVOID lpParameter) {

	int	cur_text_index_ = 0;
	int max_text_len = sizeof(g_msg_list_) / sizeof(g_msg_list_[0]);
	TestMsgParam *param = (TestMsgParam*)lpParameter;
	unsigned int to_session_id = param->to_session_id;
	bool is_group = param->is_group;

	while (g_client_connect_count_ > 0)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));


		std::list<std::shared_ptr<him::IClient>>::iterator it;
		for (it = g_client_list_.begin(); it != g_client_list_.end(); it++)
		{
			std::shared_ptr<him::IClient> client = *it;
			if (client->GetClientState() == him::ClientState::kClientConnectedOk) {
				std::string text = ws2s(g_msg_list_[cur_text_index_]);
				
				// 匿名函数
				auto callback = [](int seq, bool result) {

				};
				auto callback_f = std::bind(callback, std::placeholders::_1, std::placeholders::_2);

				client->SendTextMsg(to_session_id, is_group, text, callback_f);
			}
		}

		cur_text_index_++;
		if (cur_text_index_ >= max_text_len) {
			cur_text_index_ = 0;
		}
	}

	return 0;
}

void test_msg_robot(int robot_count, int session_id, bool is_group) {
	// 这些机器人首先得登录
	test_login_robot(robot_count);

	// 定时发送消息（这里用std::thread不知道为什么会崩溃）
	TestMsgParam param{ session_id,is_group };
	CreateThread(NULL, 0, msg_thread_proc, &param, 0, NULL);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // 参数传递需要时间
}


int main()
{
	him::GlobalInit();

	const int test_type = 2;
	const int robot_count = 100;

	switch (test_type)
	{
	case 1:
		// 批量用户登录（频率太高会崩溃，不知道为啥）
		test_login_robot(robot_count, 100);
		break;
	case 2:
		// 批量用户给某个会话发消息
		test_msg_robot(robot_count, 1, true); // session_id，群聊：群id，单聊：用户id
		break;
	}

	system("pause");

	login_all_out();
	him::GlobalUninit();
}