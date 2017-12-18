#ifndef __URL_UPLOADER_SIGNAL_H
#define __URL_UPLOADER_SIGNAL_H

#include<process.h>
#include<Windows.h>
#include"noncopyable.h"

namespace url_upload
{
	class signal: noncopyable
	{
	private:
		HANDLE handle;
	public:

		__forceinline
			signal()  noexcept
		{
			handle = CreateSemaphore(nullptr, 0, 1, "url_uploader_signal");
		}

		__forceinline
			void wait()
		{
			WaitForSingleObject(handle, INFINITE);
		}

		__forceinline
			void notify_one()
		{
			ReleaseSemaphore(handle,1,nullptr);
		}

		__forceinline
		~signal()
		{
			CloseHandle(handle);
		}
	};

}
#endif