#include "stdafx.h"
#include "client_imp.h"

// http库
#include "curl/curl.h"
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "wldap32.lib")
#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "advapi32.lib")
#ifdef _DEBUG
#pragma comment(lib, "libcurld.lib")
#else
#pragma comment(lib, "libcurl.lib")
#endif

namespace him {
	bool g_global_init = false;
	// heartbeat
	std::list<std::weak_ptr<ClientImp>>	g_client_list_;
	boost::mutex						g_client_list_mutex_;
	std::shared_ptr<std::thread>		g_heartbeat_thread_ = nullptr;
	bool								g_heartbeat_run_ = true;

	void heartbeat_thread_proc()
	{
		while (g_heartbeat_run_)
		{
			Sleep(40 * 1000);

			{
				boost::mutex::scoped_lock lock(g_client_list_mutex_);
				std::list<std::weak_ptr<ClientImp>>::iterator it;
				for (it = g_client_list_.begin(); it != g_client_list_.end(); )
				{
					std::weak_ptr<ClientImp> item = *it;
					std::shared_ptr<ClientImp> client = item.lock();
					if (client != nullptr) {
						it++;

						// 10秒1个心跳
						time_t cur_time;
						time(&cur_time);
						if ((cur_time - client->GetLastHeartBeatTime()) > 10) {
							client->SendHeartBeat();
						}
					}
					else { // 对象已销毁，删除引用
						it = g_client_list_.erase(it);
					}
				}
			}

			Sleep(1 * 1000);
		}
	}

	void GlobalInit()
	{
		if (!g_global_init) {
			g_global_init = true;

			// http (libcurl)
			// global init
			curl_global_init(CURL_GLOBAL_ALL);

			// 心跳线程
			g_heartbeat_thread_ = std::make_shared<std::thread>(heartbeat_thread_proc);
		}
	}

	void GlobalUninit()
	{
		if (g_global_init) {
			g_global_init = false;

			// http (libcurl)
			// global release
			curl_global_cleanup();

			// 销毁心跳线程
			g_heartbeat_run_ = false;
			if (g_heartbeat_thread_ != nullptr && g_heartbeat_thread_->joinable())
			{
				g_heartbeat_thread_->join();
				g_heartbeat_thread_ = nullptr;
			}
		}
	}
}