/**
 * @file convenientdirectxmath.h
 * @brief 数学便利機能一覧
 */

#pragma once

 /**
 * @namespace Math
 * @brief 便利数学機能
 */
namespace Math
{
	//! 円周率
	static constexpr float k_PI = 3.141592f;

	//! 誤差
	static constexpr float k_Epsilon = 0.000001f;

	/**
	* @brief 指定範囲内の乱数を生成する関数
	* @param[in] float(min) 範囲の下限
	* @param[in] float(max) 範囲の上限
	* @return float 生成乱数
	*/
	float GenerateFloatRandomNumber(float min, float max);


	/**
	* @brief 0から1を生成する乱数
	* @return float 生成乱数
	*/
	float GenerateZeroToOneRand();

	template<typename T>
	T Clamp(const T& value, const T& min, const T& max)
	{
		if (value > max)// 最大値を超えている場合
		{
			return max;
		}
		else if (value < min)// 最小値を下回っている場合
		{
			return min;
		}

		// 範囲内の場合
		return value;
	}


	/**
	* @brief 桁数取得
	*/
	unsigned GetDigit(unsigned num);
}
