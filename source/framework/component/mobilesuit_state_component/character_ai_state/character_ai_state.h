/**
 * @file charcter_ai_state.h
 * @brief �L�����N�^�[AI�X�e�[�g
 */

#pragma once

 /**
 * @brief �L�����N�^�[AI�X�e�[�g�N���X
 */
class CharcterAIState
{
protected:
	//! ���L����Ă���R���|�[�l���g
	class MobileSuitAIStateComponent* m_OwnerComponent;

	//! ���L����Ă��郂�r���X�[�c�A�N�^�[
	class MobileSuit* m_MobileSuit;

	//! �u�[�X�g�R���|�[�l���g
	class BoostComponent* m_BoostComponent;

	//! �G�l�~�[001�̃|�C���^
	class MobileSuit* m_EnemyMobileSuit001;
	//! �G�l�~�[002�̃|�C���^
	class MobileSuit* m_EnemyMobileSuit002;
	//! �^�[�Q�b�g�i���o�[
	int m_TargetNum;
public:
	CharcterAIState(class MobileSuitAIStateComponent* ownerComponent)
		:m_OwnerComponent(ownerComponent)
	{}

	virtual ~CharcterAIState() {}

	/**
	* @brief ����������
	*/
	virtual void Init() = 0;

	/**
	* @brief ���͏���
	*/
	virtual void ProcessInput() = 0;

	/**
	* @brief �X�V����
	*/
	virtual void Update() = 0;

	/**
	* @brief ��Ԃɓ��������̏���
	*/
	virtual void OnEnter() = 0;

	/**
	* @brief ��Ԃ���o��Ƃ��̏���
	*/
	virtual void OnExit() = 0;

	/**
	* @brief ��Ԗ�
	*/
	virtual const char* GetName() const = 0;

	/**
	* @brief �e����R���|�[�l���g���擾����
	* @return �����������ǂ���
	*/
	bool GetData();
};

