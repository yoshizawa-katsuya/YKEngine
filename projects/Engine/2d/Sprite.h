#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <string>
#include <wrl.h>
#include "Struct.h"
#include "TextureManager.h"

namespace YKEngine
{
class SpritePlatform;

/// <summary>
/// 2D画像を表示するためのクラス。
/// テクスチャを貼り付けた四角形を表示する。
/// 四角形の大きさ、位置、回転、色、テクスチャの切り出し範囲などを設定できる。
/// </summary>
class Sprite
{
public:

	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(uint32_t textureHandle);

	/// <summary>
	///	描画。
	/// </summary>
	void Draw();

	Vector2 GetSize() const { return size_; }

	Vector2 GetPosition() const { return position_; }

	float GetRotation() const { return rotation_; }

	const Vector4& GetColor() const { return materialData_->color; }

	Vector2 GetAnchorPoint() const { return anchorPoint_; }

	bool GetIsFlipX() const { return isFlipX_; }

	bool GetIsFlipY() const { return isFlipY_; }

	Vector2 GetTextureLeftTop() const { return textureLeftTop_; }

	Vector2 GetTextureSize() const { return textureSize_; }

	const EulerTransform& GetTransform() const { return transform_; }

	const EulerTransform& GetUVTransform() const { return uvTransform_; }

	uint32_t GetTeture() { return textureHandle_; }

	void SetSize(Vector2 size) { size_ = size; }

	void SetPosition(Vector2 position) { position_ = position; }

	void SetRotation(float rotation) { rotation_ = rotation; }

	void SetColor(const Vector4& color) { materialData_->color = color; }

	void SetAlpha(float alpha) { materialData_->color.w = alpha; }

	void SetAnchorPoint(Vector2 anchorPoint) { anchorPoint_ = anchorPoint; }

	void SetIsFlipX(bool IsFlipX) { isFlipX_ = IsFlipX; }

	void SetIsFlipY(bool IsFlipY) { isFlipY_ = IsFlipY; }

	void SetTextureLeftTop(Vector2 textureLeftTop) { textureLeftTop_ = textureLeftTop; }

	void SetTextureSize(Vector2 textureSize) { textureSize_ = textureSize; }

	void SetTexture(uint32_t textureHandle) { textureHandle_ = textureHandle; }

	// マスクテクスチャの設定
	void SetMaskTexture(uint32_t maskTextureHandle) { maskTextureHandle_ = maskTextureHandle; }

	void SetUVTransform(const EulerTransform& uvTransform) { uvTransform_ = uvTransform; }

	void SetUVTranslate(const Vector3& translate) { uvTransform_.translation = translate; }

	// progress
	void SetProgress(float progress) { materialData_->progress = progress; }

protected:

	/// <summary>
	/// 頂点データ作成。
	/// </summary>
	void CreateVertexData();

	/// <summary>
	/// マテリアルデータ作成。
	/// </summary>
	void CreateMaterialData();

	/// <summary>
	/// 変換行列データ作成。
	/// </summary>
	void CreateTransformData();

	/// <summary>
	/// スプライトサイズを画像サイズに合わせて調整。
	/// </summary>
	void AdjustTextureSize();

	SpritePlatform* spritePlatform_ = nullptr;

	//頂点バッファビューを作成する
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	VertexData* vertexData_ = nullptr;

	//IBV
	//Sprite用のIndexResource
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource_;
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};
	uint32_t* indexData_ = nullptr;

	//Sprite用のマテリアルリソースを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_;

	//マテリアルにデータを書き込む
	Material* materialData_ = nullptr;

	//Sprite用のTransformationMatrix用のリソースを作る。Matix4x4 1つ分のサイズを用意する
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixResource_;

	//データを書き込む
	TransformationMatrix* transformationMatrixData_ = nullptr;

	//Sprite用のTransform変数
	EulerTransform transform_{ {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f ,0.0f, 0.0f} };
	EulerTransform uvTransform_{
		{1.0f, 1.0f, 1.0f},
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
	};

	uint32_t textureHandle_ = 1;

	// マスクテクスチャ
	uint32_t maskTextureHandle_ = UINT32_MAX;

	Vector2 size_ = { 1.0f, 1.0f };
	Vector2 position_ = { 0.0f, 0.0f };
	float rotation_ = 0.0f;
	Vector2 anchorPoint_ = { 0.0f, 0.0f };

	//左右フリップ
	bool isFlipX_ = false;
	//上下フリップ
	bool isFlipY_ = false;

	//テクスチャ左上座標
	Vector2 textureLeftTop_ = { 0.0f, 0.0f };
	//テクスチャ切り出しサイズ
	Vector2 textureSize_ = { 100.0f, 100.0f };

};

} // namespace YKEngine