
#include"../../../system/main.h"
#include "tree.h"
#include"../../component/mesh_component/billboard_component.h"
#include"../../../directx/resource/texture_manager.h"

namespace TreeData
{
	//! 木のテクスチャのファイル名
	static const char* k_TextureFileName = "asset/texture/tree.png";

	//! 木の数
	static constexpr int k_BillboardNum = 20;

	static const Conv_XM::Vector3f k_Positions[k_BillboardNum] = {
		{-40.0f, 5.0f, 10.0f},
		{-41.0f, 5.0f, 0.0f},
		{-43.0f, 5.0f, -18.0f},
		{-43.0f, 5.0f, -20.0f},
		{-45.0f, 7.0f, -10.0f},
		{-47.0f, 7.0f, -12.0f},
		{-48.0f, 6.0f,  7.0f},
		{-48.0f, 7.0f, -19.0f},
		{-50.0f, 8.0f, 0.0f},
		{-52.0f, 7.0f, 6.0f},
		{-55.0f, 10.0f, -6.0f},
		{-57.0f, 8.0f, -12.0f},
		{-59.0f, 10.0f,  1.0f},
		{-60.0f, 4.0f, 10.0f},
		{-62.0f, 8.0f, -8.0f},
		{-64.0f, 8.0f, -4.0f},
		{-70.0f, 6.0f, -14.0f},
		{-72.0f, 6.0f, -18.0f},
		{-72.0f, 6.0f, -18.0f},
		{-72.0f, 6.0f, -18.0f},
	};

	static const Conv_XM::Vector2f k_Size[k_BillboardNum] = {
		{5.0f, 5.0f},
		{5.0f, 5.0f},
		{5.0f, 5.0f},
		{5.0f, 5.0f},
		{5.0f, 5.0f},
		{5.0f, 5.0f},
		{5.0f, 5.0f},
		{5.0f, 5.0f},
		{5.0f, 5.0f},
		{5.0f, 5.0f},
		{5.0f, 5.0f},
		{5.0f, 5.0f},
		{5.0f, 5.0f},
		{5.0f, 5.0f},
		{5.0f, 5.0f},
		{5.0f, 5.0f},
		{5.0f, 5.0f},
		{5.0f, 5.0f},
		{5.0f, 5.0f},
		{5.0f, 5.0f},
	};
}

void Tree::LoadTexture()
{
	TextureManager& textureManager = TextureManager::GetInstance();
	textureManager.Load(TreeData::k_TextureFileName);
}

void Tree::UnloadTexture()
{
	TextureManager& textureManager = TextureManager::GetInstance();
	textureManager.Unload(TreeData::k_TextureFileName);
}

Tree::Tree()
{
}

Tree::~Tree()
{
}

void Tree::LoadActor()
{
	for (int i = 0; i < TreeData::k_BillboardNum; i++)
	{
		BillBoardComponent* billboardComponent = CreateComponent<BillBoardComponent>();
		m_BillBoardComponents.emplace_back(billboardComponent);
	}
}

void Tree::InitActor()
{
	TextureManager& textureManager = TextureManager::GetInstance();
	ID3D11ShaderResourceView* srv = textureManager.GetSRV(TreeData::k_TextureFileName);

	for (int i = 0; i < m_BillBoardComponents.size(); i++)
	{
		m_BillBoardComponents[i]->SetSRV(srv);

		m_BillBoardComponents[i]->SetShaftRock(1);
		m_BillBoardComponents[i]->SetPosition(TreeData::k_Positions[i]);
		m_BillBoardComponents[i]->SetSize(TreeData::k_Size[i]);
	}
}
