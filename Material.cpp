#include "Material.h"
#include "Helper.h"

RenewEngine::Material::Material(
	PSOManager* psoManager,
	ID3D12Device* device,
	LPCWSTR VSFilename,
	LPCWSTR PSFilename,
	const D3D12_INPUT_LAYOUT_DESC& elementDesc,
	const std::vector<RootParameter>& rootParameters,
	PSODesc::CullMode cullMode = PSODesc::CullMode::Back)
{

	m_rootParams.reserve(m_rootParams.size());
	for (const RootParameter& param : rootParameters)
	{
		D3D12_ROOT_PARAMETER rootParam = {};
		switch (param.type)
		{
		case RootParameter::Type::CBV:
			rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		default:
			break;
		}
		rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		rootParam.Descriptor.RegisterSpace = 0;
		rootParam.Descriptor.ShaderRegister = param.ShaderRegister;
		m_rootParams.push_back(rootParam);

	}


	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.NumParameters = 1;
	rootSignatureDesc.pParameters = m_rootParams.data();
	rootSignatureDesc.NumStaticSamplers = 0;

	ComPtr<ID3DBlob> rootSignatureBlob;
	ComPtr<ID3DBlob> rootSignatureErrorBlob;
	ThrowIfFailed(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &rootSignatureBlob, &rootSignatureErrorBlob));

	ThrowIfFailed(device->CreateRootSignature(0, rootSignatureBlob->GetBufferPointer(), rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(&(m_psoDesc.rootSignature))));

	m_psoDesc.VS = std::make_unique<VertexShader>(VSFilename, elementDesc);
	m_psoDesc.PS = std::make_unique<Shader>(PSFilename);
	m_psoDesc.numRtv = 1;
	m_psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_psoDesc.primitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	m_psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	m_psoDesc.layoutDesc = elementDesc;
	m_psoDesc.blendDesc.RenderTarget[0].BlendEnable = FALSE;
	m_psoDesc.blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	m_PSO = psoManager->GetOrCreatePSO(m_psoDesc);
}

void RenewEngine::Material::Bind(ID3D12GraphicsCommandList* commandList)
{
	commandList->SetGraphicsRootSignature(m_psoDesc.rootSignature.Get());
	commandList->SetPipelineState(m_PSO);
}
