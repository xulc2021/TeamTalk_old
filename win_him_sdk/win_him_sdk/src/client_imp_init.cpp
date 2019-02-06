#include "stdafx.h"
#include "client_imp.h"

// http¿â
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

	void GlobalInit()
	{
		if (!g_global_init) {
			g_global_init = true;

			// http (libcurl)
			// global init
			curl_global_init(CURL_GLOBAL_ALL);
		}
	}

	void GlobalUninit()
	{
		if (g_global_init) {
			g_global_init = false;

			// http (libcurl)
			// global release
			curl_global_cleanup();
		}
	}
}