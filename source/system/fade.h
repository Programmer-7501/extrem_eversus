#pragma once

class Fade
{
public:
	enum FadeMode
	{
		FADE_NONE,
		FADE_IN,
		FADE_OUT,
	};

private:
	//! �A�N�e�B�u�t���O
	bool m_Active;
	//! ���݂̃t���[��
	int m_CurrentFrame;
	//! �t�F�[�h����
	int m_FadeTime;
	//! �A���t�@�l
	float m_Arufa;
	FadeMode m_FadeState;

	//! ���_�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer;
	//! �C���f�b�N�X�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_IndexBuffer;

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

	//! �F
	Conv_XM::Vector3f m_Color;
public:
	Fade();
	~Fade();

	/**
	* @brief ����������
	*/
	void Init();

	/**
	* @brief �X�V����
	*/
	void Update();

	/**
	* @brief �`�揈��
	*/
	void Draw();

	bool GetActive() { return m_Active; }
	void SetFade(FadeMode state, int fadetime);

	inline void SetSRV(ID3D11ShaderResourceView* srv) { m_ShaderResourceView = srv; }
};