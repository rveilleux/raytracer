#pragma once
#include <thread>
#include <functional>
#include <vector>

class Thread {
public:
	Thread() = delete;
	Thread(const Thread&);
	Thread(const std::function<void(void)>& func);
	~Thread();
private:
	std::thread _thread;
};

class ThreadList {
public:
	ThreadList();
	ThreadList(int nbThread);
	int GetCount() const { return _count; }
	void Add(const std::function<void(void)>& func);
private:
	int _count;
	std::vector<Thread> _threads;
};
