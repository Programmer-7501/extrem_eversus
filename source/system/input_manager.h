/**
* @file input_manager.h
* @brief ���͐���
*/

#pragma once

#include"../function/singleton.h"

namespace InputManagerData
{
	//! ���͂��L������t���[��
	static constexpr int k_InputKeepFrame = 14;
}

/**
* @namespace InputManager
* @brief ���͏��N���X
* @details �Q�[���p�b�h���ڑ�����Ă�����L�[���͖͂��������
*/
class InputManager final : public Singleton<InputManager>
{
public:
	enum InputName
	{
		//! ����
		IN_DECIDE,
		//! �߂�
		IN_RETURN,
		//! �O����
		IN_FRONT,
		//! ������
		IN_BACK,
		//! ������
		IN_LEFT,
		//! �E����
		IN_RIGHT,
		//! �����
		IN_UP,
		//! ������
		IN_DOWN,
		//! �W�����v����
		IN_JUMP,
		//! ���C������
		IN_MAIN,
		//! ����ˌ�����
		IN_SPECIALSHOT,
		//! �i������
		IN_KAKUTOU,
		IN_TURN_LEFT,
		IN_TURN_RIGHT,
		IN_TURN_UP,
		IN_TURN_DOWN,
		//! ���X�e�b�N����͂��Ă��邩�ǂ���
		IN_LEFT_STICK,
		//! �ő吔
		IN_MAX
	};
private:

	//unsigned char  //8�ȉ�
	//unsigned short //16�ȉ�
	//unsigned long  //32�ȉ�
	//! �Q�[���p�b�h���q����Ă��邩�ǂ���
	typedef unsigned long KeyStorageType;


	//! ���͕ۑ��ϐ�
	KeyStorageType m_KeepState[InputManagerData::k_InputKeepFrame] = {};
	//! ���͕ۑ��p�C���N�������g�ϐ�
	int m_KeepIncrement = 0;

	//! ���݂̓��͏��
	KeyStorageType m_CurrentMobileSuitState = 0;
	//! �O��̓��͏��
	KeyStorageType m_PrevState = 0;
	//! �g���K�[
	KeyStorageType m_Trigger = 0;
	//! �����[�X
	KeyStorageType m_Release = 0;

	//! �Q�[���p�b�h���q����Ă��邩�ǂ���
	DWORD m_Is_GamePad_Use = 0;

	//! �Q�[���p�b�h���͏��
	XINPUT_STATE m_State = {};

	//! �L�[�{�[�h�̓��͊i�[�ϐ�1
	BYTE m_InputByteBuffer[256];
private:
	friend class Singleton<InputManager>;

	InputManager() = default;
	~InputManager() = default;

public:
	/**
	* @brief ����������
	*/
	void Init();

	/**
	* @brief �I����������
	*/
	void Uninit();

	/**
	* @brief �X�V����
	*/
	void Update();

	/**
	* @brief �Q�[���p�b�h���g���Ă��邩�ǂ���
	* @return bool �Q�[���p�b�h���g���Ă�����true
	*/
	bool GetIsUseGamePad();

	/**
	* @brief �g���K�[�擾
	* @param[in] InputName(inputName) ���͖�
	* @return bool �g���K�[���
	*/
	bool GetIsTrigger(InputName inputName);

	/**
	* @brief ���͏�Ԏ擾
	* @param[in] InputName(inputName) ���͖�
	* @return bool ���͂���Ă��邩�ǂ���
	*/
	bool GetIsPress(InputName inputName);

	/**
	* @brief �����[�X�擾
	* @param[in] InputName(inputName) ���͖�
	* @return bool �����ꂽ���ǂ���
	*/
	bool GetIsRelease(InputName inputName);

	/**
	* @brief �ۑ��L�[�̎擾
	* @param[in] InputName(inputName) ���͖�
	* @param[in] int(prevFrame) ���͖�
	* @return bool ���͂���Ă��邩�ǂ���
	*/
	bool GetIsKeepState(InputName inputName, int prevFrame);


	/**
	* @brief ���X�e�b�N��X���W���擾
	* @return SHORT ���X�e�b�N��X���W�̒l
	*/
	SHORT GetLeftStickX();

	/**
	* @brief ���X�e�b�N��Y���W���擾
	* @return SHORT ���X�e�b�N��Y���W�̒l
	*/
	SHORT GetLeftStickY();

	/**
	* @brief �E�X�e�b�N��X���W���擾
	* @return SHORT �E�X�e�b�N��X���W�̒l
	*/
	SHORT GetRightStickX();

	/**
	* @brief �E�X�e�b�N��Y���W���擾
	* @return SHORT �E�X�e�b�N��Y���W�̒l
	*/
	SHORT GetRightStickY();

	/**
	* @brief ���X�e�b�N�̃x�N�g�����擾
	* @return Conv_XM::Vector2 �x�N�g��
	*/
	Conv_XM::Vector2f GetLeftStickeVector();

	/**
	* @brief �E�X�e�b�N�̃x�N�g�����擾
	* @return Conv_XM::Vector2 �x�N�g��
	*/
	Conv_XM::Vector2f GetRightStickeVector();

	/**
	* @brief ���X�e�b�N�ɓ��͂����邩�ǂ���
	* @return bool ���͂���Ă��邩�ǂ���
	*/
	bool GetLeftStickIsInput();

	/**
	* @brief �E�X�e�b�N�ɓ��͂����邩�ǂ���
	* @return bool ���͂���Ă��邩�ǂ���
	*/
	bool GetRightStickIsInput();
};