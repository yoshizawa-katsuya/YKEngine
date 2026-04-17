#pragma once
#include "BaseModel.h"

namespace YKEngine
{

/// <summary>
/// 3Dオブジェクトのクラス。
/// 3Dオブジェクトはモデルを持ち、ワールド変換行列を持つ。
/// モデルは複数の3Dオブジェクトで共有できる。
/// </summary>
class My3dObject
{
public:

	/// <summary>
	/// コンストラクタ。	
	/// </summary>
	My3dObject();

	/// <summary>
	/// デストラクタ。
	/// </summary>
	virtual ~My3dObject() = default;

	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="model">モデル</param>
	virtual void Initialize(BaseModel* model);

	/// <summary>
	/// ワールド変換行列の更新。
	/// </summary>
	/// <param name="worldTransform">ワールド変換行列</param>
	virtual void WorldTransformUpdate(const WorldTransform& worldTransform);

	/// <summary>
	/// アニメーションの更新。
	/// </summary>
	/// <param name="animation">アニメーション</param>
	virtual void AnimationUpdate(Animation* animation);

	/// <summary>
	/// カメラの更新。
	/// </summary>
	/// <param name="camera">カメラ</param>
	virtual void CameraUpdate(Camera* camera);

	/// <summary>
	/// 描画。
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// 描画（テクスチャハンドル指定）。
	/// </summary>
	/// <param name="textureHandle">テクスチャ</param>
	virtual void Draw(uint32_t textureHandle);

	/// <summary>
	/// UV変換の設定。
	/// マテリアルデータを生成する。
	/// </summary>
	/// <param name = "scale">スケール< / param>
	/// <param name="rotate">回転</param>
	/// <param name="translate">平行移動</param>
	void SetUVTransform(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

	/// <summary>
	/// UV変換の設定。
	/// マテリアルデータを生成する。
	/// </summary>
	/// <param name="uvTransform">UV変換</param>
	void SetUVTransform(const EulerTransform& uvTransform);

	/// <summary>
	/// ライティングの有効・無効設定。
	/// マテリアルデータを生成する。
	/// </summary>
	/// <param name="enableLighting">ライティングの有効・無効</param>
	void SetEnableLighting(bool enableLighting);

	/// <summary>
	/// 色の設定。
	/// マテリアルデータを生成する。
	/// </summary>
	/// <param name="color">色</param>
	void SetColor(const Vector4& color);

	/// <summary>
	/// 環境光係数の設定。
	/// マテリアルデータを生成する。
	/// </summary>
	/// <param name="coefficient">環境光係数</param>
	void SetEnviromentCoefficient(float coefficient);

	const BaseModel& GetModel() const { return *model_; }

protected:

	/// <summary>
	///	マテリアルデータの作成。
	/// </summary>
	void CreateMaterialData();


	/// <summary>
	/// Transformation用のCBufferの場所を設定.
	/// </summary>
	void SetTransformationBufferView();

	/// <summary>
	/// マテリアル用のCBufferの場所を設定.
	/// </summary>
	virtual void SetMaterialBufferView();

	DirectXCommon* dxCommon_;

	//TransformationMatrix用のリソースを作る。モデル用
	Microsoft::WRL::ComPtr<ID3D12Resource> TransformationResource_;
	//データを書き込む
	TransformationMatrix* TransformationData_;

	//マテリアル用のリソースを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_;
	//マテリアルにデータを書き込む
	Material* materialData_ = nullptr;

	BaseModel* model_;

};

} // namespace YKEngine