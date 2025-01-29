#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <string>
#include <wrl.h>
#include "Struct.h"
#include "TextureManager.h"
class SpritePlatform;

class Sprite
{
public:

	//初期化
	void Initialize(uint32_t textureHandle);

	//描画
	void Draw();

	const Vector2& GetSize() const { return size_; }
	Vector2& GetSize() { return size_; }

	Vector2& GetPosition() { return position_; }
	const Vector2& GetPosition() const { return position_; }

	float& GetRotation() { return rotation_; }
	float GetRotation() const { return rotation_; }

	Vector4& GetColor() { return materialData_->color; }
	const Vector4& GetColor() const { return materialData_->color; }

	Vector2& GetAnchorPoint() { return anchorPoint_; }
	const Vector2& GetAnchorPoint() const { return anchorPoint_; }

	bool& GetIsFlipX() { return isFlipX_; }
	bool GetIsFlipX() const { return isFlipX_; }

	bool& GetIsFlipY() { return isFlipY_; }
	bool GetIsFlipY() const { return isFlipY_; }

	Vector2& GetTextureLeftTop() { return textureLeftTop_; }
	const Vector2& GetTextureLeftTop() const { return textureLeftTop_; }

	Vector2& GetTextureSize() { return textureSize_; }
	const Vector2& GetTextureSize() const { return textureSize_; }

	EulerTransform GetTransform() { return transform_; }
	EulerTransform GetUVTransform() { return uvTransform_; }

	EulerTransform& GetTransformAddress() { return transform_; }
	EulerTransform& GetUVTransformAddress() { return uvTransform_; }

	void SetSize(const Vector2& size) { size_ = size; }

	void SetPosition(const Vector2& position) { position_ = position; }

	void SetRotation(float rotation) { rotation_ = rotation; }

	void SetColor(const Vector4& color) { materialData_->color = color; }

	void SetAnchorPoint(const Vector2& anchorPoint) { anchorPoint_ = anchorPoint; }

	void SetIsFlipX(bool IsFlipX) { isFlipX_ = IsFlipX; }

	void SetIsFlipY(bool IsFlipY) { isFlipY_ = IsFlipY; }

	void SetTextureLeftTop(const Vector2& textureLeftTop) { textureLeftTop_ = textureLeftTop; }

	void SetTextureSize(const Vector2& textureSize) { textureSize_ = textureSize; }

	void SetTexture(uint32_t textureHandle) { textureHandle_ = textureHandle; }

	uint32_t GetTeture() { return textureHandle_; }

private:

	//頂点データ作成
	void CreateVertexData();

	//マテリアルデータ作成
	void CreateMaterialData();

	//座標行列変換データ作成
	void CreateTransformData();

	//テクスチャサイズをイメージに合わせる
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

