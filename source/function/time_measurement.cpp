
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
	// �J�n���Ԏ擾
	m_Start = timeGetTime();
}

void TimeMeasurement::End()
{
	// �I�����Ԏ擾
	m_End = timeGetTime();

	// �o�ߎ��Ԍv�Z
	m_ResultTime = static_cast<double>(m_End - m_Start);
}
