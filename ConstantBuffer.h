#pragma once
#include "UploadBuffer.h"
#include <wrl.h>
#include <d3d12.h>
#include "ResourceBase.h"
#include "Helper.h"
using namespace Microsoft::WRL;
namespace RenewEngine
{
	template <typename T>
	class ConstantBuffer
	{

	public:

		ConstantBuffer(ID3D12Device* device)
		{
			D3D12_RESOURCE_DESC resourceDesc = {};
			resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
			resourceDesc.MipLevels = 1;
			resourceDesc.DepthOrArraySize = 1;
			resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
			resourceDesc.Height = 1;
			resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
			resourceDesc.SampleDesc = { 1, 0 };
			resourceDesc.Width = sizeof(T);

			D3D12_HEAP_PROPERTIES props = {};
			props.Type = D3D12_HEAP_TYPE_UPLOAD;

			ThrowIfFailed(device->CreateCommittedResource(&props, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_resource)));
		};
		void Update(void* data)
		{
			void* ptr;
			m_resource->Map(0, nullptr, &ptr);
			size_t size = 0;
			memcpy(ptr, data, sizeof(T));
			m_resource->Unmap(0, nullptr);
		}
		void Bind(ID3D12GraphicsCommandList* commandList, UINT rootParamIndex) {
			commandList->SetGraphicsRootConstantBufferView(rootParamIndex, m_resource->GetGPUVirtualAddress());
		};

	private:
		ComPtr<ID3D12Resource> m_resource;
	};
}