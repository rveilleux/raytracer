#pragma once
#include <chrono>

class Chrono {
public:
	Chrono() noexcept;
	double GetElapsedTime() const noexcept;
private:
	const std::chrono::time_point<std::chrono::system_clock> _start;
};
