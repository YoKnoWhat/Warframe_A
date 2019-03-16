
#include "StringUtil.h"


namespace StringUtil
{
	void SkipLine(const char *&text)
	{
		while (*text != '\n' && *text != '\0' && *text != '\r')
		{
			++text;
		}
		while (*text == '\n' || *text == '\r')
			++text;
	}

	void SkipWhiteSpace(const char *&text)
	{
		while (*text == ' ' || *text == '\f' || *text == '\n' || *text == '\r' || *text == '\t' || *text == '\v')
		{
			++text;
		}
	}

	std::string GetWord(const char *&text, char delimiter)
	{
		const char *wordBegin = text;

		while (*text != delimiter && *text != '\0')
		{
			++text;
		}

		return std::string(wordBegin, text - wordBegin);
	}

	std::string GetWord(const char *&text, char delimiter1, char delimiter2)
	{
		const char *wordBegin = text;

		while (*text != delimiter1 && *text != delimiter2 && *text != '\0')
		{
			++text;
		}

		return std::string(wordBegin, text - wordBegin);
	}

	std::string GetWord(const std::string &str, char delimiter)
	{
		size_t count = 0;

		while (str[count] != delimiter && count < str.size())
		{
			++count;
		}

		return str.substr(0, count);
	}
}
