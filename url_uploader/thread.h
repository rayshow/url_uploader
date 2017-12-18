#ifndef __URL_UPLOADER_THREAD_H
#define __URL_UPLOADER_THREAD_H

#include<process.h>
#include<Windows.h>
#include<functional>

namespace url_upload
{
	template<typename Fn>
	class thread:
	{
	private:
		std::function<Fn> fn;
		HANDLE handle;

	public:

		template<typename T>
		thread()

	};

}



#endif __URL_UPLOADER_THREAD_H