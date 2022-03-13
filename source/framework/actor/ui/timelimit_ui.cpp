
#include"../../../system/main.h"
#include "timelimit_ui.h"
#include"../../component/sprite_component/number_sprite_component.h"


namespace TimeLimitUIData
{
	//! �������̌���
	static constexpr int k_IntegerDigit = 3;

	//! �������̌���
	static constexpr int k_DecimalDigit = 2;

	//! ���[�̏ꏊ
	static const Conv_XM::Vector2f k_LeftPosition = Conv_XM::Vector2f(750.0f, 30.0f);

	//! �������̐�����̃T�C�Y
	static const Conv_XM::Vector2f k_IntegerSize = Conv_XM::Vector2f(50.0f, 50.0f);

	//! �������̐�����̃T�C�Y
	static const Conv_XM::Vector2f k_DecimalSize = Conv_XM::Vector2f(30.0f, 30.0f);

	//! �ő�̎���
	static constexpr float k_MaxTime = 240.0f;
}

TimeLimitUI::TimeLimitUI()
	: m_MaxTime(TimeLimitUIData::k_MaxTime)
	, m_TimeLeft(0)
	, m_IsMesureStart(false)
{
}

TimeLimitUI::~TimeLimitUI()
{
}

void TimeLimitUI::LoadActor()
{
	for (int i = 0; i < TimeLimitUIData::k_IntegerDigit + TimeLimitUIData::k_DecimalDigit; i++)
	{
		m_NumberSpriteComponent[i] = CreateComponent<NumberSpriteComponent>();
	}
}

void TimeLimitUI::InitActor()
{
	// null�`�F�b�N
	for (int i = 0; i < TimeLimitUIData::k_IntegerDigit + TimeLimitUIData::k_DecimalDigit; i++)
	{
		if (m_NumberSpriteComponent[i] == nullptr)
		{
			Logger::GetInstance().SetLog("TimeLimitUI::InitActor m_NumberSpriteComponent��nullptr");
			return;
		}
	}


	// �X�v���C�g�f�[�^���Z�b�g
	for (int i = 0; i < TimeLimitUIData::k_IntegerDigit + TimeLimitUIData::k_DecimalDigit; i++)
	{
		if (i < TimeLimitUIData::k_IntegerDigit)
		{
			m_NumberSpriteComponent[i]->SetPosition(Conv_XM::Vector2f(TimeLimitUIData::k_LeftPosition.x + TimeLimitUIData::k_IntegerSize.x * i, TimeLimitUIData::k_LeftPosition.y));
			m_NumberSpriteComponent[i]->SetSize(TimeLimitUIData::k_IntegerSize);
		}
		else
		{
			m_NumberSpriteComponent[i]->SetPosition(Conv_XM::Vector2f(TimeLimitUIData::k_LeftPosition.x + TimeLimitUIData::k_IntegerSize.x * TimeLimitUIData::k_IntegerDigit + TimeLimitUIData::k_DecimalSize.x * (i - TimeLimitUIData::k_IntegerDigit), TimeLimitUIData::k_LeftPosition.y));
			m_NumberSpriteComponent[i]->SetSize(TimeLimitUIData::k_DecimalSize);
		}
	}
}

void TimeLimitUI::UpdateActor()
{
	// null�`�F�b�N
	for (int i = 0; i < TimeLimitUIData::k_IntegerDigit + TimeLimitUIData::k_DecimalDigit; i++)
	{
		if (m_NumberSpriteComponent[i] == nullptr)
		{
			Logger::GetInstance().SetLog("TimeLimitUI::InitActor m_NumberSpriteComponent��nullptr");
			return;
		}
	}

	// �v���J�n�O�Ȃ�Œ�
	if (m_IsMesureStart == false)
	{
		m_Start = std::chrono::system_clock::now();
	}

	// ���Ԏ擾
	std::chrono::system_clock::time_point currentTime = std::chrono::system_clock::now();

	float elapsed = static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - m_Start).count()); //�����ɗv�������Ԃ��~���b�ɕϊ�
	float secElapsed = elapsed / 1000.0f;
	m_TimeLeft = m_MaxTime - secElapsed;

	int hundDigit = static_cast<int>(m_TimeLeft / 100);
	m_NumberSpriteComponent[0]->SetNumber(hundDigit);

	int tenDigit = static_cast<int>((m_TimeLeft / 10)) % 10;
	m_NumberSpriteComponent[1]->SetNumber(tenDigit);

	int oneDigit = static_cast<int>(m_TimeLeft) % 10;
	m_NumberSpriteComponent[2]->SetNumber(oneDigit);

	int firstDecimalPlace = static_cast<int>(m_TimeLeft * 10) % 10;
	m_NumberSpriteComponent[3]->SetNumber(firstDecimalPlace);

	int secondDecimalPlace = static_cast<int>(m_TimeLeft * 100) % 10;
	m_NumberSpriteComponent[4]->SetNumber(secondDecimalPlace);
}
