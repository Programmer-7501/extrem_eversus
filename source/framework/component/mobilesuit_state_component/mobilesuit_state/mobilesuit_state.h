/**
 * @file state.h
 * @brief �X�e�[�g
 */

#pragma once

 /**
 * @brief �X�e�[�g�N���X
 */
class MobileSuitState
{
protected:
	//! ����
	enum Direction
	{
		// �O
		FORWARD,
		// ���
		BACK,
		// �E
		RIGHT,
		// ��
		LEFT
	};
protected:
	//! ���L����Ă���R���|�[�l���g
	class MobileSuitStateComponent* m_Owner;

	//! ���W�b�h�{�f�B�R���|�[�l���g
	class RigidBodyComponent* m_RigidBodyComponent;

	//! ���b�N�I���J�����R���|�[�l���g
	class LockOnCameraComponent* m_LockOnCameraComponent;

	//! �u�[�X�g�R���|�[�l���g
	class BoostComponent* m_BoostComponent;

	//! �A�j���[�V�����R���|�[�l���g
	class AnimationModelComponent* m_AnimationComponent;

	//! ���L����Ă��郂�r���X�[�c�A�N�^�[
	class MobileSuit* m_MobileSuit;

	//! �r�[�����C�t���o���b�g�}�l�[�W���[
	class BulletManager* m_BeamRifleBulletManager;

	//! �L���m���o���b�g�}�l�[�W���[
	class BulletManager* m_CannonBulletManager;

	//! �o�Y�[�J�o���b�g�}�l�[�W���[
	class BulletManager* m_BazookaBulletManager;

	//! �G�l�~�[001�̃|�C���^
	class MobileSuit* m_EnemyMobileSuit001;
	//! �G�l�~�[002�̃|�C���^
	class MobileSuit* m_EnemyMobileSuit002;
	//! �^�[�Q�b�g�i���o�[
	int m_TargetNum;

	//! �r�[���T�[�x��
	class BeamSaber* m_BeamSaber;
public:
	MobileSuitState(class MobileSuitStateComponent* ownerComponent);

	virtual ~MobileSuitState(){}

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

protected:

	/**
	* @brief �e����R���|�[�l���g���擾����
	* @return �����������ǂ���
	*/
	bool GetData();

	/**
	* @brief �A�N�^�[����]������
	* @param[in] const Vector3f&(cameraForward) �J�����̑O�x�N�g��
	* @param[in] const Vector3f&(actorForward) ���g�̑O�x�N�g��
	* @param[in] Direction(direction) �����������p
	* @param[in] float(rotationLimit) ��]���(degree)
	*/
	void RotationActor(const Conv_XM::Vector3f& cameraForward, const Conv_XM::Vector3f& actorForward, Direction direction, float rotationLimit = 0.0f);

	/**
	* @brief �A�N�^�[����]������
	* @param[in] const Vector3f&(cameraForward) �J�����̑O�x�N�g��
	* @param[in] const Vector3f&(actorForward) ���g�̑O�x�N�g��
	* @param[in] Direction(direction) �����������p
	* @param[in] float(rotationLimit) ��]���(degree)
	*/
	void RotationActor(const Conv_XM::Vector3f& cameraForward, const Conv_XM::Vector3f& actorForward, const Conv_XM::Vector2f& direction, float rotationLimit = 0.0f);

	/**
	* @brief �G�l�~�[�Ɏ�����������
	*/
	void RotateToEnemy();
};

