
#include"../../../system/main.h"
#include "point_light_component.h"
#include"../../../directx/renderer.h"

PointLightComponent::PointLightComponent(Actor * owner)
	:Component(owner)
	, m_PointLight{}
{
}

PointLightComponent::~PointLightComponent()
{

}

void PointLightComponent::SetLight(const CBufferData::PointLight & pointLight)
{
	m_PointLight = pointLight;
	Renderer& renderer = Renderer::GetInstance();
	renderer.SetPointLight(m_PointLight);
}

void PointLightComponent::SetPos(float x, float y, float z)
{
	m_PointLight.Color = Conv_XM::Vector3f(1.0f, 0.0f, 0.8f);
	m_PointLight.Position = Conv_XM::Vector3f(x, y, z);
	m_PointLight.Range = 10.0f;
	Renderer& renderer = Renderer::GetInstance();
	renderer.SetPointLight(m_PointLight);
}
