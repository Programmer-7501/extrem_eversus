/**
 * @file convenientdirectxmath.h
 * @brief ���w�֗��@�\�ꗗ
 */

#pragma once

 /**
 * @namespace Math
 * @brief �֗����w�@�\
 */
namespace Math
{
	//! �~����
	static constexpr float k_PI = 3.141592f;

	//! �덷
	static constexpr float k_Epsilon = 0.000001f;

	/**
	* @brief �w��͈͓��̗����𐶐�����֐�
	* @param[in] float(min) �͈͂̉���
	* @param[in] float(max) �͈͂̏��
	* @return float ��������
	*/
	float GenerateFloatRandomNumber(float min, float max);


	/**
	* @brief 0����1�𐶐����闐��
	* @return float ��������
	*/
	float GenerateZeroToOneRand();

	template<typename T>
	T Clamp(const T& value, const T& min, const T& max)
	{
		if (value > max)// �ő�l�𒴂��Ă���ꍇ
		{
			return max;
		}
		else if (value < min)// �ŏ��l��������Ă���ꍇ
		{
			return min;
		}

		// �͈͓��̏ꍇ
		return value;
	}


	/**
	* @brief �����擾
	*/
	unsigned GetDigit(unsigned num);
}
