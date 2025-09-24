#include "PSOManager.h"
#include "Helper.h"


RenewEngine::PSOManager::PSOManager(ID3D12Device* devicePtr) : m_devicePtr(devicePtr)
{
}

ID3D12PipelineState* RenewEngine::PSOManager::GetOrCreatePSO(const PSODesc& desc)
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = { 0 };
	psoDesc.NodeMask = 0;
	psoDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	psoDesc.PrimitiveTopologyType = desc.primitiveTopologyType;
	psoDesc.pRootSignature = desc.rootSignature.Get();

	psoDesc.DepthStencilState.DepthEnable = FALSE;
	psoDesc.DepthStencilState.StencilEnable = FALSE;

	psoDesc.VS.BytecodeLength = desc.VS->GetBlob()->GetBufferSize();
	psoDesc.VS.pShaderBytecode = desc.VS->GetBlob()->GetBufferPointer();

	psoDesc.PS.BytecodeLength = desc.PS->GetBlob()->GetBufferSize();
	psoDesc.PS.pShaderBytecode = desc.PS->GetBlob()->GetBufferPointer();

	psoDesc.DSVFormat = desc.DSVFormat;
	psoDesc.NumRenderTargets = desc.numRtv;
	for (UINT i = 0; i < desc.numRtv; i++) {
		psoDesc.RTVFormats[i] = desc.RTVFormats[i];
	}
	switch (desc.cullMode)
	{
	case PSODesc::CullMode::Back:
		psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
		break;
	case PSODesc::CullMode::Front:
		psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_FRONT;
		break;
	case PSODesc::CullMode::None:
		psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		break;
	}
	psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	psoDesc.RasterizerState.DepthClipEnable = FALSE;
	psoDesc.SampleDesc = desc.samplerDesc;
	psoDesc.BlendState = desc.blendDesc;
	psoDesc.InputLayout = desc.layoutDesc;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.CachedPSO.pCachedBlob = nullptr;
	psoDesc.CachedPSO.CachedBlobSizeInBytes = 0;
	psoDesc.SampleDesc = { 1, 0 };
	ComPtr<ID3D12PipelineState> pso;
	ThrowIfFailed(m_devicePtr->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pso)));
	m_chachedPSOs.push_back(pso);
	return pso.Get();
}
