#include<string>
#include<atomic>
#include<thread>
#include<list>
#include<iostream>
#include<queue>
#include<condition_variable>
#include"signal.h"
using namespace std;

std::queue<int> que;
std::condition_variable cv;
std::mutex mtx;
std::mutex mtx2;
url_upload::signal queue_signal;
int index = 0;

void consume()
{
	while (1)
	{
		std::unique_lock<std::mutex> lck{ mtx };
		cv.wait(lck);
		int data = que.front();
		mtx2.lock();
		que.pop();
		mtx2.unlock();
		//
		std::cout << " consume: " <<  data << endl;
		//mtx2.unlock();
	}
}

void create(int i)
{
	while (1)
	{
		mtx2.lock();
		que.emplace(index);
		++index;
		mtx2.unlock();

		std::cout << "thread :" << i << " create " << index-1 << endl;
		cv.notify_one();
		Sleep(1000);
		std::this_thread::yield();
	}
}


int main()
{
	std::thread cth[4];
	for (int i = 0; i < 4; ++i)
	{
		cth[i] = std::thread( create,i );
	}
	std::thread csm{ consume };
	
	csm.join();
	for (int i = 0; i < 4; ++i)
	{
		cth[i].join();
	}

	return 0;
}


