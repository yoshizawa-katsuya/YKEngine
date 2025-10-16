#pragma once

/// <summary>
/// リソースリークチェッカー。
/// Direct3Dのリソースが解放されずに残っている場合、デストラクタでアサートを発生させる。
/// </summary>
class D3DResourceLeakChecker
{
public:
	~D3DResourceLeakChecker();

};