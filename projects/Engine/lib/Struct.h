#pragma once
#include <cstdint>
#include <vector>
#include <list>
#include <string>
#include <xaudio2.h>
#include <optional>
#include <map>

//static const int kRowHeight = 20;
//static const int kColumnWidth = 60;

struct intVector2 {
	int x;
	int y;
};

struct Vector2 {
	float x;
	float y;
};

struct Vector3{
	float x;
	float y;
	float z;

	Vector3& operator*=(float s) { x *= s;  y *= s; z *= s; return *this; }
	Vector3& operator-=(const Vector3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
	Vector3& operator+=(const Vector3& v) { x += v.x; y += v.y; z += v.z; return *this; }
	Vector3& operator/=(float s) { x /= s;  y /= s; z /= s; return *this; }

};

struct Vector4 {
	float x;
	float y;
	float z;
	float w;
};

struct  Quaternion
{
	float x;
	float y;
	float z;
	float w;
};

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

struct AABB {
	Vector3 min;
	Vector3 max;
};

struct Material {
	Vector4 color;
	int32_t enableLighting;
	float padding[3];
	Matrix4x4 uvTransform;
};

struct TransformationMatrix {
	Matrix4x4 WVP;
	Matrix4x4 World;
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

struct EulerTransform
{
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

struct QuaternionTransform
{
	Vector3 scale;
	Quaternion rotate;
	Vector3 translate;
};

struct MaterialData
{
	std::string textureFilePath;
};

struct Node {
	QuaternionTransform transform;
	Matrix4x4 localMatrix;
	std::string name;
	std::vector<Node> children;
};

struct ModelData {
	std::vector<VertexData> vertices;
	MaterialData material;
	Node rootNode;
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
	Matrix4x4 skeltonSpaceMatrix;	//skeletonSpaceでの変換行列
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
