#pragma once
#include <thread>
#include <functional>
#include <vector>

class Thread {
public:
	Thread() = delete;
	Thread(const Thread&) noexcept;
	Thread(Thread&&) = default;
	Thread(const std::function<void(void)>& func);
	Thread& operator=(const Thread&) = delete;
	Thread& operator=(Thread&&) = delete;
	~Thread();
private:
	std::thread _thread;
};

class ThreadList {
public:
	ThreadList();
	ThreadList(int nbThread);
	int GetCount() const noexcept { return _count; }
	void Add(const std::function<void(void)>& func);
private:
	int _count;
	std::vector<Thread> _threads;
};
