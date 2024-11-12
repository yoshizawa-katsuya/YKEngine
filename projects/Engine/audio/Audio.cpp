#include "Audio.h"
#include <fstream>
#include <cassert>

Audio* Audio::GetInstance()
{
	static Audio instance;
	return &instance;
}

void Audio::Finalize()
{
	
}

void Audio::Initialize()
{

	//XAudioのインスタンスを生成
	HRESULT result = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
	//マスターボイスを生成
	result = xAudio2_->CreateMasteringVoice(&masterVoice_);

}

SoundData Audio::SoundLoadWave(const std::string& fileName)
{
	
	//ファイル入力ストリームのインスタンス
	std::ifstream file;
	//.wavファイルをバイナリモードで開く
	file.open(fileName, std::ios_base::binary);
	//ファイルオープン失敗を検出する
	assert(file.is_open());

	//RIFFヘッダーの読み込み
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	//ファイルがRIFFかチェック
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}
	//タイプがWAVEかチェック
	if (strncmp(riff.type, "WAVE", 4) != 0) {
		assert(0);
	}

	//Formatチャンクの読み込み
	FormatChunk format = {};
	//チャンクヘッダーの確認
	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
		assert(0);
	}

	//チャンク本体の読み込み
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	//Dataチャンクの読み込み
	ChunkHeader data;
	file.read((char*)&data, sizeof(data));

	//LISTチャンクを検出した場合
	if (strncmp(data.id, "LIST", 4) == 0) {
		//読み取り位置をJUNKチャンクの終わりまで進める
		file.seekg(data.size, std::ios_base::cur);
		//再読み込み
		file.read((char*)&data, sizeof(data));
	}

	//JUNKチャンクを検出した場合
	if (strncmp(data.id, "JUNK", 4) == 0) {
		//読み取り位置をJUNKチャンクの終わりまで進める
		file.seekg(data.size, std::ios_base::cur);
		//再読み込み
		file.read((char*)&data, sizeof(data));
	}
	
	if (strncmp(data.id, "data", 4) != 0) {
		assert(0);
	}

	//Dataチャンクのデータ部(波形データ)の読み込み
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	//Waveファイルを閉じる
	file.close();

	//returnするための音声データ
	SoundData soundData = {};

	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;

	return soundData;

}

LoopSoundData Audio::LoopSoundLoadWave(const std::string& fileName)
{
	
	LoopSoundData loopSoundData;

	loopSoundData.soundData = SoundLoadWave(fileName);

	//波形フォーマットを元にSoundVoiceの生成
	HRESULT result;
	result = xAudio2_->CreateSourceVoice(&loopSoundData.pSourceVoice, &loopSoundData.soundData.wfex);

	return loopSoundData;
}

void Audio::SoundPlayWave(const SoundData& soundData, float volume)
{

	HRESULT result;

	//波形フォーマットを元にSoundVoiceの生成
	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = xAudio2_->CreateSourceVoice(&pSourceVoice, &soundData.wfex);

	//再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	//波形データの再生
	result = pSourceVoice->SubmitSourceBuffer(&buf);
	result = pSourceVoice->SetVolume(volume);
	result = pSourceVoice->Start();

}

void Audio::SoundLoopPlayWave(const LoopSoundData& loopSoundData, float volume)
{

	HRESULT result;

	//再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = loopSoundData.soundData.pBuffer;
	buf.AudioBytes = loopSoundData.soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	//ループ再生の設定
	buf.LoopCount = XAUDIO2_LOOP_INFINITE;

	//波形データの再生
	result = loopSoundData.pSourceVoice->SubmitSourceBuffer(&buf);
	result = loopSoundData.pSourceVoice->SetVolume(volume);
	result = loopSoundData.pSourceVoice->Start();
}

void Audio::SoundStopWave(const LoopSoundData& loopSoundData)
{

	if (loopSoundData.pSourceVoice) {
		//再生を停止
		loopSoundData.pSourceVoice->Stop(0);

		loopSoundData.pSourceVoice->FlushSourceBuffers();
	}

}

void Audio::SoundUnload(SoundData* soundData)
{

	//バッファのメモリを解放
	delete[] soundData->pBuffer;

	soundData->pBuffer = 0;
	soundData->bufferSize = 0;
	soundData->wfex = {};

}

void Audio::SoundUnload(LoopSoundData* loopSoundData)
{

	delete loopSoundData->pSourceVoice;

	SoundUnload(&loopSoundData->soundData);

}
