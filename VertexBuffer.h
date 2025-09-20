#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <memory>
#include "UploadBuffer.h"

using namespace Microsoft::WRL;
namespace RenewEngine {
	class VertexBuffer {
		
	public:
		VertexBuffer(UploadBuffer* uploadBuffer, void* vertices, unsigned int verticesSizeInBytes, UINT strideInBytes);
		void Bind(ID3D12GraphicsCommandList* commandList);
		bool IsReady();
		D3D12_VERTEX_BUFFER_VIEW& GetView();


	private:
		D3D12_VERTEX_BUFFER_VIEW m_view = {};
		UploadBuffer* m_uploadBuffer;
		std::atomic<bool> m_ready = false;
		ComPtr<ID3D12Resource> m_resource;
	};
}