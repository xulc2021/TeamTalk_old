// libcurltest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "HttpRequest.h" //https://blog.csdn.net/wyansai/article/details/51241594
#include <iostream>
#include <fstream>
#include <windows.h>

#ifdef _DEBUG
#pragma comment(lib, "../lib_curl/libcurld.lib")
#else
#pragma comment(lib, "../lib_curl/libcurl.lib")
#endif

class MyResultClass
{
public:
	MyResultClass() : m_request_finished(false) { }
	~MyResultClass() { }

public:
	void MyRequestResultCallback(int id, bool success, const std::string& data)
	{
		if (success)
		{
			//把访问的网页内容本地持久化
// 			std::ofstream outfile;
// 			outfile.open("baidu.html", std::ios_base::binary | std::ios_base::trunc);
// 			if (outfile.good())
// 			{
// 				outfile.write(data.c_str(), data.size());
// 			}
		}

		m_request_finished = true;
	}

	bool IsRequestFinish(void)
	{
		return m_request_finished;
	}

private:
	bool m_request_finished;
};

int main()
{
	MyResultClass mc;

	HttpRequest request;
	request.SetRequestUrl("http://1.25.10.15:2000/app/appLogin.action?username=guest&password=123456");//http://www.biquge.com/21_21470
	request.SetResultCallback(std::bind(&MyResultClass::MyRequestResultCallback, &mc, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	request.SetRequestHeader("User-Agent:Mozilla/4.04[en](Win95;I;Nav)");

	HANDLE hRequest = request.PerformRequest(HttpRequest::REQUEST_ASYNC);
	if (hRequest)
	{
		while (mc.IsRequestFinish() == false)
		{
			Sleep(1);
		}

		long http_code;
		if (request.GetHttpCode(hRequest, &http_code))
		{
			std::cout << "http code: " << http_code << std::endl;
		}

		std::string header;
		if (request.GetReceiveHeader(hRequest, &header))
		{
			std::cout << header << std::endl;
		}

		std::string content;
		if (request.GetReceiveContent(hRequest, &content))
		{
			std::cout << content.substr(0, 500) << std::endl;
		}

		HttpRequest::Close(hRequest);
	}

	getchar();
    return 0;
}

