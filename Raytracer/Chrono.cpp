#include "Chrono.h"

Chrono::Chrono() noexcept
	: _start(std::chrono::system_clock::now())
{
}

double Chrono::GetElapsedTime() const noexcept {
	const auto now = std::chrono::system_clock::now();
	const auto diff = now - _start;
	return std::chrono::duration<double, std::milli>(diff).count();
}
