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
	/// <summary>
	/// ログ出力。
	/// </summary>
	/// <param name="message">ログメッセージ</param>
	void Log(const std::string& message);
}

/// <summary>
/// 文字列変換ユーティリティ
/// </summary>
namespace StringUtility 
{
	/// <summary>
	/// wstringに変換。
	/// </summary>
	/// <param name="str">変換するstring</param>
	/// <returns>変換後のwstring</returns>
	std::wstring ConvertString(const std::string& str);

	/// <summary>
	/// stringに変換。
	/// </summary>
	/// <param name="str">変換するwstring</param>
	/// <returns>変換後のstring</returns>
	std::string ConvertString(const std::wstring& str);
}
