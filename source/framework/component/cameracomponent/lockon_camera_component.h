/**
 * @file lockon_camera_component.h
 * @brief ���b�N�I���J�����R���|�[�l���g
 */

#pragma once

#include"camera_component.h"

 /**
 * @brief ���b�N�I���J�����R���|�[�l���g
 */
class LockOnCameraComponent : public CameraComponent
{
private:
	//! ���b�N�I���Ώ�
	class Actor* m_TargetActor;

	//! �J�����̎p���N�H�[�^�j�I��
	Conv_XM::Vector4f m_Quaternion;

	//! �J�����̑O�x�N�g��
	Conv_XM::Vector3f m_Forward;

	//! �J�����̉E�x�N�g��
	Conv_XM::Vector3f m_Right;

	//! �����_�𓮂����Ȃ��t���O
	bool m_IsNotTargetMoveFlag;

	//! �^�[�Q�b�g�̃|�W�V������ۑ�����ϐ�
	Conv_XM::Vector3f m_StoreTargetPos;
public:
	LockOnCameraComponent(class Actor* owner);
	~LockOnCameraComponent();

	/**
	* @brief �X�V����
	*/
	virtual void Update();

	inline void SetTarget(class Actor* actor) { m_TargetActor = actor; }

	inline Conv_XM::Vector3f GetForward() const { return m_Forward; }

	inline Conv_XM::Vector3f GetRight() const { return m_Right; }

	void SetNotMoveFlag(bool flag);
};