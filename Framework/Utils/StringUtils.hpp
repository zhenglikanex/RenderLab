#pragma once

#include <string>
#include <stdio.h>
#include <locale>
#include <codecvt>
#include <vector>


namespace StringUtils
{
	using F = std::codecvt_byname<wchar_t, char, std::mbstate_t>;

	inline std::wstring String2WString(const std::string& str, const std::string& locale = "")
	{
		std::wstring_convert<F> strcvt(new F(locale));
		return strcvt.from_bytes(str);
	}

	inline std::string WString2String(const std::wstring& str, const std::string& locale = "")
	{
		std::wstring_convert<F> strcvt(new F(locale));
		return strcvt.to_bytes(str);
	}

	inline std::string WSting2UTF8(const std::wstring& str)
	{
		// strcvt中的两个空字符串参数作用是在转换错误时返回空字符串
		// 如果不设置，当传入参数为非UTF-8中文时，会引起程序错误
		std::wstring_convert<std::codecvt_utf8<wchar_t>> strcvt(std::string(""), std::wstring(L""));
		return strcvt.to_bytes(str);
	}

	inline std::wstring UTF82WString(const std::string& str)
	{
		std::wstring_convert< std::codecvt_utf8<wchar_t>> strcvt(std::string(""), std::wstring(L""));
		return strcvt.from_bytes(str);
	}

	inline std::vector<std::string> split(const std::string& str, char ch)
	{
		std::vector<std::string> tokens;

		size_t beg = 0;
		while (beg < str.size())
		{
			auto end = str.find_first_of(ch, beg);
			if (end != std::string::npos)
			{
				tokens.emplace_back(str.substr(beg, end - beg));
				beg = end + 1;
			}
			else
			{
				tokens.emplace_back(str.substr(beg, str.size() - beg));
				break;
			}
		}

		return tokens;
	}

	inline void RemoveCh(std::string& str, char ch)
	{
		auto pos = str.find(ch);
		while (pos != std::string::npos)
		{
			str = str.erase(pos, 1);
			pos = str.find(ch);
		}
	}
};
