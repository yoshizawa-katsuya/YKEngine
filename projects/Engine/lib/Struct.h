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

struct VertexData {
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};

struct Matrix3x3
{
	float m[3][3];
};

struct Matrix4x4
{
	float m[4][4];
};

struct Circle
{
	Vector2 center;
	float radius;
};

struct Square {
	Vector2 min;
	Vector2 max;
};

struct Sphere {
	Vector3 center;
	float radius;
};

struct Line {
	Vector3 origin;
	Vector3 diff;
};

struct Ray {
	Vector3 origin;
	Vector3 diff;
};

struct Segment {
	Vector3 origin;
	Vector3 diff;
};

struct Plane {
	Vector3 normal;	//法線
	float distance;	//距離
};

struct Triangle {
	Vector3 vertices[3];	//頂点
};

struct AABB {
	Vector3 min;
	Vector3 max;
};

struct OBB {
	Vector3 center;	//中心点
	Vector3 orientations[3];	//座標軸。正規化・直交必須
	Vector3 size;	//座標軸方向の長さの半分。中心から面までの距離
};

struct Material {
	Vector4 color;
	int32_t enableLighting;
	float padding[3];
	Matrix4x4 uvTransform;
	float shininess;
};

struct TransformationMatrix {
	Matrix4x4 WVP;
	Matrix4x4 World;
	Matrix4x4 WorldInverseTranspose;
};

struct Ball
{

	Vector2 pos;
	Vector2 velocity;
	Vector2 acceleration;
	float mass;
	float radius;
	unsigned int color;

};

struct Box
{
	Vector2 pos;
	Vector2 size;
	Vector2 valocity;
	Vector2 acceleration;
	float mass;
	unsigned int color;
};
//拡縮、回転、移動
struct EulerTransform
{
	Vector3 scale;
	Vector3 rotation;
	Vector3 translation;
};

struct QuaternionTransform
{
	Vector3 scale;
	Quaternion rotation;
	Vector3 translation;
};

const uint32_t kNumMaxInfluence = 4;
struct VertexInfluence {
	std::array<float, kNumMaxInfluence> weights;
	std::array<int32_t, kNumMaxInfluence> jointIndices;
};

struct WellForGPU {
	Matrix4x4 skeletonSpaceMatrix;	//位置用
	Matrix4x4 skeletonSpaceInverseTransposeMatrix;	//法線用
};

struct SkinCluster {
	std::vector<Matrix4x4> inverseBindPoseMatrices;
	Microsoft::WRL::ComPtr<ID3D12Resource> influenceResource;
	D3D12_VERTEX_BUFFER_VIEW influenceBufferView;
	std::span<VertexInfluence> mappedInfluence;
	Microsoft::WRL::ComPtr<ID3D12Resource> paletteResource;
	std::span<WellForGPU> mappedPalette;
	std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE> paletteSrvHandle;
};

struct Particle {
	EulerTransform transform;
	Vector3 velocity;
	Vector4 color;
	float lifeTime;
	float currentTime;
};

struct Emitter {
	EulerTransform transform; //!< エミッタのTransform
	uint32_t count;	//!< 発生数
	float frequency; //!<　発生頻度
	float frequencyTime; //!<頻度用時刻
};

struct ParticleForGPU {
	Matrix4x4 WVP;
	Matrix4x4 World;
	Vector4 color;
};

struct AccelerationField {
	Vector3 accerelation;	//加速度
	AABB area;	//範囲
};

//チャンクヘッダ
struct ChunkHeader
{
	char id[4];	//チャンクごとのID
	int32_t size;	//チャンクサイズ
};

//RIFFヘッダチャンク
struct RiffHeader
{
	ChunkHeader chunk;	//"RIFF"
	char type[4];	//"WAVE"
};


//FMTチャンク
struct FormatChunk
{
	ChunkHeader chunk;	//"fmt"
	WAVEFORMATEX fmt;	//波形フォーマット
};

//音声データ
struct SoundData
{
	//波形フォーマット
	WAVEFORMATEX wfex;
	//バッファの先頭アドレス
	BYTE* pBuffer;
	//バッファのサイズ
	unsigned int bufferSize;
};

//音声データ
struct LoopSoundData
{
	SoundData soundData;

	IXAudio2SourceVoice* pSourceVoice = nullptr;
};

struct ObjectData
{
	std::string fileName;
	EulerTransform transform;
};

struct LevelData
{
	std::list<ObjectData> objects;
};

template <typename tValue>
struct  KeyFrame
{
	float time;
	tValue value;
};
using KeyframeVector3 = KeyFrame<Vector3>;
using KeyframeQuaternion = KeyFrame<Quaternion>;

template <typename tValue>
struct AnimationCurve {
	std::vector<KeyFrame<tValue>> keyframes;
};

struct NodeAnimation {
	AnimationCurve<Vector3> translate;
	AnimationCurve<Quaternion> rotate;
	AnimationCurve<Vector3> scale;
};

struct Joint {
	QuaternionTransform transform;	//transform情報
	Matrix4x4 localMatrix;	//localMatrix
	Matrix4x4 skeletonSpaceMatrix;	//skeletonSpaceでの変換行列
	std::string name;	//名前
	std::vector<int32_t> children;	//子JointのIndexのリスト。いなければ空
	int32_t index;	//自身のIndex
	std::optional<int32_t> parent;	//親JointのIndex。いなければnull
};

struct Skeleton {
	int32_t root;	//RootJointのIndex
	std::map<std::string, int32_t> jointMap;	//Joint名とIndexとの辞書
	std::vector<Joint> joints;	//所属しているジョイント
};
