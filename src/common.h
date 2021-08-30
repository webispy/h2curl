#ifndef __COMMON_H__
#define __COMMON_H__

#include <curl/curl.h>

int my_trace(CURL *handle, curl_infotype type, char *data, size_t size,
	     void *userp);

#endif
