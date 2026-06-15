#pragma once

namespace YKEngine
{

/// <summary>
/// リソースリークチェッカー。
/// Direct3Dのリソースが解放されずに残っている場合、デストラクタでアサートを発生させる。
/// </summary>
class D3DResourceLeakChecker
{
public:
	/// <summary>
	/// デストラクタ。
	/// リソースが解放されていなければアサートを発生させる。
	/// </summary>
	~D3DResourceLeakChecker();

};

} // namespace YKEngine