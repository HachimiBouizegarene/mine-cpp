#include "Object.h"
#include <iostream>
#include "CameraComponent.h"
#include <stdexcept>
#include "Level.h"
#include "Engine.h"
RenewEngine::Object::Object() : m_transform({ XMFLOAT3{0.f, 0.f, 0.f} }) 
{
}

RenewEngine::Object::Object(XMFLOAT3 pos) : m_transform({ pos })
{

}

void RenewEngine::Object::SetPosition(const XMFLOAT3& pos) 
{
	m_transform.pos = pos;
}

void RenewEngine::Object::UpdatePosition(const float& x, const float& y, const float& z)
{
	m_transform.pos = XMFLOAT3(m_transform.pos.x +  x, m_transform.pos.y + y, m_transform.pos.z + z);
}

RenewEnginePublic::Component* RenewEngine::Object::AddNewComponent(RenewEnginePublic::Component::Type t)
{
	std::unique_ptr<IComponent> c = nullptr;
	switch (t)
	{
	case RenewEnginePublic::Component::Type::Camera:
		c = std::make_unique<CameraComponent>(this);
		break;
	case RenewEnginePublic::Component::Type::None:
		throw std::runtime_error("The component type can not be None on Add New Component !");
		break;
	default:
		throw std::runtime_error("The component type can not be None on Add New Component !");
		break;
	}
	IComponent* ret = c.get();
	m_components.push_back(std::move(c));

	return ret;
}



std::unique_ptr<RenewEnginePublic::Object> RenewEnginePublic::CreateObject()
{
	return std::make_unique<RenewEngine::Object>();
}
