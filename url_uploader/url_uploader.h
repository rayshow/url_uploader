#ifndef __URL_UPLOADER_H
#define __URL_UPLOADER_H

//3rd lockfree queue
#include<concurrentqueue/concurrentqueue.h>
//3rd libcurl
#include<curl/multi.h>
#include<thread>
#include<functional>
#include<exception>

#include"signal.h"

namespace url_upload
{
#ifdef _DEBUG
	#define DebugInfo(s, ...) printf(s, __VA_ARGS__);
#elif 
	#define DebugInfo(s, ...) 
#endif

	typedef std::pair<std::string, std::string> post_item;

	static size_t cb(char *d, size_t n, size_t l, void *p)
	{
		/* take care of the data here, ignored in this example */
		(void)d;
		(void)p;
		return n*l;
	}

	class url_uploader
	{
	private:
		signal signal;
		CURL*  curl_handle;
		std::thread upload_thread;
		bool is_running;
		moodycamel::ConcurrentQueue<post_item> post_queue;
		
		bool do_post( post_item& item)
		{
			DebugInfo(" posting: url %s  post_field: %s\n", item.first.c_str(), item.second.c_str() );
			int ret = CURLE_OK;
			ret |= curl_easy_setopt(curl_handle, CURLOPT_URL, item.first.c_str());
			ret |= curl_easy_setopt(curl_handle, CURLOPT_POST, 1L);
			ret |= curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDSIZE, item.second.length() );
			ret |= curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, item.second.c_str());
			ret |= curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, cb);  //back data of post, ingore
			ret |= curl_easy_perform(curl_handle);
			if (ret != CURLE_OK) return false;
			return true;
		}

		void running()
		{
			DebugInfo("%s\n", "thread running");
			post_item item;
			while (is_running)
			{
				signal.wait();
				while (post_queue.try_dequeue(item))
				{
					int state = do_post(item);
					DebugInfo(" posted state: %d \n", state);
				}
			}
		}

	public:
		//throw bad_alloc if curl init failed
		url_uploader()
			: signal{},
			curl_handle{nullptr},
			upload_thread{}
		{
			DebugInfo("%s\n", "uploader initalizing");
			curl_global_init(CURL_GLOBAL_ALL);
			curl_handle = curl_easy_init();
			if (!curl_handle)
			{
				DebugInfo("%s\n", "initalize failed");
				throw std::bad_alloc{};
			}
			is_running = true;
			upload_thread = std::thread{ std::bind(&url_uploader::running, this) };
			DebugInfo("%s\n", "uploader initalized");
		}

		void post( std::string url , std::string post_field )
		{
			post_queue.enqueue(std::make_pair(std::move(url), std::move(post_field)));
			signal.notify_one();
		}

		~url_uploader()
		{
			//wait thread to exit
			is_running = false;
			signal.notify_one();
			if(upload_thread.joinable())
				upload_thread.join();
			DebugInfo("%s\n", " uploader thread end");
			curl_easy_cleanup(curl_handle);
			curl_handle = nullptr;
			curl_global_cleanup();
			DebugInfo("%s\n", " uploader curl end");
		}
	};
}





#endif