#include "Material.h"

RenewEngine::Material::Material(ID3D12PipelineState* PSO, ComPtr<ID3D12RootSignature> rootSignature) : m_rootSignature(rootSignature), m_PSO(PSO)
{
	
}


void RenewEngine::Material::Bind(ID3D12GraphicsCommandList* commandList)
{
	commandList->SetGraphicsRootSignature(m_rootSignature.Get());
	commandList->SetPipelineState(m_PSO);
}
