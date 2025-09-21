#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <memory>
#include "UploadBuffer.h"
#include "IResource.h"

using namespace Microsoft::WRL;
 
namespace RenewEngine {
	class VertexBuffer : public IResource {	
	public:
		VertexBuffer(UploadBuffer* uploadBuffer, void* vertices, UINT verticesSizeInBytes, UINT strideInBytes);
		void Bind(ID3D12GraphicsCommandList* commandList) override;
		D3D12_VERTEX_BUFFER_VIEW* GetViewPtr();
		UINT GetNumVertices() { return m_numVertices; };

	private:
		D3D12_VERTEX_BUFFER_VIEW m_view = {};
		UploadBuffer* m_uploadBuffer;
		ComPtr<ID3D12Resource> m_resource;
		UINT m_numVertices;
	};
}