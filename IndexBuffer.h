#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <memory>
#include "UploadBuffer.h"

using namespace Microsoft::WRL;
namespace RenewEngine {
	class IndexBuffer {

	public:
		IndexBuffer(UploadBuffer* uploadBuffer, void* vertices, unsigned int verticesSizeInBytes);
		bool IsReady();
		D3D12_INDEX_BUFFER_VIEW& GetView();
		void Bind(ID3D12GraphicsCommandList *commandList);


	private:
		D3D12_INDEX_BUFFER_VIEW m_view = {};
		UploadBuffer* m_uploadBuffer;
		ComPtr<ID3D12Resource> m_resource;
		std::atomic<bool> m_ready = false;
	};
}