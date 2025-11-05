#include "Audio.h"
#include <fstream>
#include <cassert>
#include <mfapi.h>
#include <mfobjects.h>
#include <mfidl.h>
#include <mfreadwrite.h>

#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mfreadwrite.lib")
#pragma comment(lib, "mfuuid.lib")

Audio* Audio::GetInstance()
{
	static Audio instance;
	return &instance;
}

void Audio::Finalize()
{
	//MFの終了
	MFShutdown();
}

void Audio::Initialize()
{

	//XAudioのインスタンスを生成
	HRESULT result = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
	//マスターボイスを生成
	result = xAudio2_->CreateMasteringVoice(&masterVoice_);

	//MFの初期化
	MFStartup(MF_VERSION);
}

SoundData Audio::SoundLoadWave(const std::string& fileName)
{
	
	if (fileName.ends_with(".mp3"))
	{
		//MP3ファイルの読み込み
		return SoundLoadMp3(fileName);
	}

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

SoundData Audio::SoundLoadMp3(const std::string& fileName)
{
	//MP3ファイルの読み込み
	//MFソースリーダーの生成
	IMFSourceReader* pReader = nullptr;
	MFCreateSourceReaderFromURL(std::wstring(fileName.begin(), fileName.end()).c_str(), nullptr, &pReader);

	IMFMediaType* pAudioType = nullptr;
	MFCreateMediaType(&pAudioType);
	pAudioType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
	pAudioType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);

	pReader->SetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, nullptr, pAudioType);
	pAudioType->Release();

	IMFMediaType* pPartialType = nullptr;
	pReader->GetCurrentMediaType((DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM, &pPartialType);

	WAVEFORMATEX* pWaveFormat = nullptr;
	UINT32 cbFormat = 0;
	MFCreateWaveFormatExFromMFMediaType(pPartialType, &pWaveFormat, &cbFormat);
	pPartialType->Release();

	SoundData soundData = {};

	while (true) 
	{
		DWORD dwFlags = 0;
		IMFSample* pSample = nullptr;
		IMFMediaBuffer* pBuffer = nullptr;

		HRESULT hr = pReader->ReadSample(
			MF_SOURCE_READER_FIRST_AUDIO_STREAM,
			0, nullptr, &dwFlags, nullptr, &pSample
		);

		if (FAILED(hr) || (dwFlags & MF_SOURCE_READERF_ENDOFSTREAM))
		{
			break; // 読み終わり
		}

		// サンプルからバッファを取得
		if (pSample)
		{
			pSample->ConvertToContiguousBuffer(&pBuffer);

			BYTE* pAudioData = nullptr;
			DWORD cbBuffer = 0;
			pBuffer->Lock(&pAudioData, nullptr, &cbBuffer);

			soundData.pcmData.insert(soundData.pcmData.end(), pAudioData, pAudioData + cbBuffer);

			pBuffer->Unlock();
			pBuffer->Release();
			pSample->Release();
		}
	}

	pReader->Release();

	//returnするための音声データ
	soundData.wfex = *pWaveFormat;
	soundData.pBuffer = soundData.pcmData.data();
	soundData.bufferSize = (UINT32)soundData.pcmData.size();

	return soundData;
}
