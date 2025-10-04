#include "GameObject.h"
#include <iostream>
RenewEngine::Mesh::Mesh(std::unique_ptr<VertexBuffer> vertexBuffer, std::unique_ptr<IndexBuffer> indexBuffer) : 
	m_vertexBuffer(std::move(vertexBuffer)), m_indexBuffer(std::move(indexBuffer))
{
	
}

void RenewEngine::Mesh::Draw(ID3D12GraphicsCommandList* commandListPtr)
{
	m_indexBuffer->Bind(commandListPtr);
	m_vertexBuffer->Bind(commandListPtr);
	commandListPtr->DrawIndexedInstanced(m_indexBuffer->GetNumIndices(), 1, 0, 0, 0);
}

bool RenewEngine::Mesh::IsReady()
{
	return m_vertexBuffer->IsReady() && m_indexBuffer->IsReady();
}


void RenewEngine::GameObject::Render(ID3D12GraphicsCommandList* commandListPtr)
{

	m_mesh->Draw(commandListPtr);
}

void RenewEngine::GameObject::Bind(ID3D12GraphicsCommandList* commandListPtr)
{
	commandListPtr->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_material->Bind(commandListPtr);
}



void RenewEngine::GameObject::SetMesh(std::unique_ptr<Mesh> mesh)
{
	m_mesh = std::move(mesh);
}

void RenewEngine::GameObject::SetMaterial(std::unique_ptr<Material> material)
{
	m_material = std::move(material);
}

bool RenewEngine::GameObject::IsReady()
{
	return m_mesh->IsReady();
}