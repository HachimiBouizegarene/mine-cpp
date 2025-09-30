#pragma once
#include "UploadBuffer.h"
#include <wrl.h>
#include <d3d12.h>
#include "ResourceBase.h"
#include "Camera.h"
using namespace Microsoft::WRL;
namespace RenewEngine
{
	
	class ConstantBuffer
	{

	public:
		enum class Type
		{
			Camera,
		};
		ConstantBuffer(ID3D12Device* device, ConstantBuffer::Type type);
		void Update(void* data);
		void Bind(ID3D12GraphicsCommandList* commandList, UINT rootParamIndex) ;

	private:
		ComPtr<ID3D12Resource> m_resource;
		Type m_type;
	};
}