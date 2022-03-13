/**
 * @file game_manager.h
 * @brief �Q�[���Ǘ�
 */

#pragma once

#include"../actor.h"

 /**
 * @brief �Q�[���Ǘ��N���X
 */
class GameManager : public Actor
{
public:

	enum SceneChange
	{
		SCENE_CHANGE_NONE,
		SCENE_CHANGE_WIN,
		SCENE_CHANGE_LOSE,
	};

private:
	enum IdentificationNumber
	{
		PLAYER,
		ALIEN,
		ENEMY001,
		ENEMY002,
		MAXMOBILESUIT
	};

public:

	/**
	* @brief �e�N�X�`���ǂݍ���
	*/
	static void LoadTexture();

	/**
	* @brief �e�N�X�`���j��
	*/
	static void UnloadTexture();

private:
	//! �^�C�����~�b�gUI
	class TimeLimitUI* m_TimeLimitUI;

	//! �R�X�g�}�l�[�W���[
	class CostManager* m_CostManager;

	//! ���r���X�[�c
	class MobileSuit* m_MobileSuit[MAXMOBILESUIT];

	//! ���X�|�[���Ǘ��^�C��
	int m_RespawnTime[MAXMOBILESUIT];

	//! �J�E���g�t���[��
	int m_CountFrame;

	//! �J�E���g�_�E���p�X�v���C�g
	class NumberSpriteComponent* m_CountDownSpriteComponent;

	//! standby�X�v���C�g
	class SpriteComponent* m_StandbySpriteComponent;

	//! go�X�v���C�g
	class SpriteComponent* m_GoSpriteComponent;

	//! �V�[���؂�ւ��p
	SceneChange m_SceneChange;
public:
	GameManager();
	~GameManager();

	inline void SetParameter
	(
		class TimeLimitUI* timeLimitUI,
		class CostManager* costManager,
		class MobileSuit* player,
		class MobileSuit* alliens,
		class MobileSuit* enemy001,
		class MobileSuit* enemy002
		)
	{
		m_TimeLimitUI = timeLimitUI;
		m_CostManager = costManager;
		m_MobileSuit[PLAYER] = player;
		m_MobileSuit[ALIEN] = alliens;
		m_MobileSuit[ENEMY001] = enemy001;
		m_MobileSuit[ENEMY002] = enemy002;
	}

	inline SceneChange GetSceneChange() { return m_SceneChange; }

protected:

	/**
	* @brief �Q�[���̓ǂݍ��ݏ���
	*/
	virtual void LoadActor() override;

	/**
	* @brief �Q�[���̏���������
	*/
	virtual void InitActor() override;

	/**
	* @brief �Q�[���̍X�V����
	*/
	virtual void UpdateActor()override;

private:

	/**
	* @brief �Q�[���̊J�n�̊Ǘ�
	*/
	void GameStartManager();

	/**
	* @brief ���X�|�[���̊Ǘ�
	*/
	void RespawnManager();
};