#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <wrl.h>
#include <string>



/// <summary>
/// ログ出力ユーティリティ
/// </summary>
namespace Logger
{
	void Log(const std::string& message);
}

/// <summary>
/// 文字列変換ユーティリティ
/// </summary>
namespace StringUtility 
{
	//wstringに変換
	std::wstring ConvertString(const std::string& str);

	//stringに変換
	std::string ConvertString(const std::wstring& str);
}
