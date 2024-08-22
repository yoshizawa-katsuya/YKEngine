#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <wrl.h>
#include <string>



//ログ
namespace Logger
{
	void Log(const std::string& message);
}

//文字コードユーティリティ
namespace StringUtility 
{
	//wstringに変換
	std::wstring ConvertString(const std::string& str);

	//stringに変換
	std::string ConvertString(const std::wstring& str);
}
