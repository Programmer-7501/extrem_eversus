/**
* @file time_mesurement.h
* @brief 時間計測
*/

#pragma once

/**
* @brief 時間計測クラス
*/
class TimeMeasurement
{
private:
	//! 計測開始時間
	DWORD m_Start;
	//! 計算終了時間
	DWORD m_End;
	//! 経過時間(ms)
	double m_ResultTime;
public:
	TimeMeasurement();
	~TimeMeasurement();

	/**
	* @brief 計測開始
	*/
	void Start();

	/**
	* @brief 計測終了
	*/
	void End();

	/**
	* @brief 計測結果取得
	* @details msで返す
	*/
	inline double GetResultTime() const { return m_ResultTime; }
};