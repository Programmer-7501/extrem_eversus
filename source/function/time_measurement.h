/**
* @file time_mesurement.h
* @brief ���Ԍv��
*/

#pragma once

/**
* @brief ���Ԍv���N���X
*/
class TimeMeasurement
{
private:
	//! �v���J�n����
	DWORD m_Start;
	//! �v�Z�I������
	DWORD m_End;
	//! �o�ߎ���(ms)
	double m_ResultTime;
public:
	TimeMeasurement();
	~TimeMeasurement();

	/**
	* @brief �v���J�n
	*/
	void Start();

	/**
	* @brief �v���I��
	*/
	void End();

	/**
	* @brief �v�����ʎ擾
	* @details ms�ŕԂ�
	*/
	inline double GetResultTime() const { return m_ResultTime; }
};