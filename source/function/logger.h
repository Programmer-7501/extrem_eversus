/**
* @file logger.h
* @brief ログ出力クラス
* @see https://an-embedded-engineer.hateblo.jp/entry/2020/08/29/230546
*/

#pragma once

#include"singleton.h"

/**
* @brief ログ出力クラス
* @details SetLogした情報をInitで入力したfileに出力する
*/
class Logger : public Singleton<Logger>
{
private:
	//! ファイルネーム
	std::string m_FileName = "Log.txt";
	//! ログデータ
	std::vector<std::string> m_LogData;

private:
	friend class Singleton<Logger>;

	Logger() = default;
	~Logger() = default;
public:
	/**
	* @brief ログ入力
	* @param[in] const std::string&(log) ログ
	*/
	void SetLog(const std::string& log);

	/**
	* @brief ファイルにログ出力
	*/
	void OutPut();
};