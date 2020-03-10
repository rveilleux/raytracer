#include "Thread.h"

Thread::Thread(const std::function<void(void)>& func)
	: _thread(func)
{
}

Thread::Thread(const Thread&) {
	_ASSERT_EXPR(false, L"Thread: Copy-constructor is unsupported!");
}

Thread::~Thread()
{
	_thread.join();
}

ThreadList::ThreadList():
	ThreadList(std::thread::hardware_concurrency()) 
{
}

ThreadList::ThreadList(int nbThread) {
	_ASSERT_EXPR(nbThread > 0, L"ThreadList: number of thread must be greater than 0");
	_count = nbThread;
	_threads.reserve(_count);
}

void ThreadList::Add(const std::function<void(void)>& func) {
	_threads.emplace_back(func);
}
