#include "Object.h"
#include <iostream>

RenewEngine::Object::Object(const XMFLOAT3& pos) : m_pos(pos)
{

}

void RenewEngine::Object::SetPosition(const XMFLOAT3& pos)
{
	m_pos = pos;
}

void RenewEngine::Object::UpdatePosition(const float& x, const float& y, const float& z)
{
	m_pos = XMFLOAT3(m_pos.x +  x, m_pos.y + y, m_pos.z + z);
}
