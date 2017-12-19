#ifndef __URL_UPLOADER_DLLMAIN_H
#define __URL_UPLOADER_DLLMAIN_H

#ifdef  __cplusplus
extern "C" {
#endif

#if defined(WIN32) || defined(_WIN32) || defined(__SYMBIAN32__)
#  if defined(UPLOAD_LIB) //
#    define CURL_EXTERN  __declspec(dllexport)
#  else
#    define URL_UPLOAD_EXTERN  __declspec(dllimport)
#  endif
#endif

	//true if init success
	URL_UPLOAD_EXTERN bool url_upload_init();

	URL_UPLOAD_EXTERN void url_upload_post(const char* url, const char* post_field);

	URL_UPLOAD_EXTERN void url_upload_cleanup();


#ifdef  __cplusplus
	}
#endif

#endif
