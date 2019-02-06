/** @file http_client_imp.h
  * @brief http µœ÷
  * @author Captain China
  * @date 2019/02/06
  */

#ifndef _HTTP_CLIENT_IMP_5B6B79E7_4F1B_4123_93F9_EF37E67E9FAF_H_
#define _HTTP_CLIENT_IMP_5B6B79E7_4F1B_4123_93F9_EF37E67E9FAF_H_

#include "api/ihttp_client.h"
#include "curl/curl.h"

namespace him {

	class HttpClientImp : public IHttpClient
	{
	public:
		virtual UrlCode HttpGetReq(const std::string &url, std::string &response, int time_out_second = 3) override;
		virtual UrlCode HttpPostReq(const std::string &url, const std::string &post_params, std::string &response, int time_out_second = 3) override;
	};

}

#endif//_HTTP_CLIENT_IMP_5B6B79E7_4F1B_4123_93F9_EF37E67E9FAF_H_