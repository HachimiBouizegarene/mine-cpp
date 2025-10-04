#include "CameraComponent.h"
#include "Object.h"

void RenewEngine::CameraComponent::Update()
{
	ObjectTransform transform = GetOwner()->GetTransform();
	m_data.view = XMMatrixTranspose(XMMatrixTranslation(transform.pos.x, transform.pos.x, transform.pos.z));
}

void RenewEngine::CameraComponent::OnDraw(ID3D12GraphicsCommandList* commandList)
{
	m_cb->Bind(commandList, 0);
}

