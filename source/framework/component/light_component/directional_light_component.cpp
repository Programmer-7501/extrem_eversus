
#include"../../../system/main.h"
#include "directional_light_component.h"
#include"../../../directx/renderer.h"

DirectionalLightComponent::DirectionalLightComponent(Actor * owner)
	: Component(owner)
	, m_DirectionalLight{}
{
}

DirectionalLightComponent::~DirectionalLightComponent()
{
}

void DirectionalLightComponent::SetLight(const CBufferData::DirectionalLight & directionalLight)
{
	m_DirectionalLight = directionalLight;
	Renderer& renderer = Renderer::GetInstance();
	renderer.SetDirectionalLight(m_DirectionalLight);
}

void DirectionalLightComponent::SetEuler(float x, float y, float z)
{
	float rad_X, rad_Y, rad_Z;
	rad_X = DirectX::XMConvertToRadians(x);
	rad_Y = DirectX::XMConvertToRadians(y);
	rad_Z = DirectX::XMConvertToRadians(z);
	Conv_XM::Vector4f q = DirectX::XMQuaternionRotationRollPitchYaw(rad_X, rad_Y, rad_Z);

	m_DirectionalLight.Direction = DirectX::XMVector3Rotate(Conv_XM::Vector3f::WorldForward, q);
	Renderer& renderer = Renderer::GetInstance();
	renderer.SetDirectionalLight(m_DirectionalLight);
}
