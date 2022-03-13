/**
 * @file cost_manager.h
 * @brief �R�X�g�Ǘ�
 */

#pragma once

#include"../actor.h"

 /**
 * @brief �R�X�g�Ǘ��N���X
 */
class CostManager : public Actor
{
public:
	static void LoadTexture();

	static void UnloadTexture();
private:
	//! �����̃R�X�g
	int m_MyCost;

	//! �G�̃R�X�g
	int m_EnemyCost;

	//! �����̃Q�[�W�X�v���C�g�R���|�[�l���g
	class GaugeSpriteComponent* m_MyGaugeSpriteComp;

	//! �G�̃Q�[�W�X�v���C�g�R���|�[�l���g
	class GaugeSpriteComponent* m_EnemyGaugeSpriteComp;

	//! TEAM�X�v���C�g�R���|�[�l���g
	class SpriteComponent* m_TeamSpriteComponent;

	//! ENEMY�X�v���C�g�R���|�[�l���g
	class SpriteComponent* m_EnemySpriteComponent;

public:
	CostManager();
	~CostManager();

	/**
	* @brief �R�X�g�����ɖ߂�
	*/
	void Reset();

	/**
	* @brief �����̃R�X�g�����炷
	*/
	inline void DecreaseMyCost(int cost) { m_MyCost -= cost; }

	/**
	* @brief �G�̃R�X�g�����炷
	*/
	inline void DecreaseEnemyCost(int cost) { m_EnemyCost -= cost; }

	// �R�X�g�擾
	inline int GetMyCost() { return m_MyCost; }
	inline int GetEnemyCost() { return m_EnemyCost; }

protected:

	/**
	* @brief �R�X�g�Ǘ��̓ǂݍ��ݏ���
	*/
	virtual void LoadActor() override;

	/**
	* @brief �R�X�g�Ǘ��̏���������
	*/
	virtual void InitActor() override;

	/**
	* @brief �R�X�g�Ǘ��̍X�V����
	*/
	virtual void UpdateActor()override;
};