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

		D3D12_VERTEX_BUFFER_VIEW& GetView();


	private:
		D3D12_VERTEX_BUFFER_VIEW m_view = {};
		UploadBuffer* m_uploadBuffer;
		ComPtr<ID3D12Resource> m_resource;
	};
}