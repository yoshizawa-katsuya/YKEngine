#pragma once
#include "DirectXCommon.h"
#include "PrimitiveDrawer.h"
#include "DirectionalLight.h"
#include <array>
class Camera;

class ModelPlatform
{
public:

	//シングルトンインスタンスの取得
	static ModelPlatform* GetInstance();

	//終了
	void Finalize();

	//初期化
	void Initialize(DirectXCommon* dxCommon, PrimitiveDrawer* primitiveDrawer);

	//共通描画設定
	void PreDraw();

	void LinePreDraw();

	void LineDraw(const Matrix4x4& worldMatrix1, const Matrix4x4& worldMatrix2, Camera* camera);

	void SpherePreDraw();

	void SphereDraw(const Matrix4x4& worldMatrix, Camera* camera);

	DirectXCommon* GetDxCommon() const { return dxCommon_; }

	void SetDirectionalLight(DirectionalLight* directionalLight) { directionalLight_ = directionalLight; }

private:

	struct LineWVP
	{
		Matrix4x4 WVP1;
		Matrix4x4 WVP2;
	};

	static ModelPlatform* instance_;

	ModelPlatform() = default;
	~ModelPlatform() = default;
	ModelPlatform(ModelPlatform&) = default;
	ModelPlatform& operator=(ModelPlatform&) = default;

	DirectXCommon* dxCommon_;

	PrimitiveDrawer* primitiveDrawer_;

	DirectionalLight* directionalLight_;

	//VertexResourceを生成
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_;

	Vector4* vertexData_ = nullptr;
	//頂点バッファビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	static const uint32_t resourceNum_ = 128;

	//TransformationMatrix用のリソースを作る 線分用
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, resourceNum_> LineWVPResources_;
	//Microsoft::WRL::ComPtr<ID3D12Resource> LineWVPResource_;
	//データを書き込む
	std::array<LineWVP*, resourceNum_> LineWVPDatas_;
	//LineWVP* LineWVPData_ = nullptr;

	uint32_t lineIndex_ = 0;

	//TransformationMatrix用のリソースを作る
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, resourceNum_> WVPResources_;
	//Microsoft::WRL::ComPtr<ID3D12Resource> WVPResource_;
	//データを書き込む
	std::array<Matrix4x4*, resourceNum_> WVPDatas_;
	//Matrix4x4* WVPData_ = nullptr;

	uint32_t sphereIndex_ = 0;

};

