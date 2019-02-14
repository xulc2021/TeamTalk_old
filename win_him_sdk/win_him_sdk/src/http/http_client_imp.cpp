#include "stdafx.h"
#include "http_client_imp.h"

namespace him {

	std::shared_ptr<IHttpClient> GetHttpClientModule()
	{
		std::shared_ptr<HttpClientImp> instance = std::make_shared<HttpClientImp>();
		return std::dynamic_pointer_cast<IHttpClient>(instance);
	}

	//原字符串，要替换的字符串，替换为什么字符串 
	void get_http_body(std::string & str)
	{
		// \r\n
		std::string::size_type pos = str.find("\r\n\r\n");
		if (pos != std::string::npos) {
			int offset = std::string("\r\n\r\n").length();
			str = str.substr(pos + offset);
		}
	}

	// reply of the requery
	size_t req_reply(void *ptr, size_t size, size_t nmemb, void *stream)
	{
		std::string *str = (std::string*)stream;
		//printf("----->reply:%s", str->c_str());
		(*str).append((char*)ptr, size*nmemb);
		return size * nmemb;
	}

	UrlCode HttpClientImp::HttpGetReq(const std::string &url, std::string &response, int time_out_second)
	{
		// init curl
		CURL *curl = curl_easy_init();
		// res code
		CURLcode res;
		if (curl)
		{
			// set params
			curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); // url
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false); // set peer and host verify false
			// 不查看内部日志
			//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
			curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, req_reply);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response);
			curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
			curl_easy_setopt(curl, CURLOPT_HEADER, 1);
			curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, time_out_second); // set transport and time out time
			curl_easy_setopt(curl, CURLOPT_TIMEOUT, time_out_second);
			// start req
			res = curl_easy_perform(curl);
		}
		// release curl
		curl_easy_cleanup(curl);

		// 解析数据部
		if (res != CURLE_OPERATION_TIMEDOUT) {
			//int http_code;
			//curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
			if (res == CURLE_OK /*&& http_code == 200*/)
			{
				get_http_body(response);
			}
		}

		int code = res;
		return (UrlCode)code;
	}

	UrlCode HttpClientImp::HttpPostReq(const std::string &url, const std::string &post_params, std::string &response, int time_out_second)
	{
		// init curl
		CURL *curl = curl_easy_init();
		// res code
		CURLcode res;
		if (curl)
		{
			// set params
			curl_easy_setopt(curl, CURLOPT_POST, 1); // post req
			curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); // url
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_params.c_str()); // params
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false); // set peer and host verify false
			curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
			curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, req_reply);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response);
			curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
			curl_easy_setopt(curl, CURLOPT_HEADER, 1);
			curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, time_out_second);
			curl_easy_setopt(curl, CURLOPT_TIMEOUT, time_out_second);
			// start req
			res = curl_easy_perform(curl);
		}
		// release curl
		curl_easy_cleanup(curl);

		int code = (int)res;
		return (UrlCode)code;
	}

}