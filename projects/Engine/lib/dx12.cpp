#include "dx12.h"
#include <cassert>



namespace Logger
{
	void Log(const std::string& message) {
		OutputDebugStringA(message.c_str());
	}
}

namespace StringUtility
{
	std::wstring ConvertString(const std::string& str) {
		// strが空の場合は空のwstringを返す
		if (str.empty()) 
		{
			return std::wstring();
		}
		// 必要なサイズを取得
		auto sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&str[0]), static_cast<int>(str.size()), NULL, 0);
		// サイズが0の場合は空のwstringを返す
		if (sizeNeeded == 0) 
		{
			return std::wstring();
		}
		std::wstring result(sizeNeeded, 0);
		MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&str[0]), static_cast<int>(str.size()), &result[0], sizeNeeded);
		return result;
	}

	std::string ConvertString(const std::wstring& str) {
		// strが空の場合は空のstringを返す
		if (str.empty()) 
		{
			return std::string();
		}
		// 必要なサイズを取得
		auto sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), NULL, 0, NULL, NULL);
		// サイズが0の場合は空のstringを返す
		if (sizeNeeded == 0) 
		{
			return std::string();
		}
		std::string result(sizeNeeded, 0);
		WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), result.data(), sizeNeeded, NULL, NULL);
		return result;
	}
}
