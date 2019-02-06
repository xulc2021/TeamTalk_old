/** @file http_client.h
  * @brief http·â×°
  * @author Captain China
  * @date 2019/02/06
  */

#ifndef _HTTP_CLIENT_DE8DCC15_5CF9_4942_BED2_8D85AE8B07F0_H_
#define _HTTP_CLIENT_DE8DCC15_5CF9_4942_BED2_8D85AE8B07F0_H_

#include "him_sdk_dll.h"
#include <string>

namespace him {

	typedef enum {
		URLE_OK = 0,
		URLE_UNSUPPORTED_PROTOCOL,    /* 1 */
		URLE_FAILED_INIT,             /* 2 */
		URLE_URL_MALFORMAT,           /* 3 */
		URLE_NOT_BUILT_IN,            /* 4 - [was obsoleted in August 2007 for
										  7.17.0, reused in April 2011 for 7.21.5] */
		URLE_COULDNT_RESOLVE_PROXY,   /* 5 */
		URLE_COULDNT_RESOLVE_HOST,    /* 6 */
		URLE_COULDNT_CONNECT,         /* 7 */
		URLE_WEIRD_SERVER_REPLY,      /* 8 */
		URLE_REMOTE_ACCESS_DENIED,    /* 9 a service was denied by the server
										  due to lack of access - when login fails
										  this is not returned. */
		URLE_FTP_ACCEPT_FAILED,       /* 10 - [was obsoleted in April 2006 for
										  7.15.4, reused in Dec 2011 for 7.24.0]*/
		URLE_FTP_WEIRD_PASS_REPLY,    /* 11 */
		URLE_FTP_ACCEPT_TIMEOUT,      /* 12 - timeout occurred accepting server
										  [was obsoleted in August 2007 for 7.17.0,
										  reused in Dec 2011 for 7.24.0]*/
		URLE_FTP_WEIRD_PASV_REPLY,    /* 13 */
		URLE_FTP_WEIRD_227_FORMAT,    /* 14 */
		URLE_FTP_CANT_GET_HOST,       /* 15 */
		URLE_HTTP2,                   /* 16 - A problem in the http2 framing layer.
										  [was obsoleted in August 2007 for 7.17.0,
										  reused in July 2014 for 7.38.0] */
		URLE_FTP_COULDNT_SET_TYPE,    /* 17 */
		URLE_PARTIAL_FILE,            /* 18 */
		URLE_FTP_COULDNT_RETR_FILE,   /* 19 */
		URLE_OBSOLETE20,              /* 20 - NOT USED */
		URLE_QUOTE_ERROR,             /* 21 - quote command failure */
		URLE_HTTP_RETURNED_ERROR,     /* 22 */
		URLE_WRITE_ERROR,             /* 23 */
		URLE_OBSOLETE24,              /* 24 - NOT USED */
		URLE_UPLOAD_FAILED,           /* 25 - failed upload "command" */
		URLE_READ_ERROR,              /* 26 - couldn't open/read from file */
		URLE_OUT_OF_MEMORY,           /* 27 */
		/* Note: URLE_OUT_OF_MEMORY may sometimes indicate a conversion error
				 instead of a memory allocation error if CURL_DOES_CONVERSIONS
				 is defined
		*/
		URLE_OPERATION_TIMEDOUT,      /* 28 - the timeout time was reached */
		URLE_OBSOLETE29,              /* 29 - NOT USED */
		URLE_FTP_PORT_FAILED,         /* 30 - FTP PORT operation failed */
		URLE_FTP_COULDNT_USE_REST,    /* 31 - the REST command failed */
		URLE_OBSOLETE32,              /* 32 - NOT USED */
		URLE_RANGE_ERROR,             /* 33 - RANGE "command" didn't work */
		URLE_HTTP_POST_ERROR,         /* 34 */
		URLE_SSL_CONNECT_ERROR,       /* 35 - wrong when connecting with SSL */
		URLE_BAD_DOWNLOAD_RESUME,     /* 36 - couldn't resume download */
		URLE_FILE_COULDNT_READ_FILE,  /* 37 */
		URLE_LDAP_CANNOT_BIND,        /* 38 */
		URLE_LDAP_SEARCH_FAILED,      /* 39 */
		URLE_OBSOLETE40,              /* 40 - NOT USED */
		URLE_FUNCTION_NOT_FOUND,      /* 41 */
		URLE_ABORTED_BY_CALLBACK,     /* 42 */
		URLE_BAD_FUNCTION_ARGUMENT,   /* 43 */
		URLE_OBSOLETE44,              /* 44 - NOT USED */
		URLE_INTERFACE_FAILED,        /* 45 - CURLOPT_INTERFACE failed */
		URLE_OBSOLETE46,              /* 46 - NOT USED */
		URLE_TOO_MANY_REDIRECTS,      /* 47 - catch endless re-direct loops */
		URLE_UNKNOWN_OPTION,          /* 48 - User specified an unknown option */
		URLE_TELNET_OPTION_SYNTAX,    /* 49 - Malformed telnet option */
		URLE_OBSOLETE50,              /* 50 - NOT USED */
		URLE_PEER_FAILED_VERIFICATION, /* 51 - peer's certificate or fingerprint
										   wasn't verified fine */
		URLE_GOT_NOTHING,             /* 52 - when this is a specific error */
		URLE_SSL_ENGINE_NOTFOUND,     /* 53 - SSL crypto engine not found */
		URLE_SSL_ENGINE_SETFAILED,    /* 54 - can not set SSL crypto engine as
										  default */
		URLE_SEND_ERROR,              /* 55 - failed sending network data */
		URLE_RECV_ERROR,              /* 56 - failure in receiving network data */
		URLE_OBSOLETE57,              /* 57 - NOT IN USE */
		URLE_SSL_CERTPROBLEM,         /* 58 - problem with the local certificate */
		URLE_SSL_CIPHER,              /* 59 - couldn't use specified cipher */
		URLE_SSL_CACERT,              /* 60 - problem with the CA cert (path?) */
		URLE_BAD_CONTENT_ENCODING,    /* 61 - Unrecognized/bad encoding */
		URLE_LDAP_INVALID_URL,        /* 62 - Invalid LDAP URL */
		URLE_FILESIZE_EXCEEDED,       /* 63 - Maximum file size exceeded */
		URLE_USE_SSL_FAILED,          /* 64 - Requested FTP SSL level failed */
		URLE_SEND_FAIL_REWIND,        /* 65 - Sending the data requires a rewind
										  that failed */
		URLE_SSL_ENGINE_INITFAILED,   /* 66 - failed to initialise ENGINE */
		URLE_LOGIN_DENIED,            /* 67 - user, password or similar was not
										  accepted and we failed to login */
		URLE_TFTP_NOTFOUND,           /* 68 - file not found on server */
		URLE_TFTP_PERM,               /* 69 - permission problem on server */
		URLE_REMOTE_DISK_FULL,        /* 70 - out of disk space on server */
		URLE_TFTP_ILLEGAL,            /* 71 - Illegal TFTP operation */
		URLE_TFTP_UNKNOWNID,          /* 72 - Unknown transfer ID */
		URLE_REMOTE_FILE_EXISTS,      /* 73 - File already exists */
		URLE_TFTP_NOSUCHUSER,         /* 74 - No such user */
		URLE_CONV_FAILED,             /* 75 - conversion failed */
		URLE_CONV_REQD,               /* 76 - caller must register conversion
										  callbacks using curl_easy_setopt options
										  CURLOPT_CONV_FROM_NETWORK_FUNCTION,
										  CURLOPT_CONV_TO_NETWORK_FUNCTION, and
										  CURLOPT_CONV_FROM_UTF8_FUNCTION */
		URLE_SSL_CACERT_BADFILE,      /* 77 - could not load CACERT file, missing
										  or wrong format */
		URLE_REMOTE_FILE_NOT_FOUND,   /* 78 - remote file not found */
		URLE_SSH,                     /* 79 - error from the SSH layer, somewhat
										  generic so the error message will be of
										  interest when this has happened */

		URLE_SSL_SHUTDOWN_FAILED,     /* 80 - Failed to shut down the SSL
										  connection */
		URLE_AGAIN,                   /* 81 - socket is not ready for send/recv,
										  wait till it's ready and try again (Added
										  in 7.18.2) */
		URLE_SSL_CRL_BADFILE,         /* 82 - could not load CRL file, missing or
										  wrong format (Added in 7.19.0) */
		URLE_SSL_ISSUER_ERROR,        /* 83 - Issuer check failed.  (Added in
										  7.19.0) */
		URLE_FTP_PRET_FAILED,         /* 84 - a PRET command failed */
		URLE_RTSP_CSEQ_ERROR,         /* 85 - mismatch of RTSP CSeq numbers */
		URLE_RTSP_SESSION_ERROR,      /* 86 - mismatch of RTSP Session Ids */
		URLE_FTP_BAD_FILE_LIST,       /* 87 - unable to parse FTP file list */
		URLE_CHUNK_FAILED,            /* 88 - chunk callback reported error */
		URLE_NO_CONNECTION_AVAILABLE, /* 89 - No connection available, the
										  session will be queued */
		URLE_SSL_PINNEDPUBKEYNOTMATCH, /* 90 - specified pinned public key did not
										   match */
		URLE_SSL_INVALIDCERTSTATUS,   /* 91 - invalid certificate status */
		URLE_HTTP2_STREAM,            /* 92 - stream error in HTTP/2 framing layer
										  */
		CURL_LAST /* never use! */
	} UrlCode;

	class HIM_SDK_API IHttpClient
	{
	public:
		virtual UrlCode HttpGetReq(const std::string &url, std::string &response,int time_out_second = 3) = 0;
		virtual UrlCode HttpPostReq(const std::string &url, const std::string &post_params, std::string &response, int time_out_second = 3) = 0;
	};

	HIM_SDK_API std::shared_ptr<IHttpClient> GetHttpClientModule();
}

#endif//_HTTP_CLIENT_DE8DCC15_5CF9_4942_BED2_8D85AE8B07F0_H_