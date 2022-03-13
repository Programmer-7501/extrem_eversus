
#include"../../../system/main.h"
#include "life_ui.h"
#include"../../component/sprite_component/number_sprite_component.h"
#include"../../component/mobilesuit_component/life_component.h"
#include"../../actor/mobilesuit/mobilesuit.h"

namespace LifeUIData
{
	//! ��ԍ��̐����̏ꏊ
	static const Conv_XM::Vector2f k_LeftPosition = Conv_XM::Vector2f(50.0f, 400.0f);

	//! ������̃T�C�Y
	static const Conv_XM::Vector2f k_Size = Conv_XM::Vector2f(100.0f, 100.0f);

	//! ���C�t�������ȏ�̎��̐F
	const Conv_XM::Vector4f k_DefaultLifeColor = Conv_XM::Vector4f(1.0f, 1.0f, 1.0f, 1.0f);

	//! ���C�t�������ȉ��̎��̐F
	const Conv_XM::Vector4f k_HalfLifeColor = Conv_XM::Vector4f(1.0f, 1.0f, 0.0f, 1.0f);

	//! ���C�t��1/4�̎��̐F
	const Conv_XM::Vector4f k_QuarterLifeColor = Conv_XM::Vector4f(1.0f, 0.0f, 0.0f, 1.0f);
}

LifeUI::LifeUI()
	:m_LifeComponent(nullptr)
{
}

LifeUI::~LifeUI()
{
}

void LifeUI::LoadActor()
{
	for (int i = 0; i < LifeUIData::k_Digit; i++)
	{
		m_NumberSpriteComponent[i] = CreateComponent<NumberSpriteComponent>();
	}
}

void LifeUI::InitActor()
{

	for (int i = 0; i < LifeUIData::k_Digit; i++)
	{
		// null�`�F�b�N
		if (m_NumberSpriteComponent[i] == nullptr)
		{
			Logger::GetInstance().SetLog("TimeLimitUI::InitActor m_NumberSpriteComponent��nullptr");
			return;
		}

		// �X�v���C�g�f�[�^���Z�b�g
		m_NumberSpriteComponent[i]->SetPosition(Conv_XM::Vector2f(LifeUIData::k_LeftPosition.x + LifeUIData::k_Size.x * i, LifeUIData::k_LeftPosition.y));
		m_NumberSpriteComponent[i]->SetSize(LifeUIData::k_Size);
	}
	if (m_MobileSuit == nullptr)
	{
		Logger::GetInstance().SetLog("TimeLimitUI::InitActor m_MobileSuit��nullptr");
		return;
	}

	m_LifeComponent = m_MobileSuit->GetComponent<LifeComponent>();
}

void LifeUI::UpdateActor()
{
	// null�`�F�b�N
	for (int i = 0; i < LifeUIData::k_Digit; i++)
	{
		if (m_NumberSpriteComponent[i] == nullptr)
		{
			Logger::GetInstance().SetLog("TimeLimitUI::InitActor m_NumberSpriteComponent��nullptr");
			return;
		}
	}
	if (m_LifeComponent == nullptr)
	{
		Logger::GetInstance().SetLog("LifeUI::UpdateActor nullptr");
		return;
	}

	float life = m_LifeComponent->GetLife();

	int hundDigit = static_cast<int>(life / 100);
	m_NumberSpriteComponent[0]->SetNumber(hundDigit);

	int tenDigit = static_cast<int>((life / 10)) % 10;
	m_NumberSpriteComponent[1]->SetNumber(tenDigit);

	int oneDigit = static_cast<int>(life) % 10;
	m_NumberSpriteComponent[2]->SetNumber(oneDigit);

	float lifeRatio = m_LifeComponent->GetLifeRatio();

	if (lifeRatio < 0.25f)
	{
		for (int i = 0; i < LifeUIData::k_Digit; i++)
		{
			m_NumberSpriteComponent[i]->SetColor(LifeUIData::k_QuarterLifeColor);
		}
	}
	else if (lifeRatio < 0.5f)
	{
		for (int i = 0; i < LifeUIData::k_Digit; i++)
		{
			m_NumberSpriteComponent[i]->SetColor(LifeUIData::k_HalfLifeColor);
		}
	}
	else
	{
		for (int i = 0; i < LifeUIData::k_Digit; i++)
		{
			m_NumberSpriteComponent[i]->SetColor(LifeUIData::k_DefaultLifeColor);
		}
	}
}
