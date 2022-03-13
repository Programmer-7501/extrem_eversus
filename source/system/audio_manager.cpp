
#include"../system/main.h"
#include "audio_manager.h"

bool AudioManager::Init()
{
	//
	//  Initialize XAudio2
	//
	HRESULT hr;
	hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hr) == true)
	{
		Logger::GetInstance().SetLog("CoInitializeEx ���s");
		return false;
	}

	UINT32 flags = 0;
#ifdef _DEBUG
	flags |= XAUDIO2_DEBUG_ENGINE;
#endif
	hr = XAudio2Create(&xaudio, flags);
	if (FAILED(hr) == true)
	{
		Logger::GetInstance().SetLog("XAudio2Create ���s");
		CoUninitialize();
		return false;
	}

	//
	//  Create a mastering voice
	//
	hr = xaudio->CreateMasteringVoice(&mastering_voice);
	if (FAILED(hr) == true)
	{
		Logger::GetInstance().SetLog("CreateMasteringVoice ���s");
		if (xaudio != 0)
		{
			xaudio->Release();
			xaudio = 0;
		}
		CoUninitialize();
		return false;
	}

	return true;
}

void AudioManager::Uninit()
{
	// �I�[�f�B�I�f�[�^�폜
	AudioDataClear();

	if (mastering_voice != 0)
	{
		mastering_voice->DestroyVoice();
		mastering_voice = 0;
	}
	if (xaudio != 0)
	{
		xaudio->Release();
		xaudio = 0;
	}
	CoUninitialize();
}

void AudioManager::Load(const std::string& fileName)
{
	// �ǂݍ��ݍς݂��m�F���邽�ߘA�z�z�񂩂�T��
	auto iter = m_AudioData.find(fileName);

	if (iter == m_AudioData.end())
	{
		// �T�E���h�f�[�^�Ǎ�
		WAVEFORMATEX wfx = { 0 };


		HMMIO hmmio = NULL;
		MMIOINFO mmioinfo = { 0 };
		MMCKINFO riffchunkinfo = { 0 };
		MMCKINFO datachunkinfo = { 0 };
		MMCKINFO mmckinfo = { 0 };
		UINT32 buflen;
		LONG readlen;


		hmmio = mmioOpen((LPSTR)fileName.c_str(), &mmioinfo, MMIO_READ);
		assert(hmmio);

		riffchunkinfo.fccType = mmioFOURCC('W', 'A', 'V', 'E');
		mmioDescend(hmmio, &riffchunkinfo, NULL, MMIO_FINDRIFF);

		mmckinfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
		mmioDescend(hmmio, &mmckinfo, &riffchunkinfo, MMIO_FINDCHUNK);

		if (mmckinfo.cksize >= sizeof(WAVEFORMATEX))
		{
			mmioRead(hmmio, (HPSTR)&wfx, sizeof(wfx));
		}
		else
		{
			PCMWAVEFORMAT pcmwf = { 0 };
			mmioRead(hmmio, (HPSTR)&pcmwf, sizeof(pcmwf));
			memset(&wfx, 0x00, sizeof(wfx));
			memcpy(&wfx, &pcmwf, sizeof(pcmwf));
			wfx.cbSize = 0;
		}
		mmioAscend(hmmio, &mmckinfo, 0);

		datachunkinfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
		mmioDescend(hmmio, &datachunkinfo, &riffchunkinfo, MMIO_FINDCHUNK);

		AudioData audioData = {};

		buflen = datachunkinfo.cksize;
		audioData.SoundData = new unsigned char[buflen];
		readlen = mmioRead(hmmio, (HPSTR)audioData.SoundData, buflen);


		audioData.Length = readlen;
		audioData.PlayLength = readlen / wfx.nBlockAlign;

		mmioClose(hmmio, 0);

		// �T�E���h�\�[�X����
		xaudio->CreateSourceVoice(&audioData.SourceVoice, &wfx);

		m_AudioData.emplace(fileName, audioData);
	}
}

void AudioManager::Unload(const std::string & fileName)
{
	// �ǂݍ��ݍς݂��m�F���邽�ߘA�z�z�񂩂�T��
	auto iter = m_AudioData.find(fileName);

	if (iter != m_AudioData.end())
	{
		XAUDIO2_VOICE_STATE xa2state;

		// ��Ԏ擾
		iter->second.SourceVoice->GetState(&xa2state);
		// �Đ����Ȃ�
		if (xa2state.BuffersQueued != 0)
		{
			// ��~
			iter->second.SourceVoice->Stop(0);
			// �I�[�f�B�I�o�b�t�@�̍폜
			iter->second.SourceVoice->FlushSourceBuffers();
		}

		iter->second.SourceVoice->DestroyVoice();

		if (iter->second.SoundData)
		{
			delete[] iter->second.SoundData;
		}

		// ���g������
		m_AudioData.erase(iter);
	}
}

void AudioManager::Play(std::string fileName, bool loop)
{

	// �ǂݍ��ݍς݂��m�F���邽�ߘA�z�z�񂩂�T��
	auto iter = m_AudioData.find(fileName);

	if (iter != m_AudioData.end())
	{
		XAUDIO2_VOICE_STATE xa2state;
		XAUDIO2_BUFFER buffer;

		// �o�b�t�@�̒l�ݒ�
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = iter->second.Length;
		buffer.pAudioData = iter->second.SoundData;
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount = loop;

		// ��Ԏ擾
		iter->second.SourceVoice->GetState(&xa2state);
		if (xa2state.BuffersQueued != 0)
		{// �Đ���
			// �ꎞ��~
			iter->second.SourceVoice->Stop(0);

			// �I�[�f�B�I�o�b�t�@�̍폜
			iter->second.SourceVoice->FlushSourceBuffers();
		}

		// �I�[�f�B�I�o�b�t�@�̓o�^
		iter->second.SourceVoice->SubmitSourceBuffer(&buffer);

		// �Đ�
		iter->second.SourceVoice->Start(0);
	}
}

void AudioManager::Stop(std::string fileName)
{
	// �ǂݍ��ݍς݂��m�F���邽�ߘA�z�z�񂩂�T��
	auto iter = m_AudioData.find(fileName);

	if (iter != m_AudioData.end())
	{
		XAUDIO2_VOICE_STATE xa2state;

		// ��Ԏ擾
		iter->second.SourceVoice->GetState(&xa2state);
		// �Đ���
		if (xa2state.BuffersQueued != 0)
		{
			// �ꎞ��~
			iter->second.SourceVoice->Stop(0);

			// �I�[�f�B�I�o�b�t�@�̍폜
			iter->second.SourceVoice->FlushSourceBuffers();
		}
	}
}

void AudioManager::SetVolume(std::string fileName, float volume)
{
	// �ǂݍ��ݍς݂��m�F���邽�ߘA�z�z�񂩂�T��
	auto iter = m_AudioData.find(fileName);

	if (iter != m_AudioData.end())
	{
		XAUDIO2_VOICE_STATE xa2state;

		// ��Ԏ擾
		iter->second.SourceVoice->GetState(&xa2state);
		// �Đ���
		if (xa2state.BuffersQueued != 0)
		{
			// �{�����[���ύX
			iter->second.SourceVoice->SetVolume(volume);
		}
	}
}

void AudioManager::AudioDataClear()
{
	for (auto audioData : m_AudioData)
	{
		XAUDIO2_VOICE_STATE xa2state;

		// ��Ԏ擾
		audioData.second.SourceVoice->GetState(&xa2state);
		// �Đ���
		if (xa2state.BuffersQueued != 0)
		{
			audioData.second.SourceVoice->Stop(0);
			// �I�[�f�B�I�o�b�t�@�̍폜
			audioData.second.SourceVoice->FlushSourceBuffers();
		}
		audioData.second.SourceVoice->DestroyVoice();

		if (audioData.second.SoundData)
		{
			delete[] audioData.second.SoundData;
		}
	}

	// �z����N���A
	m_AudioData.clear();
}
