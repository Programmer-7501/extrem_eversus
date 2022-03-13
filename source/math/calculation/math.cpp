
#include "math.h"
#include<random>

float Math::GenerateFloatRandomNumber(float min, float max)
{
	std::random_device rnd;     // 非決定的な乱数生成器を生成
	std::mt19937 mt(rnd());     //  メルセンヌ・ツイスタの32ビット版
	std::uniform_real_distribution<float> distr(min, max);//aからbの範囲のfloat乱数

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
