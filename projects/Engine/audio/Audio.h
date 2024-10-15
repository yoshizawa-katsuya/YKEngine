#pragma once
#include <wrl.h>
#include <xaudio2.h>
#include "Struct.h"

#pragma comment(lib,"xaudio2.lib")

class Audio
{
public:

	//シングルトンインスタンスの取得
	static Audio* GetInstance();

	//終了
	void Finalize();

	//初期化
	void Initialize();

	SoundData SoundLoadWave(const std::string& fileName);

	//音声再生
	void SoundPlayWave(const SoundData& soundData);

	//ループ再生
	void SoundLoopPlayWave(const SoundData& soundData);

	//音声データ解放
	void SoundUnload(SoundData* soundData);

private:

	static Audio* instance_;

	Audio() = default;
	~Audio() = default;
	Audio(Audio&) = default;
	Audio& operator=(Audio&) = default;

	Microsoft::WRL::ComPtr<IXAudio2> xAudio2_;
	IXAudio2MasteringVoice* masterVoice_;

};

