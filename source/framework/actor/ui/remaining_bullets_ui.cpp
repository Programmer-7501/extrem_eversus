
#include"../../../system/main.h"
#include "remaining_bullets_ui.h"
#include"../../actor_manager/bullet_manager.h"
#include"../../component/sprite_component/number_sprite_component.h"

namespace RemaingBulletsUIData
{
	//! �����̃f�t�H���g�̃T�C�Y
	static const Conv_XM::Vector2f k_NumberDefaultSize = Conv_XM::Vector2f(50.0f, 50.0f);

	//! �����̃f�t�H���g�̃|�W�V����
	static const Conv_XM::Vector2f k_NumberDefaultLeftPosition = Conv_XM::Vector2f(800.0f, 300.0f);
}

RemaingBulletsUI::RemaingBulletsUI()
	:m_NumberSpriteComponent{}
	, m_BulletManager(nullptr)
	, m_LeftPosition(RemaingBulletsUIData::k_NumberDefaultLeftPosition)
	, m_Digit(0)
{
}

RemaingBulletsUI::~RemaingBulletsUI()
{
}

void RemaingBulletsUI::LoadActor()
{
	for (int i = 0; i < RemaingBulletsUIData::k_MaxDigit; i++)
	{
		m_NumberSpriteComponent[i] = CreateComponent<NumberSpriteComponent>();
	}
}

void RemaingBulletsUI::InitActor()
{
	if (m_BulletManager == nullptr)
	{
		Logger::GetInstance().SetLog("RemaingBulletsUI::InitActor nullptr");
		return;
	}

	// �e�̏���l�擾
	int maxBullet = m_BulletManager->GetMaxBullet();
	// �ő包���擾
	m_Digit = Math::GetDigit(maxBullet);

	for (int i = 0; i < m_Digit; i++)
	{
		if (m_NumberSpriteComponent[i] == nullptr)
		{
			Logger::GetInstance().SetLog("RemaingBulletsUI::InitActor nullptr");
			return;
		}
		m_NumberSpriteComponent[i]->SetPosition(Conv_XM::Vector2f(m_LeftPosition.x + RemaingBulletsUIData::k_NumberDefaultSize.x * i, m_LeftPosition.y));
		m_NumberSpriteComponent[i]->SetSize(RemaingBulletsUIData::k_NumberDefaultSize);
	}
}

void RemaingBulletsUI::UpdateActor()
{
	if (m_BulletManager == nullptr)
	{
		Logger::GetInstance().SetLog("RemaingBulletsUI::InitActor nullptr");
		return;
	}

	// ���݂̎c�e�����擾
	int currentBulletNum = m_BulletManager->GetCurrentBullet();

	// �e���̐�������
	for (int i = 0; i < m_Digit; i++)
	{
		int num = currentBulletNum % 10;
		int index = m_Digit - 1 - i;
		if (m_NumberSpriteComponent[index] == nullptr)
		{
			Logger::GetInstance().SetLog("RemaingBulletsUI::InitActor nullptr");
			return;
		}
		m_NumberSpriteComponent[index]->SetNumber(num);
		currentBulletNum = currentBulletNum / 10;
	}
}
