
#include"../system/main.h"
#include "logger.h"
#include"../system/mywindows.h"

void Logger::SetLog(const std::string & log)
{
#if defined(_DEBUG) || defined(DEBUG)
	m_LogData.emplace_back(log);
#endif	//	_DEBUG || DEBUG
}

void Logger::OutPut()
{
#if defined(_DEBUG) || defined(DEBUG)
	//���O���Ȃ��ꍇ
	if (m_LogData.empty())
	{
		return;
	}

	// �t�@�C�����J���A�|�C���^��ۑ�
	FILE* file = fopen(m_FileName.data(), "w");

	// �t�@�C�����J���Ȃ������ꍇ
	if (file == nullptr)
	{
		return;
	}

	for (auto& log : m_LogData)
	{
		std::string outlog = log + '\n';
		// �������ރ��O�̓��e��ۑ�����
		fprintf(file, outlog.c_str());
		// �f�o�b�O��ʂɏ�������s
		OutputDebugStringA(outlog.c_str());
		// �t�@�C���ɂ���������
		fflush(file);
	}
	//�t�@�C�������
	fclose(file);

	// �x������
	std::string errorlog = m_FileName + "���Q�Ƃ��Ă�������";
	MessageBox(NULL, errorlog.c_str(), "�G���[����", MB_OKCANCEL | MB_ICONERROR);
#endif	//	_DEBUG || DEBUG
}
