#pragma once
#include <string>

// JSONキー定義
namespace JsonKey
{
	namespace Camera
	{
		const std::string kGroupName = "Camera";
		const std::string kFovY = "FovY";	//カメラの垂直方向の視野角
		const std::string kNearClip = "NearClip";	//カメラの近平面距離
		const std::string kFarClip = "FarClip";	//カメラの遠平面距離
	}
	namespace Model
	{
		const std::string kGroupName = "Model";
		const std::string kEnableLighting = "EnableLighting";	//ライティングの有効化
		const std::string kShininess = "Shininess";	//マテリアルの光沢
		const std::string kEnviromentCoefficient = "EnvironmentCoefficient";	//マテリアルの環境光の係数
	}
}