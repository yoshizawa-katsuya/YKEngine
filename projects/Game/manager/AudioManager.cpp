#include "AudioManager.h"

using namespace YKEngine;

AudioManager* AudioManager::GetInstance()
{
	static AudioManager instance;
	return &instance;
}

void AudioManager::Finalize()
{
	//全てのBGMを停止
	for (const std::pair<const BGMType, BGMData>& bgmData : bgmDatas_)
	{
		audio_->SoundStopWave(bgmData.second.loopSoundData);
	}
}

void AudioManager::Initialize()
{
	audio_ = Audio::GetInstance();

	std::unordered_map<BGMType, std::string> bgmFilePaths
	{
		{BGMType::kTitle, "./Resources/BGM/title.mp3"},
		{BGMType::kGame, "./Resources/BGM/game.mp3"},
		{BGMType::kGameOver, "./Resources/BGM/gameOver.mp3"},
		{BGMType::kClear, "./Resources/BGM/clear.mp3"}
	};

	for (const std::pair<BGMType, std::string>& bgmFilePath : bgmFilePaths)
	{
		BGMData& bgmData = bgmDatas_[bgmFilePath.first];
		bgmData.loopSoundData = audio_->LoopSoundLoadWave(bgmFilePath.second);
		bgmData.volume = 0.5f;
	}

}

void AudioManager::PlayBGM(BGMType bgmType)
{
	//全てのBGMを停止
	for (const std::pair<const BGMType, BGMData>& bgmData : bgmDatas_)
	{
		audio_->SoundStopWave(bgmData.second.loopSoundData);
	}
	//指定されたBGMを再生
	const BGMData& bgmData = bgmDatas_[bgmType];
	audio_->SoundLoopPlayWave(bgmData.loopSoundData, bgmData.volume);
}

void AudioManager::StopBGM(BGMType bgmType)
{
	const BGMData& bgmData = bgmDatas_[bgmType];
	audio_->SoundStopWave(bgmData.loopSoundData);
}
