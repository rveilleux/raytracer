#pragma once
#include <chrono>

class Chrono {
public:
	Chrono();
	double GetElapsedTime() const;
private:
	const std::chrono::time_point<std::chrono::system_clock> _start;
};
