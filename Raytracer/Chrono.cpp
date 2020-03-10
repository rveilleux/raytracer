#include "Chrono.h"

Chrono::Chrono()
	: _start(std::chrono::system_clock::now())
{
}

double Chrono::GetElapsedTime() const {
	auto now = std::chrono::system_clock::now();
	auto diff = now - _start;
	return std::chrono::duration<double, std::milli>(diff).count();
}
