#include "PSO.h"
#include "Helper.h"

RenewEngine::PSO::PSO(ID3D12Device* device, LPCWSTR vsFilename, LPCWSTR psFilename) {

	m_VS = std::make_unique<VertexShader>(vsFilename);
	m_PS = std::make_unique<PixelShader>(psFilename);


	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.NumParameters = 0;
	rootSignatureDesc.NumStaticSamplers = 0;
	rootSignatureDesc.pParameters = nullptr;
	rootSignatureDesc.pStaticSamplers = nullptr;

	ComPtr<ID3DBlob> rootSignatureBlob;
	ComPtr<ID3DBlob> rootSignatureErrorBlob;
	ThrowIfFailed(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &rootSignatureBlob, &rootSignatureErrorBlob));

	ThrowIfFailed(device->CreateRootSignature(0, rootSignatureBlob->GetBufferPointer(), rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature)));


	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = { 0 };
	psoDesc.NodeMask = 0;
	psoDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.pRootSignature = m_rootSignature.Get();

	psoDesc.DepthStencilState.DepthEnable = FALSE;
	psoDesc.DepthStencilState.StencilEnable = FALSE;

	psoDesc.VS.BytecodeLength = m_VS->GetBlob()->GetBufferSize();
	psoDesc.VS.pShaderBytecode = m_VS->GetBlob()->GetBufferPointer();

	psoDesc.PS.BytecodeLength = m_PS->GetBlob()->GetBufferSize();
	psoDesc.PS.pShaderBytecode = m_PS->GetBlob()->GetBufferPointer();

	psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

	psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	psoDesc.SampleMask = UINT_MAX;

	psoDesc.SampleDesc = { 1, 0 };

	psoDesc.BlendState.RenderTarget[0].BlendEnable = FALSE;
	psoDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	D3D12_INPUT_ELEMENT_DESC inputElDesc = {};
	inputElDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElDesc.SemanticName = "POSITION";
	inputElDesc.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;

	D3D12_INPUT_ELEMENT_DESC elementsDesc[] = {
		inputElDesc
	};
	psoDesc.InputLayout.NumElements = 1;
	psoDesc.InputLayout.pInputElementDescs = elementsDesc;


	psoDesc.CachedPSO.pCachedBlob = nullptr;
	psoDesc.CachedPSO.CachedBlobSizeInBytes = 0;
	ThrowIfFailed(device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pso)));
}

void RenewEngine::PSO::Bind(ID3D12GraphicsCommandList* commandList)
{
	commandList->SetPipelineState(m_pso.Get());
	commandList->SetGraphicsRootSignature(m_rootSignature.Get());
}
