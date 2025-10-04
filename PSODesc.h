#pragma once
#include <wrl.h>
#include "Shader.h"
#include "VertexShader.h"
#include <d3d12.h>
#include <memory>

using namespace Microsoft::WRL;

namespace RenewEngine
{
	struct PSODesc
	{

		enum class CullMode
		{
			Back, Front, None
		};
			
		std::unique_ptr<VertexShader> VS;
		std::unique_ptr<Shader> PS;
		D3D12_INPUT_LAYOUT_DESC layoutDesc;
		ComPtr<ID3D12RootSignature> rootSignature;
		D3D12_PRIMITIVE_TOPOLOGY_TYPE primitiveTopologyType;
		DXGI_FORMAT DSVFormat;
		DXGI_FORMAT RTVFormats[8];
		UINT numRtv;
		CullMode cullMode;
		DXGI_SAMPLE_DESC samplerDesc;
		D3D12_BLEND_DESC blendDesc;
	};
}