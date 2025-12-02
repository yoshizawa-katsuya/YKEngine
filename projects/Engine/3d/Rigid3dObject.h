#pragma once
#include "Base3dObject.h"

/// <summary>
/// リジッドモデルを扱うクラス。
/// Base3dObjectを継承している。
/// スキンメッシュを必要としない3Dモデルを扱う。
/// </summary>
class Rigid3dObject : public Base3dObject
{
public:

	/// <summary>
	/// デストラクタ。
	/// </summary>
	~Rigid3dObject() override;

private:


};

