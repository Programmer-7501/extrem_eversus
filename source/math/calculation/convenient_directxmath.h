/**
 * @file convenient_directxmath.h
 * @brief DirectXMath�֗̕��֐�
 */

#pragma once

#include<DirectXMath.h>

#include"math.h"

/**
* @namespace Conv_XM
* @brief DirectXMath�֗��֐�
*/
namespace Conv_XM
{	
	/**
	* @brief float��2�����x�N�g���������N���X
	* @details XMFLOAT2���p��������
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
		* @brief XMVECTOR����2�����x�N�g�����擾
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
		* @brief XMVECTOR������
		*/
		Vector2f& operator=(const DirectX::XMVECTOR& xmVector) {
			DirectX::XMStoreFloat2(this, xmVector);
			return *this;
		}

		/**
		* @brief XMVECTOR�ɃL���X�g
		*/
		operator DirectX::XMVECTOR() const {
			return XMLoadFloat2(this);
		}


		/**
		* @brief ����
		* @param[in] const Vector3f&(a)
		* @param[in] const Vector3f&(b)
		* @return float ����
		*/
		static float Dot(const Vector2f& a, const Vector2f& b);

		/**
		* @brief ���a1�̉~���̃����_���ȓ_���w��
		* @return Vector2f �����_
		*/
		static Vector2f RandomOnCircle() {
			float theta = 2.0f * DirectX::XM_PI * Math::GenerateFloatRandomNumber(0.0f, 1.0f);
			return Vector2f(cosf(theta), sinf(theta));
		}

		/**
		* @brief �w�蔼�a�̉~�̒��̃����_���ȓ_���w��
		* @param[in] float(maxradius) ���a
		* @return Vector2f �����_
		*/
		static Vector2f RandomInCircle(float maxradius) {
			float theta = 2.0f * DirectX::XM_PI * Math::GenerateFloatRandomNumber(0.0f, 1.0f);
			float radius = sqrtf(maxradius * Math::GenerateFloatRandomNumber(0.0f, 1.0f));

			return Vector2f(radius * cosf(theta), radius * sinf(theta));
		}

		static const Vector2f Forward;	//! �O�P�ʃx�N�g��
	};

	/**
	* @brief float��3�����x�N�g���������N���X
	* @details XMFLOAT3���p��������
	*/
	struct Vector3f : public DirectX::XMFLOAT3
	{
		Vector3f() = default;
		Vector3f(float x, float y, float z)
		{
			this->x = x; this->y = y; this->z = z;
		}

		/**
		* @brief XMVECTOR����3�����x�N�g�����擾
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
		* @brief XMVECTOR������
		*/
		Vector3f& operator=(const DirectX::XMVECTOR& xmVector) {
			DirectX::XMStoreFloat3(this, xmVector);
			return *this;
		}

		/**
		* @brief XMVECTOR�ɃL���X�g
		*/
		operator DirectX::XMVECTOR() const {
			return DirectX::XMLoadFloat3(this);
		}

		/**
		* @brief ���a1�̋����̃����_���ȓ_���w��
		* @return Vector3f �����_
		*/
		static Vector3f RandomOnSphere() {
			float cosTheta = -2.0f * Math::GenerateFloatRandomNumber(0.0f,1.0f) + 1.0f;
			float sinTheta = sqrtf(1.0f - cosTheta * cosTheta);
			float phi = 2.0f * DirectX::XM_PI * Math::GenerateFloatRandomNumber(0.0f, 1.0f);
			return Vector3f(sinTheta * cosf(phi), sinTheta * sinf(phi), cosTheta);
		}

		/**
		* @brief �w�蔼�a�̋��̒��̃����_���ȓ_���w��
		* @param[in] float(maxradius) ���a
		* @return Vector3f �����_
		*/
		static Vector3f RandomInSphere(float maxradius) {
			float cosTheta = -2.0f * Math::GenerateFloatRandomNumber(0.0f, 1.0f) + 1.0f;
			float sinTheta = sqrtf(1.0f - cosTheta * cosTheta);
			float phi = 2.0f * DirectX::XM_PI * Math::GenerateFloatRandomNumber(0.0f, 1.0f);
			float radius = powf(maxradius * Math::GenerateFloatRandomNumber(0.0f, 1.0f), 1.0f / 3.0f);
			return Vector3f(radius * sinTheta * cosf(phi), radius * sinTheta * sinf(phi), radius * cosTheta);
		}

		/**
		* @brief ����
		* @param[in] const Vector3f&(a)
		* @param[in] const Vector3f&(b)
		* @return float ����
		*/
		static float Dot(const Vector3f& a, const Vector3f& b);

		/**
		* @brief �O��
		* @param[in] const Vector3f&(a)
		* @param[in] const Vector3f&(b)
		* @param[out] Vector3f*(pout) �O�σx�N�g��
		* @return Vector3f �O�σx�N�g��
		*/
		static Vector3f Cross(const Vector3f& a, const Vector3f& b, Vector3f* pout = 0);


		/**
		* @brief ����
		* @param[in] const Vector3f&(a)
		* @return float ����
		*/
		static float Length(const Vector3f& a);

		/**
		* @brief ������2��
		* @param[in] const Vector3f&(a)
		* @return float ������2��
		*/
		static float LengthSq(const Vector3f& a);

		/**
		* @brief ���`��Ԋ֐�
		* @param[out] Vector3f*(out) ��ԃx�N�g��
		* @param[in] const Vector3f&(start) �J�n�x�N�g��
		* @param[in] const Vector3f&(end) �I���x�N�g��
		* @param[in] float(t) ��Ԓl�i0�`1�j
		* @return Vector3f* ��ԃx�N�g��
		*/
		static Vector3f* Linear(Vector3f* out, const Vector3f& start, const Vector3f& end, float t);

		/**
		* @brief ���ʐ��`��Ԋ֐�
		* @param[out] Vector3f*(out) ��ԃx�N�g��
		* @param[in] const Vector3f&(start) �J�n�x�N�g��
		* @param[in] const Vector3f&(end) �I���x�N�g��
		* @param[in] float(t) ��Ԓl�i0�`1�j
		* @return Vector3f* ��ԃx�N�g��
		*/
		static Vector3f* SphereLinear(Vector3f* out, const Vector3f& start, const Vector3f& end, float t);

		static const Vector3f Zero;			//! �[���x�N�g��
		static const Vector3f WorldForward;	//! �O�P�ʃx�N�g��
		static const Vector3f WorldRight;	//! �E�P�ʃx�N�g��
		static const Vector3f WorldUp;		//! ��P�ʃx�N�g��
		static const Vector3f UnitX;		//! X�P�ʃx�N�g��
		static const Vector3f UnitY;		//! Y�P�ʃx�N�g��
		static const Vector3f UnitZ;		//! Z�P�ʃx�N�g��
	};


	/**
	* @brief float��4�����x�N�g���������N���X
	* @details XMFLOAT4���p��������
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
		* @brief XMVECTOR����3�����x�N�g�����擾
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
		* @brief XMVECTOR������
		*/
		Vector4f& operator=(const DirectX::XMVECTOR& xmVector) 
		{
			DirectX::XMStoreFloat4(this, xmVector);
			return *this;
		}

		/**
		* @brief XMVECTOR�ɃL���X�g
		*/
		operator DirectX::XMVECTOR() const 
		{
			return DirectX::XMLoadFloat4(this);
		}


		/**
		* @brief �x�N�g������p�����쐬(Z��O�x�N�g���ɂ���)
		*/
		static Vector4f MakeQuaternionFromVec3(const Conv_XM::Vector3f& forward);

		/**
		* @brief �p�x����
		* @param[in] float(x) �p�xX
		* @param[in] float(y) �p�xY
		* @param[in] float(z) �p�xZ
		* @details degree�œ���
		*/
		static Vector4f MakeQuaternionFromEuler(float x, float y, float z);

		/**
		* @brief �I�C���[�p����p�����쐬
		* @details degree�œ���
		*/
		static Vector4f MakeQuaternionFromEuler(const Conv_XM::Vector3f& euler);

	};

	/**
	* @brief int��4�����x�N�g���������N���X
	* @details XMINT4���p��������
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
		* @brief XMVECTOR����3�����x�N�g�����擾
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
		* @brief XMVECTOR������
		*/
		Vector4i& operator=(const DirectX::XMVECTOR& xmVector) {
			DirectX::XMStoreSInt4(this, xmVector);
			return *this;
		}

		/**
		* @brief XMVECTOR�ɃL���X�g
		*/
		operator DirectX::XMVECTOR() const {
			return DirectX::XMLoadSInt4(this);
		}
	};

	/**
	* @brief �s��N���X
	* @details XMFLOAT4X4���p��������
	*/
	struct Matrix4x4f : public DirectX::XMFLOAT4X4
	{
		Matrix4x4f() = default;

		/**
		* @brief XMMATRIX����s��x�N�g�����擾
		*/
		Matrix4x4f(const DirectX::XMMATRIX& other)
		{
			DirectX::XMStoreFloat4x4(this, other);
		}

		/**
		* @brief XMMATRIX����L���X�g
		*/
		DirectX::XMFLOAT4X4& operator=(const DirectX::XMMATRIX& other) {
			DirectX::XMStoreFloat4x4(this, other);
			return *this;
		}

		/**
		* @brief XMMATRIX�ɑ��
		*/
		operator DirectX::XMMATRIX() const {
			return XMLoadFloat4x4(this);
		}

		/**
		* @brief ���`���
		* @param[out] Matrix4x4f*(out) ����
		* @param[in] const Matrix4x4f&(start) �J�n
		* @param[in] const Matrix4x4f&(end) �ڕW
		* @param[in] float(t) ��ԌW���i0�`1�j
		* @return Matrix4x4f* ����
		*/
		static Matrix4x4f* Linear(Matrix4x4f* out, const Matrix4x4f& start, const Matrix4x4f& end, float t);

		/**
		* @brief �X�P�[�����o��
		* @param[out] Matrix4x4f(mat) ����
		* @return Vector3 ����
		*/
		static Vector3f MatrixToScale(const Matrix4x4f& mat);

		/**
		* @brief ���ʐ��`��Ԃɂ���Ԏp���Z�o�֐�
		* @param[out] Matrix4x4f*(out) ��Ԏp��
		* @param[in] const Matrix4x4f&(start) �J�n�p��
		* @param[in] const Matrix4x4f&(end) �ڕW�p��
		* @param[in] float(t) ��ԌW���i0�`1�j
		* @return Matrix4x4f* ��Ԏp��
		*/
		static Matrix4x4f* CalcInterPause(Matrix4x4f* out, const Matrix4x4f& start, const Matrix4x4f& end, float t);
	};
}