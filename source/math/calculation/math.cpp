
#include "math.h"
#include<random>

float Math::GenerateFloatRandomNumber(float min, float max)
{
	std::random_device rnd;     // �񌈒�I�ȗ���������𐶐�
	std::mt19937 mt(rnd());     //  �����Z���k�E�c�C�X�^��32�r�b�g��
	std::uniform_real_distribution<float> distr(min, max);//a����b�͈̔͂�float����

	return distr(mt);
}

float Math::GenerateZeroToOneRand()
{
	return static_cast<float>(rand()) / RAND_MAX;
}

unsigned Math::GetDigit(unsigned num)
{
	unsigned digit = 0;

	while (num != 0) 
	{
		num /= 10;
		digit++;
	}

	return digit;
}
