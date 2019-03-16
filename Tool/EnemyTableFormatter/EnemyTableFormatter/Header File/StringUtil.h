#pragma once

#include <string>

namespace StringUtil
{
	void SkipLine(const char *&text);
	void SkipWhiteSpace(const char *&text);
	std::string GetWord(const char *&text, char delimiter);
	std::string GetWord(const char *&text, char delimiter1, char delimiter2);
	std::string GetWord(const std::string &str, char delimiter);
}
