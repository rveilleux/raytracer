#pragma once
#include <string>
#include <functional>

class UnitTest {
public:
	explicit UnitTest(const std::string_view& title);
	~UnitTest();
	UnitTest() = delete;
	UnitTest(const UnitTest&) = delete;
	UnitTest(UnitTest&&) = delete;
	UnitTest& operator=(const UnitTest&) = delete;
	UnitTest& operator=(UnitTest&&) = delete;

	void Run(const std::string_view& title, const wchar_t* text, const std::function<bool()>& proc);
#define DOTEST(unit, title, func, ...) unit.Run(title, L#func","#__VA_ARGS__, [&]() func, __VA_ARGS__);
private:
	int _countTest = 0;
};
