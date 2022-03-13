
#include"../../../system/main.h"
#include"../../../directx/renderer.h"
#include"mesh_field_component.h"
#include"../../../directx/resource/shader_manager.h"
#include"../../../directx/resource/texture_manager.h"
#include"../../../directx/render_data/srv_data.h"
#include"../../scene/scene_manager.h"
#include"../../scene/scene.h"
#include"../../scene_function/scene_collision_manager.h"
#include"../../../function/useful_string.h"
#include <fstream>

namespace MeshFieldData
{
	static constexpr int k_X_Num = 80;
	static constexpr float k_X_Length = 5.0f;
	static constexpr int k_Z_Num = 80;
	static constexpr float k_Z_Length = 5.0f;
}

ID3D11ShaderResourceView* MeshFieldComponent::m_Grass_ShaderResourceView = nullptr;
ID3D11ShaderResourceView* MeshFieldComponent::m_Rock_ShaderResourceView = nullptr;
ID3D11ShaderResourceView* MeshFieldComponent::m_Border_ShaderResourceView = nullptr;



void MeshFieldComponent::LoadTexture()
{
	TextureManager& textureManager = TextureManager::GetInstance();

	m_Grass_ShaderResourceView = textureManager.Load("asset/texture/dirt.png");

	m_Rock_ShaderResourceView = textureManager.Load("asset/texture/s_grass.jpg");

	m_Border_ShaderResourceView = textureManager.Load("asset/texture/border.png");
}

void MeshFieldComponent::UnloadTexture()
{
	TextureManager& textureManager = TextureManager::GetInstance();

	textureManager.Unload("asset/texture/dirt.png");

	textureManager.Unload("asset/texture/s_grass.jpg");

	textureManager.Unload("asset/texture/border.png");
}

MeshFieldComponent::MeshFieldComponent(Actor * owner)
	:MeshComponent(owner)
	, m_VertexBuffer(nullptr)
	, m_IndexBuffer(nullptr)
	, m_Vertex(nullptr)
	, m_DeviceContext(nullptr)
	, m_VertexShader(nullptr)
	, m_PixelShader(nullptr)
	, m_InputLayout(nullptr)
	, m_X_Num(MeshFieldData::k_X_Num)
	, m_Z_Num(MeshFieldData::k_Z_Num)
	, m_X_Length(MeshFieldData::k_X_Length)
	, m_Z_Length(MeshFieldData::k_Z_Length)
	, m_IsTitle(false)
{

}

MeshFieldComponent::~MeshFieldComponent()
{
	delete[] m_Vertex;
}

void MeshFieldComponent::Draw() const
{
	if (m_Owner->GetActive() == false)
	{
		return;
	}

	Renderer& renderer = Renderer::GetInstance();

	//入力レイアウト設定
	m_DeviceContext->IASetInputLayout(m_InputLayout);

	//シェーダー設定
	m_DeviceContext->VSSetShader(m_VertexShader, NULL, 0);
	m_DeviceContext->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	Conv_XM::Matrix4x4f mat;
	if (m_IsTitle == false)
	{
		mat = DirectX::XMMatrixIdentity();
	}
	else
	{
		mat = DirectX::XMMatrixRotationRollPitchYaw(0.0f, DirectX::XMConvertToRadians(90.0f), 0.0f);
	}
	renderer.SetWorldMatrix(mat);

	// マテリアル設定
	renderer.SetMaterial(m_Material);

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_NORMAL_TEXCOOD_TEXTUREBLEND);
	UINT offset = 0;
	m_DeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);

	//インデックスバッファ設定
	m_DeviceContext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	//テクスチャ設定
	m_DeviceContext->PSSetShaderResources(SRVData::k_DefaultShaderResourceViewIndex, 1, &m_Grass_ShaderResourceView);
	m_DeviceContext->PSSetShaderResources(SRVData::k_MeshField001ShaderResourceViewIndex, 1, &m_Rock_ShaderResourceView);
	m_DeviceContext->PSSetShaderResources(SRVData::k_MeshField002ShaderResourceViewIndex, 1, &m_Border_ShaderResourceView);
	
	// 使用するプリミティブタイプを設定
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ポリゴン描画
	m_DeviceContext->DrawIndexed((((m_Z_Num + 2) * 2) * m_X_Num - 2), 0, 0);
}

float MeshFieldComponent::GetHeight(const Conv_XM::Vector3f & position)
{
	int x, z;

	x = static_cast<int>(position.x / m_X_Length + m_X_Num / 2);
	z = static_cast<int>(position.z / -m_Z_Length + m_Z_Num / 2);

	Conv_XM::Vector3f pos0, pos1, pos2, pos3;

	if (x < 0 || z < 0 || x > m_X_Num + 1 || z > m_Z_Num + 1)
	{
		return 0;
	}

	pos0 = m_Vertex[x * (m_Z_Num + 1) + z].Position;
	pos1 = m_Vertex[(x + 1) * (m_Z_Num + 1) + z].Position;
	pos2 = m_Vertex[x * (m_Z_Num + 1) + z + 1].Position;
	pos3 = m_Vertex[(x + 1) * (m_Z_Num + 1) + z + 1].Position;


	Conv_XM::Vector3f v12, v1p, c;

	v12 = pos2 - pos1;
	v1p = position - pos1;

	Conv_XM::Vector3f::Cross(v12, v1p, &c);

	float py;
	Conv_XM::Vector3f n;

	if (c.y > 0.0f)
	{
		Conv_XM::Vector3f v10;
		v10 = pos0 - pos1;
		Conv_XM::Vector3f::Cross(v10, v12, &n);
	}
	else
	{
		Conv_XM::Vector3f v13;
		v13 = pos3 - pos1;
		Conv_XM::Vector3f::Cross(v12, v13, &n);
	}

	py = -((position.x - pos1.x) * n.x + (position.z - pos1.z) * n.z) / n.y + pos1.y;

	return py;
}

void MeshFieldComponent::LoadHeightMapFile(const std::string & fileName)
{
	std::ifstream ifs(fileName.c_str());
	if (!ifs)
	{
		Logger::GetInstance().SetLog("MeshFieldComponent::LoadHeightMapFile ファイル読み込み失敗");
		return;
	}

	std::string line;

	int loop = 0;
	while (getline(ifs, line))
	{
		// 最初は長さ
		if (loop == 0)
		{
			std::vector<std::string> strvec = Split(line, ',');
			for (int i = 0; i < 2; i++)
			{
				float in = std::stof(strvec[i]);
				if (i == 0)
				{
					m_X_Length = in;
				}
				else
				{
					m_Z_Length = in;
				}
			}
			
			loop++;
			continue;
		}

		//切り分ける
		std::vector<std::string> strvec = Split(line, ',');

		std::vector<float> x;

		for (const auto& str : strvec)
		{
			float in = std::stof(str);
			x.emplace_back(in);
		}

		m_HeightMap.emplace_back(x);

		loop++;
	}

	if (m_HeightMap.empty() == true)
	{
		return;
	}
	if (m_HeightMap[0].empty() == true)
	{
		return;
	}

	m_Z_Num = static_cast<int>(m_HeightMap.size() - 1);
	m_X_Num = static_cast<int>(m_HeightMap[0].size() - 1);
}

void MeshFieldComponent::LoadTextureBlendFile(const std::string & fileName, int blendNo)
{
	if (blendNo > 4 || blendNo < 1)
	{
		Logger::GetInstance().SetLog("MeshFieldComponent::LoadTextureBlendFile blendNoが違う");
		return;
	}

	std::ifstream ifs(fileName.c_str());
	if (!ifs)
	{
		Logger::GetInstance().SetLog("MeshFieldComponent::LoadTextureBlendFile ファイル読み込み失敗");
		return;
	}

	std::string line;

	while (getline(ifs, line))
	{
		//切り分ける
		std::vector<std::string> strvec = Split(line, ',');

		std::vector<float> x;

		for (const auto& str : strvec)
		{
			float in = std::stof(str);
			x.emplace_back(in);
		}

		switch (blendNo)
		{
		case 1:
			m_TextureBlend1.emplace_back(x);
			break;
		case 2:
			m_TextureBlend2.emplace_back(x);
			break;
		case 3:
			m_TextureBlend3.emplace_back(x);
			break;
		case 4:
			m_TextureBlend4.emplace_back(x);
			break;
		default:
			break;
		}
	}
}

bool MeshFieldComponent::LoadData()
{
	Scene* currentScene = SceneManager::GetInstance().GetCurrentScene();
	if (currentScene == nullptr)
	{
		Logger::GetInstance().SetLog("Player::InitActor currentSceneがnullptr");
		return false;
	}

	// D3D11のデバイスインターフェースの取得
	if (GetDeviceContext() == false)
	{
		Logger::GetInstance().SetLog("ModelComponent::Init GetDeviceInterfaceが失敗");
		return false;
	}

	// シェーダーの取得
	if (GetShader() == false)
	{
		Logger::GetInstance().SetLog("ModelComponent::Init GetShaderが失敗");
		return false;
	}

	// メッシュ作成
	if (CreateMesh() == false)
	{
		Logger::GetInstance().SetLog("ModelComponent::Init CreateMeshが失敗");
		return false;
	}

	return true;
}

bool MeshFieldComponent::CreateMesh()
{
	if (m_HeightMap.empty() || m_TextureBlend1.empty() || m_TextureBlend2.empty() || m_TextureBlend3.empty() || m_TextureBlend4.empty())
	{
		Logger::GetInstance().SetLog("MeshFieldComponent::CreateMesh 空");
		return false;
	}

	HRESULT hr;
	Renderer& renderer = Renderer::GetInstance();
	ID3D11Device* device = renderer.GetDevice();
	if (device == nullptr)
	{
		Logger::GetInstance().SetLog("MeshFieldComponent::CreateMesh deviceがnullptr");
		return false;
	}

	m_Vertex = new VERTEX_NORMAL_TEXCOOD_TEXTUREBLEND[((m_X_Num + 1) * (m_Z_Num + 1))];

	//頂点バッファ生成
	{
		Conv_XM::Vector4f blend = Conv_XM::Vector4f(0.0f,0.0f,0.0f,0.0f);
		for (int x = 0; x <= m_X_Num; x++)
		{
			for (int z = 0; z <= m_Z_Num; z++)
			{
				blend = Conv_XM::Vector4f(m_TextureBlend1[z][x], m_TextureBlend2[z][x], m_TextureBlend3[z][x], m_TextureBlend4[z][x]);

				m_Vertex[x * (m_Z_Num + 1) + z].Position = Conv_XM::Vector3f((x - m_X_Num / 2) * m_X_Length, m_HeightMap[z][x], (z - m_Z_Num / 2) * -m_Z_Length);
				m_Vertex[x* (m_Z_Num + 1) + z].TexCoord = Conv_XM::Vector2f(static_cast<float>(x), static_cast<float>(z));
				m_Vertex[x* (m_Z_Num + 1) + z].TextureBlend = blend;
			}
		}

		//法線ベクトル算出
		for (int x = 1; x <= m_X_Num - 1; x++)
		{
			for (int z = 0; z <= m_Z_Num - 1; z++)
			{
				Conv_XM::Vector3f vx, vz, vn;
				vx = m_Vertex[(x + 1) * (m_Z_Num + 1) + z].Position - m_Vertex[(x - 1) * (m_Z_Num + 1) + z].Position;
				vz = m_Vertex[x * (m_Z_Num + 1) + z - 1].Position - m_Vertex[x * (m_Z_Num + 1) + z + 1].Position;

				vn = DirectX::XMVector3Cross((DirectX::XMVECTOR)vz, (DirectX::XMVECTOR)vx);//外積
				vn = DirectX::XMVector3Normalize(vn);
				m_Vertex[x * (m_Z_Num + 1) + z].Normal = vn;
			}
		}

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_NORMAL_TEXCOOD_TEXTUREBLEND) * ((m_X_Num + 1) * (m_Z_Num + 1));
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = m_Vertex;

		hr = device->CreateBuffer(&bd, &sd, m_VertexBuffer.GetAddressOf());
		if (FAILED(hr))
		{
			Logger::GetInstance().SetLog("MeshFieldComponent::CreateMesh CreateBufferが失敗");
			return false;
		}
	}

	//インデックスバッファ
	{
		unsigned int* index = new unsigned int[((m_Z_Num + 2) * 2) * m_X_Num - 2];

		int i = 0;
		for (int x = 0; x < m_X_Num; x++)
		{
			for (int z = 0; z < m_Z_Num + 1; z++)
			{
				index[i] = x * (m_Z_Num + 1) + z;
				i++;

				index[i] = (x + 1) * (m_Z_Num + 1) + z;
				i++;
			}

			if (x == m_X_Num - 1)break;

			index[i] = (x + 1) * (m_Z_Num + 1) + m_X_Num;
			i++;

			index[i] = (x + 1) * (m_Z_Num + 1);
			i++;
		}

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * (((m_Z_Num + 2) * 2) * m_X_Num - 2);
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = index;

		hr = device->CreateBuffer(&bd, &sd, m_IndexBuffer.GetAddressOf());
		if (FAILED(hr))
		{
			Logger::GetInstance().SetLog("MeshFieldComponent::CreateMesh CreateBufferが失敗");
			return false;
		}

		delete[] index;
	}

	return true;
}

bool MeshFieldComponent::GetDeviceContext()
{
	// デバイスコンテキストの取得
	m_DeviceContext = Renderer::GetInstance().GetDeviceContext();
	// nullチェック
	if (m_DeviceContext == nullptr)
	{
		Logger::GetInstance().SetLog("SpriteComponent::GetInterface m_DeviceContextがnullptr");
		return false;
	}

	return true;
}

bool MeshFieldComponent::GetShader()
{
	// シェーダーマネージャーの取得
	ShaderManager& shaderManager = ShaderManager::GetInstance();

	// 頂点シェーダーの取得
	m_VertexShader = shaderManager.GetVertexShader(ShaderManager::VS_NAME_MESHFIELD);
	if (m_VertexShader == nullptr)
	{
		Logger::GetInstance().SetLog("ModelComponent::GetShader m_InputGBufferVertexShaderがnullptr");
		return false;
	}

	// ピクセルシェーダーの取得
	m_PixelShader = shaderManager.GetPixelShader(ShaderManager::PS_NAME_MESHFIELD);
	if (m_PixelShader == nullptr)
	{
		Logger::GetInstance().SetLog("ModelComponent::GetInterface m_InputGBufferPixelShaderがnullptr");
		return false;
	}

	// インプットレイアウトの取得
	m_InputLayout = shaderManager.GetInputLayout(ShaderManager::IL_NAME_MESHFIELD);
	if (m_InputLayout == nullptr)
	{
		Logger::GetInstance().SetLog("ModelComponent::GetInterface m_InputLayoutがnullptr");
		return false;
	}

	return true;
}
