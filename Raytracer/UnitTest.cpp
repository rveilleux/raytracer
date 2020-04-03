#include "UnitTest.h"
#include <iostream>
#include <string_view>

UnitTest::UnitTest(const std::string_view& title) {
	std::cout << "Running tests '" << title << "'" << std::endl;
}

UnitTest::~UnitTest() {
	try {
		std::cout << "Completed. Ran " << _countTest << " test(s)." << std::endl;
	}
	catch (...) {
	}
}

#if _DEBUG
#define TEXT_NAME text
#else
#define TEXT_NAME
#endif
void UnitTest::Run(const std::string_view& title, const wchar_t* TEXT_NAME, const std::function<bool()>& proc) {
	std::cout << "   Scenario: " << title << "...";
	_countTest++;
	const bool result = proc();

	_ASSERT_EXPR(result, text);
	std::cout << "Success." << std::endl;
}
#undef TEXT_NAME
