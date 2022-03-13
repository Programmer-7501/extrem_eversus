/**
* @file audio_manager.h
* @brief オーディオ制御
*/

#pragma once

#include"../function/singleton.h"
#include <xaudio2.h>

/**
* @namespace AudioManager
* @brief オーディオ制御クラス
* @details xaudio2使用
*/
class AudioManager final : public Singleton<AudioManager>
{
private:
	struct AudioData
	{
		IXAudio2SourceVoice* SourceVoice;
		BYTE* SoundData;
		LONG Length;
		int	PlayLength;
	};

private:
	IXAudio2* xaudio;
	IXAudio2MasteringVoice* mastering_voice;

	std::unordered_map<std::string, AudioData> m_AudioData;
private:
	friend class Singleton<AudioManager>;

	AudioManager() = default;
	~AudioManager() = default;

public:
	/**
	* @brief 初期化処理
	*/
	bool Init();

	/**
	* @brief 終了処理処理
	*/
	void Uninit();

	/**
	* @brief ロード
	*/
	void Load(const std::string& fileName);

	/**
	* @brief アンロード
	*/
	void Unload(const std::string& fileName);

	/**
	* @brief 再生
	*/
	void Play(std::string fileName, bool loop);

	/**
	* @brief 停止
	*/
	void Stop(std::string fileName);

	/**
	* @brief ボリュームコントロール
	* @param[in] float(volume) ボリューム
	* @details -XAUDIO2_MAX_VOLUME_LEVELとXAUDIO2_MAX_VOLUME_LEVEL（-2²⁴から2²⁴）
	* @details 音量レベル1.0は減衰またはゲインがないことを意味し、0は無音を意味します。
	* @details 負のレベルを使用して、オーディオの位相を反転させることができます
	*/
	void SetVolume(std::string fileName, float volume);

	/**
	* @brief オーディオデータクリア
	*/
	void AudioDataClear();
};