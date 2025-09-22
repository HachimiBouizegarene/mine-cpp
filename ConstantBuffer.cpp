#include "ConstantBuffer.h"
#include <iostream>
#include "Helper.h"
RenewEngine::ConstantBuffer::ConstantBuffer(ID3D12Device* device, ConstantBuffer::Type type) : m_type(type)
{

	D3D12_RESOURCE_DESC resourceDesc = {};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.MipLevels = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
	resourceDesc.Height = 1;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resourceDesc.SampleDesc = { 1, 0 };

	switch (type)
	{
	case RenewEngine::ConstantBuffer::Type::Camera:
		resourceDesc.Width = sizeof(CameraCBData);
		break;
	default:
		resourceDesc.Width = sizeof(CameraCBData);
		break;
	}

	D3D12_HEAP_PROPERTIES props = {};
	props.Type = D3D12_HEAP_TYPE_UPLOAD;

	ThrowIfFailed(device->CreateCommittedResource(&props, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_resource)));

}

void RenewEngine::ConstantBuffer::Update(void* data)
{
	void* ptr;
	m_resource->Map(0, nullptr, &ptr);
	size_t size = 0;
	switch (m_type)
	{
	case Type::Camera:
		size = sizeof(CameraCBData);
	default:
		break;
	}
	memcpy(ptr, data, size);
	m_resource->Unmap(0, nullptr);
}


void RenewEngine::ConstantBuffer::Bind(ID3D12GraphicsCommandList* commandList, UINT rootParamIndex)
{
	commandList->SetGraphicsRootConstantBufferView(rootParamIndex, m_resource->GetGPUVirtualAddress());
}