/**
* @file singleton.h
* @brief �V���O���g���e���v���[�g�N���X
*/

#pragma once

#include<mutex>

class SingletonFinalizer {
public:
	using FinalizerFunc = void(*)();
	static void addFinalizer(FinalizerFunc func);
	static void Finalize();
};

/**
* @brief �V���O���g���N���X
* @details ����̌p����ɃV���O���g���̐U�镑����������
* @details ����̌p����̃R���X�g���N�^��private�ɂ��A�����friend�N���X�ɂ��邱��
*/
template<typename T>
class Singleton
{
private:
	static T* m_pInstance;
	static std::mutex m_Mutex;
	static std::once_flag initFlag;

protected:
	Singleton() = default;
	~Singleton() = default;

public:

	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	Singleton(Singleton&&) = delete;
	Singleton& operator=(Singleton&&) = delete;

	/**
	* @brief �C���X�^���X�擾�֐�
	* @return Singleton* �N���X�C���X�^���X
	* @details �V���O���g����\���֐�
	*/
	static T& GetInstance()
	{
		// ��x��������������
		std::call_once(initFlag, Create);
		//null�`�F�b�N
		assert(m_pInstance);
		return *m_pInstance;
	}

private:
	/**
	* @brief �C���X�^���X�쐬�֐�
	*/
	static void Create()
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new T;
			SingletonFinalizer::addFinalizer(&Singleton<T>::Destroy);
		}
	}

	/**
	* @brief �C���X�^���X�j���֐�
	*/
	static void Destroy()
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
};

template<typename T>
std::mutex Singleton<T>::m_Mutex;
template<typename T>
std::once_flag Singleton<T>::initFlag;
template<typename T>
T* Singleton<T>::m_pInstance = nullptr;
