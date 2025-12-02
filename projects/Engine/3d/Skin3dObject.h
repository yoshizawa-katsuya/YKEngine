#pragma once
#include "Base3dObject.h"

/// <summary>
/// スキンモデルを扱うクラス。
/// Base3dObjectを継承している。
/// スキンメッシュを持つモデルを描画するために使用される。
/// スキニングアニメーションをサポートしており、ボーンとジョイントの管理も行う。
/// ボーンとジョイントのデバッグ描画機能も備えている。
/// </summary>
class Skin3dObject : public Base3dObject
{
public:

	/// <summary>
	/// デストラクタ。
	/// </summary>
	~Skin3dObject();

	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="model">ベースモデル。</param>
	void Initialize(BaseModel* model) override;

	/// <summary>
	/// アニメーションの適用と更新。
	/// </summary>
	/// <param name="animation">アニメーション</param>
	void AnimationUpdate(Animation* animation) override;

	/// <summary>
	/// 描画。
	/// </summary>
	void Draw() override;

	/// <summary>
	/// テクスチャハンドルを指定して描画。
	/// </summary>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Draw(uint32_t textureHandle) override;

	/// <summary>
	/// ボーンのデバッグ描画。
	/// </summary>
	/// <param name="transform">ワールド変換情報</param>
	/// <param name="camera">カメラ</param>
	void BoneDraw(const EulerTransform& transform, Camera* camera);

	/// <summary>
	/// ジョイントのデバッグ描画。
	/// </summary>
	/// <param name="transform">ワールド変換情報</param>
	/// <param name="camera">カメラ</param>
	void JointDraw(const EulerTransform& transform, Camera* camera);

private:

	/// <summary>
	/// スケルトンの作成。
	/// </summary>
	void CreateSkelton();

	/// <summary>
	/// ジョイントの作成。
	/// </summary>
	/// <param name="node">ノード</param>
	/// <param name="parent">親ジョイントのインデックス</param>
	int32_t CreateJoint(const Node& node, const std::optional<int32_t>& parent);

	/// <summary>
	/// スキンクラスターの作成。
	/// </summary>
	void CreateSkinCluster();

	/// <summary>
	/// アニメーションの適用。
	/// </summary>
	/// <param name="animation">アニメーション</param>
	void ApplyAnimation(Animation* animation);

	/// <summary>
	/// スケルトンの更新。
	/// </summary>
	void SkeletonUpdate();

	/// <summary>
	/// スキンクラスターの更新。
	/// </summary>
	void SkinClusterUpdate();

	/// <summary>
	/// 描画処理共通部分。
	/// </summary>
	void DrawCommonProcess();

	/// <summary>
	/// マテリアル用のCBufferの場所を設定.
	/// </summary>
	void SetMaterialBufferView() override;

	Skeleton skeleton_;

	SkinCluster skinCluster_;

	uint32_t srvIndex_;

};

