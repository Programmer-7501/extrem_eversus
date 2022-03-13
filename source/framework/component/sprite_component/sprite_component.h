/**
 * @file sprite_component.h
 * @brief �X�v���C�g�R���|�[�l���g
 */

#pragma once

#include"../component.h"

 /**
 * @brief �X�v���C�g�R���|�[�l���g
 * @details �ݒ��r���ŕύX�������̂Ȃ��X�v���C�g�Ɏg�p
 */
class SpriteComponent : public Component
{
public:
	struct SpriteTextureConfig
	{
		Conv_XM::Vector2f TexCut;//�e�N�X�`���؂�����W
		Conv_XM::Vector2f TexCutSize;//�e�N�X�`���؂���T�C�Y
	};

	enum DrawNumber
	{
		DM_DEFAULT = 100,
	};
protected:
	//! ���_�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer;
	//! �C���f�b�N�X�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_IndexBuffer;

	//! �`�揇��
	int m_DrawOrder;

	//! �`�悷�邩�ǂ���
	bool m_Visible;

	//! �e�N�X�`�����
	SpriteTextureConfig m_SpriteTextureConfig;

	//! �X�v���C�g�̃|�W�V����
	Conv_XM::Vector2f m_Position;
	//! �X�v���C�g�̃T�C�Y
	Conv_XM::Vector2f m_Size;

	//! �f�o�C�X�R���e�L�X�g
	ID3D11DeviceContext* m_DeviceContext;
	//! ���_�V�F�[�_�[
	ID3D11VertexShader* m_VertexShader;
	//! �s�N�Z���V�F�[�_�[�V�F�[�_�[
	ID3D11PixelShader* m_PixelShader;
	//! �C���v�b�g���C�A�E�g
	ID3D11InputLayout* m_InputLayout;
	//! �e�N�X�`��
	ID3D11ShaderResourceView* m_ShaderResourceView;
public:
	SpriteComponent(class Actor* owner);
	virtual ~SpriteComponent()override;

	/**
	* @brief ����������
	*/
	virtual void Init()override;

	/**
	* @brief �I������
	*/
	virtual void Uninit()override;

	/**
	* @brief �`�揈��
	*/
	virtual void Draw() const;

	// �Z�b�^�[�A�Q�b�^�[

	inline void SetVisible(bool flag) { m_Visible = flag; }

	inline void SetPosition(const Conv_XM::Vector2f& position) { m_Position = position; }
	inline void SetSize(const Conv_XM::Vector2f& size) { m_Size = size; }

	inline int GetDrawOrder() const { return m_DrawOrder; }

	// ����������ActorInit�܂łɐݒ肵�Ȃ��Ɩ���

	inline void SetDrawOrder(int order) { m_DrawOrder = order; }

	inline void SetSRV(ID3D11ShaderResourceView* srv) { m_ShaderResourceView = srv; }

	inline void SetSpriteTextureConfig(const SpriteTextureConfig& spriteTextureConfig) { m_SpriteTextureConfig = spriteTextureConfig; }

protected:
	/**
	* @brief �f�o�C�X�R���e�L�X�g�̎擾
	* @return bool �����������ǂ���
	*/
	bool GetDeviceContext();

private:


	/**
	* @brief �V�F�[�_�[�̎擾
	* @return bool �����������ǂ���
	*/
	virtual bool GetShader();
};