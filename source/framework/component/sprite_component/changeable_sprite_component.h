/**
 * @file changeable_color_sprite_component.h
 * @brief �ύX�\�X�v���C�g�R���|�[�l���g
 */

#pragma once

#include"sprite_component.h"

 /**
 * @brief �ύX�\�X�v���C�g�R���|�[�l���g
  * @details �ݒ���X�V���ɕύX����K�v�̂���X�v���C�g�Ɏg�p
 */
class ChangeableSpriteComponent : public SpriteComponent
{
protected:
	//! �F
	Conv_XM::Vector4f m_Color;
	//! �X�V�t���O
	bool m_IsUpdate;
public:
	ChangeableSpriteComponent(class Actor* owner);
	virtual ~ChangeableSpriteComponent()override;

	// �Z�b�^�[�A�Q�b�^�[
	void SetColor(const Conv_XM::Vector4f color);

protected:
	/**
	* @brief ����������
	*/
	virtual void Init()override;

	/**
	* @brief �I������
	*/
	virtual void Uninit()override;

	/**
	* @brief �X�V����
	*/
	virtual void Update()override;

	/**
	* @brief �`�揈��
	*/
	virtual void Draw() const override;

private:

	/**
	* @brief ���_�\���̂̍X�V
	*/
	void UpdateVetexData();

	/**
	* @brief �V�F�[�_�[�̎擾
	* @return bool �����������ǂ���
	*/
	bool GetShader() override;
};