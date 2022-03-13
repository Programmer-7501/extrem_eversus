
#include"../system/main.h"
#include "time_measurement.h"

TimeMeasurement::TimeMeasurement()
{
}

TimeMeasurement::~TimeMeasurement()
{
}

void TimeMeasurement::Start()
{
	// 開始時間取得
	m_Start = timeGetTime();
}

void TimeMeasurement::End()
{
	// 終了時間取得
	m_End = timeGetTime();

	// 経過時間計算
	m_ResultTime = static_cast<double>(m_End - m_Start);
}
