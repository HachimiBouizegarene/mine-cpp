#pragma once

#include <d3d12.h>
#include <DirectXMath.h>
#include <memory>

#include "Material.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

using namespace DirectX;

namespace RenewEngine
{
	class Mesh
	{
	public:
		Mesh() = default;
		Mesh(std::unique_ptr<VertexBuffer> vertexBuffer, std::unique_ptr<IndexBuffer> indexBuffer);
		void Draw(ID3D12GraphicsCommandList* commandListPtr);
		bool IsReady();

	private:
		
		std::unique_ptr<VertexBuffer> m_vertexBuffer;
		std::unique_ptr<IndexBuffer> m_indexBuffer;
	};


	class GameObject
	{
	public:
		GameObject() = default;
		void Bind(ID3D12GraphicsCommandList* commandListPtr);
		void Render(ID3D12GraphicsCommandList* commandListPtr);
		void SetMesh(std::unique_ptr<Mesh> mesh);
		void SetMaterial(std::unique_ptr<Material> material);
		bool IsReady();
		
	private:
		std::unique_ptr<Material> m_material;
		std::unique_ptr<Mesh> m_mesh;
		XMFLOAT3 pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	};
}