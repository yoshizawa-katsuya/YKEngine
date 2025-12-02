#pragma once
#include <cstdint>
#include <vector>
#include <list>
#include <string>
#include <xaudio2.h>
#include <optional>
#include <map>
#include <array>
#include <wrl.h>
#include <span>
#include <Windows.h>
#include <d3d12.h>
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"
#include "Color.h"

/// <summary>
/// 頂点データ構造体。
/// </summary>
/// <param name="position">座標</param>
/// <param name="texcoord">テクスチャ座標</param>
/// <param name="normal">法線</param>
struct VertexData 
{
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};

/// <summary>
/// 3x3行列
/// </summary>
struct Matrix3x3
{
	float m[3][3];
};

/// <summary>
/// 4x4行列
/// </summary>
struct Matrix4x4
{
	float m[4][4];
};

/// <summary>
/// 円
/// </summary>
struct Circle
{
	Vector2 center;
	float radius;
};

/// <summary>
/// 正方形
/// </summary>
struct Square 
{
	Vector2 min;
	Vector2 max;
};

/// <summary>
/// 球
/// </summary>
struct Sphere 
{
	Vector3 center;
	float radius;
};

/// <summary>
/// 直線
/// </summary>
struct Line 
{
	Vector3 origin;
	Vector3 diff;
};

/// <summary>
/// 半直線
/// </summary>
struct Ray 
{
	Vector3 origin;
	Vector3 diff;
};

/// <summary>
/// 線分
/// </summary>
struct Segment
{
	Vector3 origin;
	Vector3 diff;
};

/// <summary>
/// 平面
/// </summary>
struct Plane 
{
	Vector3 normal;	//法線
	float distance;	//距離
};

/// <summary>
/// 三角形
/// </summary>
struct Triangle
{
	Vector3 vertices[3];	//頂点
};

/// <summary>
/// 軸平行境界ボックス
/// </summary>
struct AABB 
{
	Vector3 min;
	Vector3 max;
};

/// <summary>
/// 方向付き境界ボックス
/// </summary>
struct OBB 
{
	Vector3 center;	//中心点
	Vector3 orientations[3];	//座標軸。正規化・直交必須
	Vector3 size;	//座標軸方向の長さの半分。中心から面までの距離
};

/// <summary>
/// マテリアル
/// </summary>
/// <param name="color">色</param>
/// <param name="enableLighting">ライティングを有効にするか</param>
/// <param name="shininess">光沢度</param>
/// <param name="enviromentCoefficient">環境光の係数</param>
/// <param name="padding">パディング</param>
/// <param name="uvTransform">UV変換行列</param>
struct Material 
{
	Vector4 color;
	bool enableLighting;
	float shininess;				//光沢度
	float enviromentCoefficient;	//環境光の係数
	//変数を新しく追加する場合paddingを忘れないように
	float padding;
	Matrix4x4 uvTransform;
};

/// <summary>
/// 変換行列
/// </summary>
/// <param name="WVP">ワールドビュー射影行列</param>
/// <param name="World">ワールド行列</param>
/// <param name="WorldInverseTranspose">ワールド逆転置行列</param>
struct TransformationMatrix
{
	Matrix4x4 WVP;
	Matrix4x4 World;
	Matrix4x4 WorldInverseTranspose;
};

/// <summary>
/// ボール
/// </summary>
/// <param name="pos">位置</param>
/// <param name="velocity">速度</param>
/// <param name="acceleration">加速度</param>
/// <param name="mass">質量</param>
/// <param name="radius">半径</param>
/// <param name="color">色</param>
struct Ball
{

	Vector2 pos;
	Vector2 velocity;
	Vector2 acceleration;
	float mass;
	float radius;
	unsigned int color;

};

/// <summary>
/// 箱
/// </summary>
/// <param name="pos">位置</param>
/// <param name="size">大きさ</param>
/// <param name="valocity">速度</param>
/// <param name="acceleration">加速度</param>
/// <param name="mass">質量</param>
/// <param name="color">色</param>
struct Box
{
	Vector2 pos;
	Vector2 size;
	Vector2 valocity;
	Vector2 acceleration;
	float mass;
	unsigned int color;
};

/// <summary>
/// オイラー変換
/// </summary>
/// <param name="scale">スケール</param>
/// <param name="rotation">回転</param>
/// <param name="translation">平行移動</param>
struct EulerTransform
{
	Vector3 scale;
	Vector3 rotation;
	Vector3 translation;
};

/// <summary>
/// クォータニオン変換
/// </summary>
/// <param name="scale">スケール</param>
/// <param name="rotation">回転</param>
/// <param name="translation">平行移動</param>
struct QuaternionTransform
{
	Vector3 scale;
	Quaternion rotation;
	Vector3 translation;
};

const uint32_t kNumMaxInfluence = 4;
/// <summary>
/// 頂点の影響情報
/// </summary>
/// <param name="weights">影響度</param>
/// <param name="jointIndices">ジョイントのインデックス</param>
struct VertexInfluence
{
	std::array<float, kNumMaxInfluence> weights;
	std::array<int32_t, kNumMaxInfluence> jointIndices;
};

/// <summary>
/// GPU用のジョイント行列
/// </summary>
/// <param name="skeletonSpaceMatrix">スケルトンスペース行列（位置用）</param>
/// <param name="skeletonSpaceInverseTransposeMatrix">スケルトンスペース逆転置行列（法線用）</param>
struct WellForGPU 
{
	Matrix4x4 skeletonSpaceMatrix;	//位置用
	Matrix4x4 skeletonSpaceInverseTransposeMatrix;	//法線用
};

/// <summary>
/// スキンクラスター
/// </summary>
/// <param name="inverseBindPoseMatrices">逆バインドポーズ行列群</param>
/// <param name="influenceResource">頂点影響リソース</param>
/// <param name="influenceBufferView">頂点影響バッファビュー</param>
/// <param name="mappedInfluence">マップ済み頂点影響</param>
/// <param name="paletteResource">ジョイントパレットリソース</param>
/// param name="mappedPalette">マップ済みジョイントパレット</param>
/// param name="paletteSrvHandle">ジョイントパレットSRVハンドル</param>
struct SkinCluster 
{
	std::vector<Matrix4x4> inverseBindPoseMatrices;
	Microsoft::WRL::ComPtr<ID3D12Resource> influenceResource;
	D3D12_VERTEX_BUFFER_VIEW influenceBufferView;
	std::span<VertexInfluence> mappedInfluence;
	Microsoft::WRL::ComPtr<ID3D12Resource> paletteResource;
	std::span<WellForGPU> mappedPalette;
	std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE> paletteSrvHandle;
};

/// <summary>
/// パーティクル
/// </summary>
/// <param name="transform">変換情報</param>
/// <param name="velocity">速度</param>
/// <param name="color">色</param>
/// <param name="lifeTime">寿命</param>
/// <param name="currentTime">現在の時間</param>
struct Particle 
{
	EulerTransform transform;
	Vector3 velocity;
	Vector3 rotationVelocity;
	Color color;
	float lifeTime;
	float currentTime;
};

/// <summary>
/// GPU用パーティクル構造体。
/// </summary>
/// <param name="WVP">ワールドビュー射影行列</param>
/// <param name="World">ワールド行列</param>
/// param name="color">色</param>
struct ParticleForGPU 
{
	Matrix4x4 WVP;
	Matrix4x4 World;
	Color color;
};

/// <summary>
/// 加速度フィールド
/// </summary>
/// <param name="accerelation">加速度</param>
/// param name="area">範囲</param>
struct AccelerationField 
{
	Vector3 accerelation;	//加速度
	AABB area;	//範囲
};

/// <summary>
/// チャンクヘッダ
/// </summary>
/// <param name="id">チャンクごとのID</param>
/// param name="size">チャンクサイズ</param>
struct ChunkHeader
{
	char id[4];	//チャンクごとのID
	int32_t size;	//チャンクサイズ
};

/// <summary>
/// RIFFヘッダ
/// </summary>
/// <param name="chunk">チャンクヘッダ</param>
/// param name="type">タイプ</param>
struct RiffHeader
{
	ChunkHeader chunk;	//"RIFF"
	char type[4];	//"WAVE"
};

/// <summary>
/// フォーマットチャンク
/// </summary>
/// <param name="chunk">チャンクヘッダ</param>
/// param name="fmt">波形フォーマット</param>
struct FormatChunk
{
	ChunkHeader chunk;	//"fmt"
	WAVEFORMATEX fmt;	//波形フォーマット
};

/// <summary>
/// 音声データ
/// </summary>
/// <param name="wfex">波形フォーマット</param>
/// param name="pBuffer">バッファ</param>
/// param name="bufferSize">バッファのサイズ</param>
/// param name="pcmData">PCMデータ</param>
struct SoundData
{
	//波形フォーマット
	WAVEFORMATEX wfex;
	//バッファ
	std::vector<BYTE> buffer;

	std::vector<BYTE> pcmData;
};

/// <summary>
/// ループ音声データ
/// </summary>
/// <param name="soundData">音声データ</param>
/// param name="pSourceVoice">ソースボイス</param>
struct LoopSoundData
{
	SoundData soundData;

	IXAudio2SourceVoice* pSourceVoice = nullptr;
};

template <typename tValue>
/// <summary>
/// キーフレーム
/// </summary>
/// <param name="time">時間</param>
/// <param name="value">値</param>
struct  KeyFrame
{
	float time;
	tValue value;
};
using KeyframeVector3 = KeyFrame<Vector3>;
using KeyframeQuaternion = KeyFrame<Quaternion>;

template <typename tValue>
/// <summary>
/// アニメーションカーブ
/// </summary>
/// <param name="keyframes">キーフレーム群</param>
struct AnimationCurve 
{
	std::vector<KeyFrame<tValue>> keyframes;
};

/// <summary>
/// ノードアニメーション
/// </summary>
/// <param name="translate">平行移動アニメーションカーブ</param>
/// param name="rotate">回転アニメーションカーブ</param>
/// param name="scale">スケールアニメーションカーブ</param>
struct NodeAnimation 
{
	AnimationCurve<Vector3> translate;
	AnimationCurve<Quaternion> rotate;
	AnimationCurve<Vector3> scale;
};

/// <summary>
/// ジョイント
/// </summary>
/// <param name="transform">transform情報</param>
/// param name="localMatrix">localMatrix</param>
/// param name="skeletonSpaceMatrix">skeletonSpaceでの変換行列</param>
/// param name="name">名前</param>
/// param name="children">子JointのIndexのリスト。いなければ空</param>
/// param name="index">自身のIndex</param>
/// param name="parent">親JointのIndex。いなければnull</param>
struct Joint 
{
	QuaternionTransform transform;	//transform情報
	Matrix4x4 localMatrix;	//localMatrix
	Matrix4x4 skeletonSpaceMatrix;	//skeletonSpaceでの変換行列
	std::string name;	//名前
	std::vector<int32_t> children;	//子JointのIndexのリスト。いなければ空
	int32_t index;	//自身のIndex
	std::optional<int32_t> parent;	//親JointのIndex。いなければnull
};

/// <summary>
/// 骨格
/// </summary>
/// <param name="root">RootJointのIndex</param>
/// param name="jointMap">Joint名とIndexとの辞書</param>
/// param name="joints">所属しているジョイント</param>
struct Skeleton
{
	int32_t root;	//RootJointのIndex
	std::map<std::string, int32_t> jointMap;	//Joint名とIndexとの辞書
	std::vector<Joint> joints;	//所属しているジョイント
};
