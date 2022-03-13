/**
 * @file convenient_directxmath.h
 * @brief DirectXMathの便利関数
 */

#pragma once

#include<DirectXMath.h>

#include"math.h"

/**
* @namespace Conv_XM
* @brief DirectXMath便利関数
*/
namespace Conv_XM
{	
	/**
	* @brief floatで2次元ベクトルを扱うクラス
	* @details XMFLOAT2を継承し扱う
	*/
	struct Vector2f : public DirectX::XMFLOAT2
	{

		Vector2f() = default;
		Vector2f(float x, float y)
		{
			this->x = x; this->y = y;
		}
		Vector2f(const DirectX::XMFLOAT2& vec)
		{
			this->x = vec.x; this->y = vec.y;
		}

		/**
		* @brief XMVECTORから2次元ベクトルを取得
		*/
		Vector2f(const DirectX::XMVECTOR& other) :XMFLOAT2() {
			DirectX::XMVECTOR temp = other;
			DirectX::XMStoreFloat2(this, temp);
		}

		inline Vector2f operator+(const Vector2f& vec) const
		{
			return Vector2f(x + vec.x, y + vec.y);
		}

		inline Vector2f operator-(const Vector2f& vec) const
		{
			return Vector2f(x - vec.x, y - vec.y);
		}

		inline Vector2f operator+=(const Vector2f& vec)
		{
			x += vec.x; y += vec.y;
			return *this;
		}

		inline Vector2f operator-=(const Vector2f& vec)
		{
			x -= vec.x; y -= vec.y;
			return *this;
		}

		inline Vector2f operator+(const float& value) const
		{
			return Vector2f(x + value, y + value);
		}

		inline Vector2f operator-(const float& value) const
		{
			return Vector2f(x - value, y - value);
		}

		inline Vector2f operator*(const float& value) const
		{
			return Vector2f(x * value, y * value);
		}

		inline Vector2f operator/(const float& value) const
		{
			return Vector2f(x / value, y / value);
		}

		inline Vector2f operator+=(const float& value)
		{
			x += value; y += value;
			return *this;
		}

		inline Vector2f operator-=(const float& value)
		{
			x -= value; y -= value;
			return *this;
		}

		inline Vector2f operator*=(const float& value)
		{
			x *= value; y *= value;
			return *this;
		}

		inline Vector2f operator/=(const float& value)
		{
			x /= value; y /= value;
			return *this;
		}

		/**
		* @brief XMVECTORから代入
		*/
		Vector2f& operator=(const DirectX::XMVECTOR& xmVector) {
			DirectX::XMStoreFloat2(this, xmVector);
			return *this;
		}

		/**
		* @brief XMVECTORにキャスト
		*/
		operator DirectX::XMVECTOR() const {
			return XMLoadFloat2(this);
		}


		/**
		* @brief 内積
		* @param[in] const Vector3f&(a)
		* @param[in] const Vector3f&(b)
		* @return float 内積
		*/
		static float Dot(const Vector2f& a, const Vector2f& b);

		/**
		* @brief 半径1の円周のランダムな点を指定
		* @return Vector2f 生成点
		*/
		static Vector2f RandomOnCircle() {
			float theta = 2.0f * DirectX::XM_PI * Math::GenerateFloatRandomNumber(0.0f, 1.0f);
			return Vector2f(cosf(theta), sinf(theta));
		}

		/**
		* @brief 指定半径の円の中のランダムな点を指定
		* @param[in] float(maxradius) 半径
		* @return Vector2f 生成点
		*/
		static Vector2f RandomInCircle(float maxradius) {
			float theta = 2.0f * DirectX::XM_PI * Math::GenerateFloatRandomNumber(0.0f, 1.0f);
			float radius = sqrtf(maxradius * Math::GenerateFloatRandomNumber(0.0f, 1.0f));

			return Vector2f(radius * cosf(theta), radius * sinf(theta));
		}

		static const Vector2f Forward;	//! 前単位ベクトル
	};

	/**
	* @brief floatで3次元ベクトルを扱うクラス
	* @details XMFLOAT3を継承し扱う
	*/
	struct Vector3f : public DirectX::XMFLOAT3
	{
		Vector3f() = default;
		Vector3f(float x, float y, float z)
		{
			this->x = x; this->y = y; this->z = z;
		}

		/**
		* @brief XMVECTORから3次元ベクトルを取得
		*/
		Vector3f(const DirectX::XMVECTOR& xmVector)
		{
			DirectX::XMStoreFloat3(this, xmVector);
		}

		inline Vector3f operator+(const Vector3f& vec) const
		{
			return Vector3f(x + vec.x , y + vec.y, z + vec.z);
		}

		inline Vector3f operator-(const Vector3f& vec) const
		{
			return Vector3f(x - vec.x, y - vec.y, z - vec.z);
		}

		inline Vector3f operator+=(const Vector3f& vec)
		{
			x += vec.x; y += vec.y; z += vec.z;
			return *this;
		}

		inline Vector3f operator-=(const Vector3f& vec)
		{
			x -= vec.x; y -= vec.y; z -= vec.z;
			return *this;
		}

		inline Vector3f operator+(const float& value) const
		{
			return Vector3f(x + value, y + value, z + value);
		}

		inline Vector3f operator-(const float& value) const
		{
			return Vector3f(x - value, y - value, z - value);
		}

		inline Vector3f operator*(const float& value) const
		{
			return Vector3f(x * value, y * value, z * value);
		}

		inline Vector3f operator/(const float& value) const
		{
			return Vector3f(x / value, y / value, z / value);
		}

		inline Vector3f operator+=(const float& value)
		{
			x += value; y += value; z += value;
			return *this;
		}

		inline Vector3f operator-=(const float& value)
		{
			x -= value; y -= value; z -= value;
			return *this;
		}

		inline Vector3f operator*=(const float& value)
		{
			x *= value; y *= value; z *= value;
			return *this;
		}

		inline Vector3f operator/=(const float& value)
		{
			x /= value; y /= value; z /= value;
			return *this;
		}


		/**
		* @brief XMVECTORから代入
		*/
		Vector3f& operator=(const DirectX::XMVECTOR& xmVector) {
			DirectX::XMStoreFloat3(this, xmVector);
			return *this;
		}

		/**
		* @brief XMVECTORにキャスト
		*/
		operator DirectX::XMVECTOR() const {
			return DirectX::XMLoadFloat3(this);
		}

		/**
		* @brief 半径1の球周のランダムな点を指定
		* @return Vector3f 生成点
		*/
		static Vector3f RandomOnSphere() {
			float cosTheta = -2.0f * Math::GenerateFloatRandomNumber(0.0f,1.0f) + 1.0f;
			float sinTheta = sqrtf(1.0f - cosTheta * cosTheta);
			float phi = 2.0f * DirectX::XM_PI * Math::GenerateFloatRandomNumber(0.0f, 1.0f);
			return Vector3f(sinTheta * cosf(phi), sinTheta * sinf(phi), cosTheta);
		}

		/**
		* @brief 指定半径の球の中のランダムな点を指定
		* @param[in] float(maxradius) 半径
		* @return Vector3f 生成点
		*/
		static Vector3f RandomInSphere(float maxradius) {
			float cosTheta = -2.0f * Math::GenerateFloatRandomNumber(0.0f, 1.0f) + 1.0f;
			float sinTheta = sqrtf(1.0f - cosTheta * cosTheta);
			float phi = 2.0f * DirectX::XM_PI * Math::GenerateFloatRandomNumber(0.0f, 1.0f);
			float radius = powf(maxradius * Math::GenerateFloatRandomNumber(0.0f, 1.0f), 1.0f / 3.0f);
			return Vector3f(radius * sinTheta * cosf(phi), radius * sinTheta * sinf(phi), radius * cosTheta);
		}

		/**
		* @brief 内積
		* @param[in] const Vector3f&(a)
		* @param[in] const Vector3f&(b)
		* @return float 内積
		*/
		static float Dot(const Vector3f& a, const Vector3f& b);

		/**
		* @brief 外積
		* @param[in] const Vector3f&(a)
		* @param[in] const Vector3f&(b)
		* @param[out] Vector3f*(pout) 外積ベクトル
		* @return Vector3f 外積ベクトル
		*/
		static Vector3f Cross(const Vector3f& a, const Vector3f& b, Vector3f* pout = 0);


		/**
		* @brief 長さ
		* @param[in] const Vector3f&(a)
		* @return float 長さ
		*/
		static float Length(const Vector3f& a);

		/**
		* @brief 長さの2乗
		* @param[in] const Vector3f&(a)
		* @return float 長さの2乗
		*/
		static float LengthSq(const Vector3f& a);

		/**
		* @brief 線形補間関数
		* @param[out] Vector3f*(out) 補間ベクトル
		* @param[in] const Vector3f&(start) 開始ベクトル
		* @param[in] const Vector3f&(end) 終了ベクトル
		* @param[in] float(t) 補間値（0～1）
		* @return Vector3f* 補間ベクトル
		*/
		static Vector3f* Linear(Vector3f* out, const Vector3f& start, const Vector3f& end, float t);

		/**
		* @brief 球面線形補間関数
		* @param[out] Vector3f*(out) 補間ベクトル
		* @param[in] const Vector3f&(start) 開始ベクトル
		* @param[in] const Vector3f&(end) 終了ベクトル
		* @param[in] float(t) 補間値（0～1）
		* @return Vector3f* 補間ベクトル
		*/
		static Vector3f* SphereLinear(Vector3f* out, const Vector3f& start, const Vector3f& end, float t);

		static const Vector3f Zero;			//! ゼロベクトル
		static const Vector3f WorldForward;	//! 前単位ベクトル
		static const Vector3f WorldRight;	//! 右単位ベクトル
		static const Vector3f WorldUp;		//! 上単位ベクトル
		static const Vector3f UnitX;		//! X単位ベクトル
		static const Vector3f UnitY;		//! Y単位ベクトル
		static const Vector3f UnitZ;		//! Z単位ベクトル
	};


	/**
	* @brief floatで4次元ベクトルを扱うクラス
	* @details XMFLOAT4を継承し扱う
	*/
	struct Vector4f : public DirectX::XMFLOAT4
	{
		Vector4f()
		{
			this->x = 0.0f; this->y = 0.0f; this->z = 0.0f; this->w = 0.0f;
		}
		Vector4f(float x, float y, float z, float w)
		{
			this->x = x; this->y = y; this->z = z; this->w = w;
		}

		/**
		* @brief XMVECTORから3次元ベクトルを取得
		*/
		Vector4f(const DirectX::XMVECTOR& xmVector)
		{
			DirectX::XMStoreFloat4(this, xmVector);
		}

		inline Vector4f operator+(const Vector4f& vec) const
		{
			return Vector4f(x + vec.x, y + vec.y, z + vec.z, w + vec.w);
		}

		inline Vector4f operator-(const Vector4f& vec) const
		{
			return Vector4f(x - vec.x, y - vec.y, z - vec.z, w - vec.w);
		}

		inline Vector4f operator+=(const Vector4f& vec)
		{
			x += vec.x; y += vec.y; z += vec.z; w += vec.w;
			return *this;
		}

		inline Vector4f operator-=(const Vector4f& vec)
		{
			x -= vec.x; y -= vec.y; z -= vec.z; w -= vec.w;
			return *this;
		}

		inline Vector4f operator+(const float& value) const
		{
			return Vector4f(x + value, y + value, z + value, w + value);
		}

		inline Vector4f operator-(const float& value) const
		{
			return Vector4f(x - value, y - value, z - value, w - value);
		}

		inline Vector4f operator*(const float& value) const
		{
			return Vector4f(x * value, y * value, z * value, w * value);
		}

		inline Vector4f operator/(const float& value) const
		{
			return Vector4f(x / value, y / value, z / value, w / value);
		}

		inline Vector4f operator+=(const float& value)
		{
			x += value; y += value; z += value; w += value;
			return *this;
		}

		inline Vector4f operator-=(const float& value)
		{
			x -= value; y -= value; z -= value; w -= value;
			return *this;
		}

		inline Vector4f operator*=(const float& value)
		{
			x *= value; y *= value; z *= value; w *= value;
			return *this;
		}

		inline Vector4f operator/=(const float& value)
		{
			x /= value; y /= value; z /= value; w /= value;
			return *this;
		}

		/**
		* @brief XMVECTORから代入
		*/
		Vector4f& operator=(const DirectX::XMVECTOR& xmVector) 
		{
			DirectX::XMStoreFloat4(this, xmVector);
			return *this;
		}

		/**
		* @brief XMVECTORにキャスト
		*/
		operator DirectX::XMVECTOR() const 
		{
			return DirectX::XMLoadFloat4(this);
		}


		/**
		* @brief ベクトルから姿勢を作成(Zを前ベクトルにする)
		*/
		static Vector4f MakeQuaternionFromVec3(const Conv_XM::Vector3f& forward);

		/**
		* @brief 角度入力
		* @param[in] float(x) 角度X
		* @param[in] float(y) 角度Y
		* @param[in] float(z) 角度Z
		* @details degreeで入力
		*/
		static Vector4f MakeQuaternionFromEuler(float x, float y, float z);

		/**
		* @brief オイラー角から姿勢を作成
		* @details degreeで入力
		*/
		static Vector4f MakeQuaternionFromEuler(const Conv_XM::Vector3f& euler);

	};

	/**
	* @brief intで4次元ベクトルを扱うクラス
	* @details XMINT4を継承し扱う
	*/
	struct Vector4i : public DirectX::XMINT4
	{
		Vector4i()
		{
			this->x = 0; this->y = 0; this->z = 0; this->w = 0;
		}
		Vector4i(int x, int y, int z, int w)
		{
			this->x = x; this->y = y; this->z = z; this->w = w;
		}

		/**
		* @brief XMVECTORから3次元ベクトルを取得
		*/
		Vector4i(const DirectX::XMVECTOR& xmVector)
		{
			DirectX::XMStoreSInt4(this, xmVector);
		}

		inline Vector4i operator+(const Vector4i& vec) const
		{
			return Vector4i(x + vec.x, y + vec.y, z + vec.z, w + vec.w);
		}

		inline Vector4i operator-(const Vector4i& vec) const
		{
			return Vector4i(x - vec.x, y - vec.y, z - vec.z, w - vec.w);
		}

		inline Vector4i operator+=(const Vector4i& vec)
		{
			x += vec.x; y += vec.y; z += vec.z; w += vec.w;
			return *this;
		}

		inline Vector4i operator-=(const Vector4i& vec)
		{
			x -= vec.x; y -= vec.y; z -= vec.z; w -= vec.w;
			return *this;
		}

		inline Vector4i operator+(const int& value) const
		{
			return Vector4i(x + value, y + value, z + value, w + value);
		}

		inline Vector4i operator-(const int& value) const
		{
			return Vector4i(x - value, y - value, z - value, w - value);
		}

		inline Vector4i operator*(const int& value) const
		{
			return Vector4i(x * value, y * value, z * value, w * value);
		}

		inline Vector4i operator/(const int& value) const
		{
			return Vector4i(x / value, y / value, z / value, w / value);
		}

		inline Vector4i operator+=(const int& value)
		{
			x += value; y += value; z += value; w += value;
			return *this;
		}

		inline Vector4i operator-=(const int& value)
		{
			x -= value; y -= value; z -= value; w -= value;
			return *this;
		}

		inline Vector4i operator*=(const int& value)
		{
			x *= value; y *= value; z *= value; w *= value;
			return *this;
		}

		inline Vector4i operator/=(const int& value)
		{
			x /= value; y /= value; z /= value; w /= value;
			return *this;
		}

		/**
		* @brief XMVECTORから代入
		*/
		Vector4i& operator=(const DirectX::XMVECTOR& xmVector) {
			DirectX::XMStoreSInt4(this, xmVector);
			return *this;
		}

		/**
		* @brief XMVECTORにキャスト
		*/
		operator DirectX::XMVECTOR() const {
			return DirectX::XMLoadSInt4(this);
		}
	};

	/**
	* @brief 行列クラス
	* @details XMFLOAT4X4を継承し扱う
	*/
	struct Matrix4x4f : public DirectX::XMFLOAT4X4
	{
		Matrix4x4f() = default;

		/**
		* @brief XMMATRIXから行列ベクトルを取得
		*/
		Matrix4x4f(const DirectX::XMMATRIX& other)
		{
			DirectX::XMStoreFloat4x4(this, other);
		}

		/**
		* @brief XMMATRIXからキャスト
		*/
		DirectX::XMFLOAT4X4& operator=(const DirectX::XMMATRIX& other) {
			DirectX::XMStoreFloat4x4(this, other);
			return *this;
		}

		/**
		* @brief XMMATRIXに代入
		*/
		operator DirectX::XMMATRIX() const {
			return XMLoadFloat4x4(this);
		}

		/**
		* @brief 線形補間
		* @param[out] Matrix4x4f*(out) 結果
		* @param[in] const Matrix4x4f&(start) 開始
		* @param[in] const Matrix4x4f&(end) 目標
		* @param[in] float(t) 補間係数（0～1）
		* @return Matrix4x4f* 結果
		*/
		static Matrix4x4f* Linear(Matrix4x4f* out, const Matrix4x4f& start, const Matrix4x4f& end, float t);

		/**
		* @brief スケール取り出し
		* @param[out] Matrix4x4f(mat) 入力
		* @return Vector3 結果
		*/
		static Vector3f MatrixToScale(const Matrix4x4f& mat);

		/**
		* @brief 球面線形補間による補間姿勢算出関数
		* @param[out] Matrix4x4f*(out) 補間姿勢
		* @param[in] const Matrix4x4f&(start) 開始姿勢
		* @param[in] const Matrix4x4f&(end) 目標姿勢
		* @param[in] float(t) 補間係数（0～1）
		* @return Matrix4x4f* 補間姿勢
		*/
		static Matrix4x4f* CalcInterPause(Matrix4x4f* out, const Matrix4x4f& start, const Matrix4x4f& end, float t);
	};
}