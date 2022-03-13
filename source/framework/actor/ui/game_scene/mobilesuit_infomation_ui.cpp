
#include"../../../../system/main.h"
#include"../../../../directx/resource/texture_manager.h"
#include "mobilesuit_infomation_ui.h"
#include"../../../component/mesh_component/billboard_component.h"
#include"../../../component/mesh_component/gauge_billboard_component.h"
#include"../../../component/mobilesuit_component/life_component.h"
#include"../../mobilesuit/mobilesuit.h"

namespace MobileSuitInfomationUIData
{
	//! �f�t�H���g�̃l�[���^�O�̃t�@�C����
	static const char* k_Enemy001NameTagFileName = "asset/texture/game_scene_nametag_001.png";

	//! �f�t�H���g�̃l�[���^�O�̃t�@�C����
	static const char* k_Enemy002NameTagFileName = "asset/texture/game_scene_nametag_002.png";

	//! �f�t�H���g�̃l�[���^�O�̃t�@�C����
	static const char* k_AllyNameTagFileName = "asset/texture/game_scene_nametag_003.png";

	//! �f�t�H���g�̃l�[���^�O�̃I�t�Z�b�g�|�W�V����
	static const Conv_XM::Vector3f k_DefaultOffsetNameTagPosition = Conv_XM::Vector3f(0.0f, 5.0f, 0.0f);

	//! �f�t�H���g�̃l�[���^�O�̃T�C�Y
	static const Conv_XM::Vector2f k_DefaultNameTagSize = Conv_XM::Vector2f(2.0f, 1.0f);


	//! ���C�t�Q�[�W�p�̃e�N�X�`���̃t�@�C����
	static const char* k_LifeGaugeFileName = "asset/texture/white.png";

	//! ���C�t�Q�[�W�̃I�t�Z�b�g�|�W�V����
	static const Conv_XM::Vector3f k_OffsetLifeGaugePosition = Conv_XM::Vector3f(0.0f, 3.0f, 0.0f);

	//! ���C�t�Q�[�W�̍ő�T�C�Y
	static const Conv_XM::Vector2f k_LifeGaugeMaxSize = Conv_XM::Vector2f(2.0f, 1.0f);
}

void MobileSuitInfomationUI::LoadTexture()
{
	//�@�e�N�X�`���}�l�[�W���[�擾
	TextureManager& textureManager = TextureManager::GetInstance();

	textureManager.Load(MobileSuitInfomationUIData::k_Enemy001NameTagFileName);
	textureManager.Load(MobileSuitInfomationUIData::k_Enemy002NameTagFileName);
	textureManager.Load(MobileSuitInfomationUIData::k_AllyNameTagFileName);
	textureManager.Load(MobileSuitInfomationUIData::k_LifeGaugeFileName);
}

void MobileSuitInfomationUI::UnloadTexture()
{
	//�@�e�N�X�`���}�l�[�W���[�擾
	TextureManager& textureManager = TextureManager::GetInstance();

	textureManager.Unload(MobileSuitInfomationUIData::k_Enemy001NameTagFileName);
	textureManager.Unload(MobileSuitInfomationUIData::k_Enemy002NameTagFileName);
	textureManager.Unload(MobileSuitInfomationUIData::k_AllyNameTagFileName);
	textureManager.Unload(MobileSuitInfomationUIData::k_LifeGaugeFileName);
}

MobileSuitInfomationUI::MobileSuitInfomationUI()
	: m_ParentActor(nullptr)
	, m_LifeComponent(nullptr)
	, m_NameTagBillBoardComponent(nullptr)
	, m_LifeGaugeBillBoardComponent(nullptr)
{
}

MobileSuitInfomationUI::~MobileSuitInfomationUI()
{
}

void MobileSuitInfomationUI::LoadActor()
{
	// �r���{�[�h�R���|�[�l���g�쐬
	m_NameTagBillBoardComponent = CreateComponent<BillBoardComponent>();

	// �Q�[�W�r���{�[�h�R���|�[�l���g�쐬
	m_LifeGaugeBillBoardComponent = CreateComponent<GaugeBillBoardComponent>();
}

void MobileSuitInfomationUI::InitActor()
{
	TextureManager& textureManager = TextureManager::GetInstance();

	//�@�e�N�X�`���擾
	ID3D11ShaderResourceView* nameTagSRV = nullptr;
	switch (m_MobileSuitName)
	{
	case MobileSuitInfomationUI::ENEMY001:
		nameTagSRV = textureManager.GetSRV(MobileSuitInfomationUIData::k_Enemy001NameTagFileName);
		break;
	case MobileSuitInfomationUI::ENEMY002:
		nameTagSRV = textureManager.GetSRV(MobileSuitInfomationUIData::k_Enemy002NameTagFileName);
		break;
	case MobileSuitInfomationUI::ALLY:
		nameTagSRV = textureManager.GetSRV(MobileSuitInfomationUIData::k_AllyNameTagFileName);
		break;
	default:
		break;
	}
	if (nameTagSRV == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitInfomationUI::InitActor nameTagSRV��nullptr");
		return;
	}
	// �e�N�X�`���Z�b�g
	if (m_NameTagBillBoardComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitInfomationUI::InitActor m_NameTagBillBoardComponent��nullptr");
		return;
	}
	m_NameTagBillBoardComponent->SetSRV(nameTagSRV);

	if (m_ParentActor == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitInfomationUI::InitActor m_ParentActor��nullptr");
		return;
	}
	Conv_XM::Vector3f parentActorPosition = m_ParentActor->GetPosition();

	m_NameTagBillBoardComponent->SetPosition(parentActorPosition + MobileSuitInfomationUIData::k_DefaultOffsetNameTagPosition);
	m_NameTagBillBoardComponent->SetSize(MobileSuitInfomationUIData::k_DefaultNameTagSize);

	// �e�̃��C�t�R���|�[�l���g�擾
	m_LifeComponent = m_ParentActor->GetLifeComponent();
	if (m_LifeComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitInfomationUI::InitActor m_LifeComponent��nullptr");
		return;
	}

	//�@�e�N�X�`���擾
	ID3D11ShaderResourceView* gaugeSRV = textureManager.GetSRV(MobileSuitInfomationUIData::k_LifeGaugeFileName);
	if (gaugeSRV == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitInfomationUI::InitActor gaugeSRV��nullptr");
		return;
	}
	// �e�N�X�`���Z�b�g
	if (m_LifeGaugeBillBoardComponent == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitInfomationUI::InitActor m_LifeGaugeBillBoardComponent��nullptr");
		return;
	}

	m_LifeGaugeBillBoardComponent->SetRatio(1.0f);
	m_LifeGaugeBillBoardComponent->SetSRV(gaugeSRV);
	m_LifeGaugeBillBoardComponent->SetSize(MobileSuitInfomationUIData::k_LifeGaugeMaxSize);
	m_LifeGaugeBillBoardComponent->SetMaxLength(MobileSuitInfomationUIData::k_LifeGaugeMaxSize.x);
}

void MobileSuitInfomationUI::UpdateActor()
{
	if (m_LifeComponent == nullptr || m_LifeGaugeBillBoardComponent == nullptr || m_NameTagBillBoardComponent == nullptr || m_ParentActor == nullptr)
	{
		Logger::GetInstance().SetLog("MobileSuitInfomationUI::UpdateActor nullptr");
		return;
	}

	if (m_ParentActor->GetActive() == false)
	{
		SetActive(false);
		return;
	}
	else
	{
		SetActive(true);
	}

	//�@�e�̃|�W�V�����擾
	Conv_XM::Vector3f parentActorPosition = m_ParentActor->GetPosition();

	// �r���{�[�h�R���|�[�l���g�ɃZ�b�g
	m_NameTagBillBoardComponent->SetPosition(parentActorPosition + MobileSuitInfomationUIData::k_DefaultOffsetNameTagPosition);

	// ���C�t�Q�[�W�r���{�[�h�R���|�[�l���g�ɃZ�b�g
	m_LifeGaugeBillBoardComponent->SetStartPosition(parentActorPosition + MobileSuitInfomationUIData::k_OffsetLifeGaugePosition);
	float ratio = m_LifeComponent->GetLifeRatio();
	m_LifeGaugeBillBoardComponent->SetRatio(ratio);
}