#include<upload.h>
#include<cstdio>
#include<thread>
#include<mutex>
#include<string>


bool stop = false;
std::mutex mtx;
int index = 0;

void post_index(int thread_index)
{
	while (!stop)
	{
		mtx.lock();
		std::string url{ std::string{ "thread:" }+std::to_string(thread_index) + " post: " + std::to_string(index++) };
		mtx.unlock();

		url_upload_post( url.c_str() , "");

		std::this_thread::sleep_for(std::chrono::duration<int>{1});
	}
}


int main()
{
	printf("press any key to start, and again press any key to exits");
	getchar();

	if (!url_upload_init())
	{
		printf("init failed");
		return 0;
	}

	/*mutl-thread post test, always failed*/
	getchar();
	std::thread threads[4];
	for (int i = 0; i < 4; ++i)
	{
		threads[i] = std::thread{ post_index, i };
	}

	/*clean up and exist*/
	getchar();
	stop = true;
	for (int i = 0; i < 4; ++i)
	{
		if (threads[i].joinable())
		{
			threads[i].join();
		}
	}

	/*post success test*/
	url_upload_post("47.89.246.81:8080/DatingHelper/user/login.do", "account=1157988949@qq.com&passwd=1234567");

	url_upload_cleanup();
	getchar();
}
