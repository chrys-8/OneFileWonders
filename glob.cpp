#include <cstdlib>
#include <iostream>
#include <string_view>

bool _groupContains(std::string_view pattern, auto& iter, auto chr)
{
	bool result = false;
	bool inverse = false;

	if (iter + 1 == pattern.end()) return false;
	else if (*(iter + 1) == ']') {
		result = chr == ']';
		iter += 2;
	}
	else if (*(iter + 1) == '!')   {
		inverse = true;
		iter += 2;
	}
	else {
		iter += 1;
	}

	while (iter != pattern.end() && *iter != ']') {
		result |= *iter == chr;
		iter++;
	}

	if (iter == pattern.end()) return false;
	return result ^ inverse;
}

bool glob(std::string_view pattern, std::string_view text)
{
	auto iterPattern = pattern.begin();
	auto iterText    = text.begin();

	auto iterLastWildcard = pattern.end();
	bool isPreviousWildcard = false;
	while (iterPattern != pattern.end() && iterText != text.end()) {
		bool isMatch = false;
		bool isWildcard = false;

		switch (*iterPattern) {
		case '?': // any
			{
				isMatch = true;
			}

		case '*': // wildcard
			{
				iterLastWildcard = iterPattern;
				isWildcard = true;
				break;
			}

		case '[':
			{
				// mutates iterPattern;
				isMatch = _groupContains(pattern, iterPattern, *iterText);
				break;
			}

		default:
			{
				isMatch = (*iterPattern == *iterText);
				break;
			}
		}

		if (isMatch) {
			isPreviousWildcard = false;
			iterPattern++;
			iterText++;
		}
		else if (isWildcard) {
			isPreviousWildcard = true;
			iterPattern++;
		}
		else if (isPreviousWildcard) {
			iterText++;
		}
		else if (iterLastWildcard != pattern.end()) {
			iterPattern = iterLastWildcard;
		}
		else {
			iterText = text.end();
		}
	}

	return iterPattern == pattern.end();
}

enum ReportCode {
	TestPass, TestFail, PrintReport
};

void test_report(ReportCode code)
{
	static int nTests = 0;
	static int nPasses = 0;
	static int nFails = 0;

	switch (code) {
	case TestPass:
		{
			nTests += 1;
			nPasses += 1;
			break;
		}
	case TestFail:
		{
			nTests += 1;
			nFails += 1;
			break;
		}
	case PrintReport:
		{
			std::cout << nTests << " tests: "
				<< nPasses << " passed, "
				<< nFails << " failed.\n";
			break;
		}
	default:
		{
			std::cout << "Unknown report code: " << code << "\n";
			break;
		}
	}
}

void test_glob(std::string_view pattern, std::string_view text, bool expected = true)
{
	bool result = glob(pattern, text);
	test_report(result == expected ? TestPass : TestFail);

	std::cout
		<< "\t" << pattern
		<< "\t" << text
		<< "\t" << result
		<< "\t" << (result == expected ? "pass" : "fail")
		<< "\n";
}

int main()
{
	using namespace std::literals::string_view_literals;
	std::cout << std::boolalpha;

	{
		std::cout << 1;
		auto pattern = "lamb"sv;
		auto text    = "lamby"sv;
		test_glob(pattern, text);
	}
	{
		std::cout << 2;
		auto pattern = "thing"sv;
		auto text    = "otherthing"sv;
		test_glob(pattern, text, false);
	}
	{
		std::cout << 3;
		auto pattern = "*"sv;
		auto text    = "anything"sv;
		test_glob(pattern, text);
	}
	{
		std::cout << 4;
		auto pattern = "main.*"sv;
		auto text    = "main.css"sv;
		test_glob(pattern, text);
	}
	{
		std::cout << 5;
		auto pattern = "**"sv;
		auto text    = "mightbreak"sv;
		test_glob(pattern, text);
	}
	{
		std::cout << 6;
		auto pattern = "*.txt"sv;
		auto text    = "apple.tx.txt"sv;
		test_glob(pattern, text);
	}
	{
		std::cout << 7;
		auto pattern = "?at"sv;
		auto text    = "cat"sv;
		test_glob(pattern, text);
	}
	{
		std::cout << 8;
		auto pattern = "?at"sv;
		auto text    = "at"sv;
		test_glob(pattern, text, false);
	}
	{
		std::cout << 9;
		auto pattern = "ye?"sv;
		auto text    = "ye"sv;
		test_glob(pattern, text, false);
	}
	{
		std::cout << "10";
		auto pattern = "[abc]"sv;
		auto text    = "a"sv;
		test_glob(pattern, text);
	}
	{
		std::cout << "11";
		auto pattern = "[!aeiou]"sv;
		auto text    = "b"sv;
		test_glob(pattern, text);
	}
	{
		std::cout << "12";
		auto pattern = "[!aeiou]"sv;
		auto text    = "e"sv;
		test_glob(pattern, text, false);
	}
	{
		std::cout << "13";
		auto pattern = "[][!]"sv;
		auto text    = "]"sv;
		test_glob(pattern, text);
	}
	{
		std::cout << "14";
		auto pattern = "[][!]"sv;
		auto text    = "["sv;
		test_glob(pattern, text);
	}
	{
		std::cout << "15";
		auto pattern = "[][!]"sv;
		auto text    = "!"sv;
		test_glob(pattern, text);
	}

	std::cout << "\n";
	test_report(PrintReport);
}
