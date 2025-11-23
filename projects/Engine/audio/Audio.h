#pragma once
#include <wrl.h>
#include <xaudio2.h>
#include "Struct.h"

#pragma comment(lib,"xaudio2.lib")

/// <summary>
/// オーディオ。
/// XAudio2を使用。
/// 音声読み込み、再生、終了、解放を行う。
/// Waveファイル、mp3ファイルに対応。
/// </summary>
class Audio
{
public:

	//シングルトンインスタンスの取得
	static Audio* GetInstance();

	/// <summary>
	/// 終了処理。
	/// </summary>
	void Finalize();

	/// <summary>
	/// 初期化処理。
	/// </summary>
	void Initialize();

	/// <summary>
	/// 音声読み込み。
	/// </summary>
	/// <param name="fileName">音声ファイル名</param>
	/// <returns>音声データ</returns>
	SoundData SoundLoadWave(const std::string& fileName);

	/// <summary>
	/// ループ音声読み込み。
	/// </summary>
	/// <param name="fileName">音声ファイル名</param>
	/// <returns>ループ音声データ</returns>
	LoopSoundData LoopSoundLoadWave(const std::string& fileName);

	/// <summary>
	/// 音声再生。
	/// </summary>
	/// <param name="soundData">音声データ</param>
	/// <param name="volume">音量(0.0f～1.0f)。1.0f = 100%</param>
	void SoundPlayWave(const SoundData& soundData, float volume = 1.0f);

	/// <summary>
	/// ループ音声再生。
	/// </summary>
	/// <param name="loopSoundData">ループ音声データ</param>
	/// <param name="volume">音量(0.0f～1.0f)。1.0f = 100%</param>
	void SoundLoopPlayWave(const LoopSoundData& loopSoundData, float volume = 1.0f);

	/// <summary>
	/// 再生を停止。
	/// </summary>
	/// <param name="loopSoundData">ループ音声データ</param>
	void SoundStopWave(const LoopSoundData& loopSoundData);

	/// <summary>
	/// 音声解放。
	/// </summary>
	/// <param name="soundData">音声データ</param>
	void SoundUnload(SoundData* soundData);

	/// <summary>
	/// ループ音声解放。
	/// </summary>
	/// <param name="loopSoundData">ループ音声データ</param>
	void SoundUnload(LoopSoundData* loopSoundData);

private:

	/// <summary>
	/// mp3音声読み込み。
	/// </summary>
	/// <param name="fileName">音声ファイル名</param>
	/// <returns>音声データ</returns>
	SoundData SoundLoadMp3(const std::string& fileName);

	// シングルトンインスタンス
	static Audio* instance_;

	Audio() = default;
	~Audio() = default;
	Audio(Audio&) = delete;
	Audio& operator=(Audio&) = delete;

	Microsoft::WRL::ComPtr<IXAudio2> xAudio2_;
	IXAudio2MasteringVoice* masterVoice_;

};

