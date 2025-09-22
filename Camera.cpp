#include "Camera.h"
#include <iostream>
RenewEngine::Camera::Camera(XMFLOAT3 pos, float fov, float aspectRatio) : Object(pos), m_data({ XMMatrixTranspose(XMMatrixPerspectiveFovLH(fov, aspectRatio, 1.0f, 1000.0f)),  XMMatrixTranspose(XMMatrixTranslation(pos.x, pos.y, pos.z)) })
{
	
}

void RenewEngine::Camera::SetPosition(const XMFLOAT3& pos)
{
	Object::SetPosition(pos);
	m_data.view = XMMatrixTranspose(XMMatrixTranslation(pos.x, pos.y, pos.z));
}


void RenewEngine::Camera::UpdatePosition(const float& x, const float& y, const float& z) {
	Object::UpdatePosition(x, y, z);
	std::cout << m_pos.z << std::endl;
	m_data.view = XMMatrixTranspose(XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z));
}