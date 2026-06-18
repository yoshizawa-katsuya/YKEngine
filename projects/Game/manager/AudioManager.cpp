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

	// BGMのファイルパスを設定
	std::unordered_map<BGMType, std::string> bgmFilePaths
	{
		{BGMType::kTitle, "./Resources/BGM/title.mp3"},
		{BGMType::kGame, "./Resources/BGM/game.mp3"},
		{BGMType::kGameOver, "./Resources/BGM/gameOver.mp3"},
		{BGMType::kClear, "./Resources/BGM/clear.mp3"}
	};

	// BGMデータをロード
	for (const std::pair<BGMType, std::string>& bgmFilePath : bgmFilePaths)
	{
		BGMData& bgmData = bgmDatas_[bgmFilePath.first];
		bgmData.loopSoundData = audio_->LoopSoundLoadWave(bgmFilePath.second);
		bgmData.volume = 0.5f;
	}

	// SEのファイルパスを設定
	std::unordered_map<SEType, std::string> seFilePaths
	{
		{SEType::kShot01, "./Resources/SE/shot01.mp3"},
		{SEType::kShot02, "./Resources/SE/shot02.mp3"},
		{SEType::kShot03, "./Resources/SE/shot03.mp3"},
		{SEType::kDamage01, "./Resources/SE/damage01.mp3"},
		{SEType::kDamage02, "./Resources/SE/damage02.mp3"},
		{SEType::kDeath01, "./Resources/SE/death01.mp3"},
		{SEType::kStart01, "./Resources/SE/start01.mp3"},
		{SEType::kStart02, "./Resources/SE/start02.mp3"},
		{SEType::kAppear01, "./Resources/SE/appear01.wav"},
		{SEType::kAppear02, "./Resources/SE/appear02.mp3"},
		{SEType::kAppear03, "./Resources/SE/appear03.mp3"},
		{SEType::kDodge01, "./Resources/SE/dodge01.wav"},
		{SEType::kDecision01, "./Resources/SE/decision01.mp3"},
		{SEType::kMenu01, "./Resources/SE/menu01.mp3"},
		{SEType::kCursorMove01, "./Resources/SE/cursorMove01.mp3"}
	};

	for (const std::pair<SEType, std::string>& seFilePath : seFilePaths)
	{
		SEData& seData = seDatas_[seFilePath.first];
		seData.soundData = audio_->SoundLoadWave(seFilePath.second);
		seData.volume = 0.8f;
	}
}

void AudioManager::PlayBGM(BGMType bgmType)
{
	//指定されたBGMを再生
	const BGMData& bgmData = bgmDatas_[bgmType];
	audio_->SoundLoopPlayWave(bgmData.loopSoundData, bgmData.volume);
}

void AudioManager::PlaySE(SEType seType)
{
	const SEData& seData = seDatas_[seType];
	audio_->SoundPlayWave(seData.soundData, seData.volume);
}

void AudioManager::StopBGM(BGMType bgmType)
{
	const BGMData& bgmData = bgmDatas_[bgmType];
	audio_->SoundStopWave(bgmData.loopSoundData);
}
