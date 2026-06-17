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
	kShot01,
	kDamage01,
	kDamage02,
	kDeath01,
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

	struct BGMData
	{
		YKEngine::LoopSoundData loopSoundData;
		float volume;
	};

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

