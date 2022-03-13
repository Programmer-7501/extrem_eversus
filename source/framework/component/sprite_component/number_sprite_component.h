/**
 * @file number_sprite_component.h
 * @brief �i���o�[�X�v���C�g�R���|�[�l���g
 */

#pragma once

#include"changeable_sprite_component.h"

 /**
 * @brief �i���o�[�X�v���C�g�R���|�[�l���g
 * @details 0����9�܂őΉ�
 */
class NumberSpriteComponent : public ChangeableSpriteComponent
{
public:
	//! �g�p���鐔���e�N�X�`��
	static ID3D11ShaderResourceView* m_NumberTexture;

	/**
	* @brief �����e�N�X�`���̓ǂݍ���
	*/
	static void LoadNumberTexture();

	/**
	* @brief �����e�N�X�`���̔j��
	*/
	static void UnloadNumberTexture();
private:
	//! �\�����鐔��
	int m_DrawNumber;
public:
	NumberSpriteComponent(class Actor* owner);
	virtual ~NumberSpriteComponent()override;

	/**
	* @brief �\�����鐔���Z�b�g
	*/
	void SetNumber(int num);
};