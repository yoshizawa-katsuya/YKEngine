#pragma once
#include "Audio.h"
#include <unordered_map>

enum class BGMType
{
	kTitle,
	kGame,
	kGameOver,
	kClear,
};

enum class SEType
{
	kShot01,	// プレイヤーのショットSE
	kShot02,	// プレイヤーのチャージショットSE
	kShot03,	// 敵のショットSE
	kDamage01,	// プレイヤーのダメージSE
	kDamage02,	// 敵のダメージSE
	kDeath01,	// 死亡SE
	kDeath02,	// 自機の死亡演出SE
	kStart01,	// プレイヤーのスタートSE
	kStart02,	// 敵のスタートSE
	kAppear01,	// プレイヤーの出現SE
	kAppear02,	// 敵の出現SE
	kAppear03,	// クリアシーンでの出現SE
	kDodge01,	// 回避SE
	kDecision01,	//決定SE
	kMenu01,	//メニューSE
	kCursorMove01,	//カーソル移動SE
};

/// <summary>
/// オーディオ管理クラス。
/// </summary>
class AudioManager
{
public:

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>オーディオ管理クラスのインスタンス</returns>
	static AudioManager* GetInstance();

	/// <summary>
	/// 終了処理。
	/// </summary>
	void Finalize();

	/// <summary>
	/// 初期化。
	/// </summary>
	void Initialize();

	/// <summary>
	/// BGM再生。
	/// </summary>
	/// <param name="bgmType">BGMの種類</param>
	void PlayBGM(BGMType bgmType);

	/// <summary>
	/// SE再生。
	/// </summary>
	/// <param name="seType">SEの種類</param>
	void PlaySE(SEType seType);

	/// <summary>
	/// BGM停止。
	/// </summary>
	/// <param name="bgmType">BGMの種類</param>
	void StopBGM(BGMType bgmType);

private:

	AudioManager() = default;
	~AudioManager() = default;
	AudioManager(AudioManager&) = delete;
	const AudioManager& operator=(AudioManager&) = delete;

	/// <summary>
	/// BGMデータ。
	/// </summary>
	/// <param name="loopSoundData">ループ音声データ</param>
	/// <param name="volume">音量</param>
	struct BGMData
	{
		YKEngine::LoopSoundData loopSoundData;
		float volume;
	};

	/// <summary>
	/// SEデータ
	/// </summary>
	/// <param name="soundData">音声データ</param>
	/// <param name="volume">音量</param>
	struct SEData
	{
		YKEngine::SoundData soundData;
		float volume;
	};

	// BGMデータ
	std::unordered_map<BGMType, BGMData> bgmDatas_;

	// SEデータ
	std::unordered_map<SEType, SEData> seDatas_;

	YKEngine::Audio* audio_;

};

