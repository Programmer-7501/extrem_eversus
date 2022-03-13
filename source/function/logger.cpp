
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
	//ログがない場合
	if (m_LogData.empty())
	{
		return;
	}

	// ファイルを開き、ポインタを保存
	FILE* file = fopen(m_FileName.data(), "w");

	// ファイルが開けなかった場合
	if (file == nullptr)
	{
		return;
	}

	for (auto& log : m_LogData)
	{
		std::string outlog = log + '\n';
		// 書き込むログの内容を保存する
		fprintf(file, outlog.c_str());
		// デバッグ画面に書き込むs
		OutputDebugStringA(outlog.c_str());
		// ファイルにを書き込む
		fflush(file);
	}
	//ファイルを閉じる
	fclose(file);

	// 警告する
	std::string errorlog = m_FileName + "を参照してください";
	MessageBox(NULL, errorlog.c_str(), "エラー発生", MB_OKCANCEL | MB_ICONERROR);
#endif	//	_DEBUG || DEBUG
}
