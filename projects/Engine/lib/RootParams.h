#pragma once

namespace YKEngine
{

//ポストエフェクト用ルートパラメータ
enum class PostEffectRootParam
{
	kTexture = 0,
	kCount,
};

//ランダムテクスチャ用ルートパラメータ
enum class RandomRootParam
{
	kTexture = 0,
	kMaterial = 1,
	kCount,
};

//深度輪郭線用ルートパラメータ
enum class DepthOutlineRootParam
{
	kTexture = 0,
	kDepthTexture = 1,
	kMaterial = 2,
	kCount,
};

//ディゾルブ用ルートパラメータ
enum class DissolveRootParam
{
	kTexture = 0,
	kMaskTexture = 1,
	kCount,
};

//デバッグ球用ルートパラメータ
enum class DebugSphereRootParam
{
	kWVP = 0,
	kCount,
};

//デバッグ線用ルートパラメータ
enum class DebudLineRootParam
{
	kWVP = 0,
	kCount,
};

//スカイボックス用ルートパラメータ
enum class SkyBoxRootParam
{
	kMaterial = 0,
	kTransformationMatrix = 1,
	kTexture = 2,
	kCount,
};

//スキンモデル用ルートパラメータ
enum class SkinModelRootParam
{
	kMaterial = 0,
	kTransformationMatrix = 1,
	kTexture = 2,
	kDirectionalLight = 3,
	kCamera = 4,
	kPointLight = 5,
	kSpotLight = 6,
	kLightCount = 7,
	kEnvironmentMap = 8,
	kWell = 9,
	kCount,
};

//モデル用ルートパラメータ。インスタンシングにも使用。
enum class ModelRootParam
{
	kMaterial = 0,
	kTransformationMatrix = 1,
	kTexture = 2,
	kDirectionalLight = 3,
	kCamera = 4,
	kPointLight = 5,
	kSpotLight = 6,
	kLightCount = 7,
	kEnvironmentMap = 8,
	kCount,
};

enum class SpriteRootParam
{
	kMaterial = 0,
	kTransformationMatrix = 1,
	kTexture = 2,
	kMaskTexture = 3,
	kCount,
};

enum class ParticleRootParam
{
	kMaterial = 0,
	kParticleForGPU = 1,
	kTexture = 2,
	kCount,
};

} // namespace YKEngine