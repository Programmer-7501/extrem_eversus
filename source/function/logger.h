/**
* @file logger.h
* @brief ���O�o�̓N���X
* @see https://an-embedded-engineer.hateblo.jp/entry/2020/08/29/230546
*/

#pragma once

#include"singleton.h"

/**
* @brief ���O�o�̓N���X
* @details SetLog��������Init�œ��͂���file�ɏo�͂���
*/
class Logger : public Singleton<Logger>
{
private:
	//! �t�@�C���l�[��
	std::string m_FileName = "Log.txt";
	//! ���O�f�[�^
	std::vector<std::string> m_LogData;

private:
	friend class Singleton<Logger>;

	Logger() = default;
	~Logger() = default;
public:
	/**
	* @brief ���O����
	* @param[in] const std::string&(log) ���O
	*/
	void SetLog(const std::string& log);

	/**
	* @brief �t�@�C���Ƀ��O�o��
	*/
	void OutPut();
};