#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <memory>
#include "UploadBuffer.h"
#include "IResource.h"
using namespace Microsoft::WRL;
namespace RenewEngine {
	class IndexBuffer  : public IResource{

	public:
		IndexBuffer(UploadBuffer* uploadBuffer, UINT indices[], UINT indicesSizeInBytes);
		D3D12_INDEX_BUFFER_VIEW* GetViewPtr();
		void Bind(ID3D12GraphicsCommandList *commandList) override;

	public:
		inline UINT GetNumIndices() { return m_numIndices; };

	private:
		D3D12_INDEX_BUFFER_VIEW m_view = {};
		UploadBuffer* m_uploadBuffer;
		ComPtr<ID3D12Resource> m_resource;
		UINT m_numIndices;
	};
}