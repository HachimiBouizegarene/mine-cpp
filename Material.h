#pragma once
#include <d3d12.h>
#include <wrl.h>

using namespace Microsoft::WRL;
namespace RenewEngine
{
	class Material
	{
	public:
		Material(ID3D12PipelineState* PSO, ComPtr<ID3D12RootSignature> rootSignature);
		inline ID3D12RootSignature* GetRootSignature() { return m_rootSignature.Get(); }
		inline ID3D12PipelineState* GetPso() { return m_PSO; };
		void Bind(ID3D12GraphicsCommandList* commandList);
		bool IsReady();
	private:
		ComPtr<ID3D12RootSignature> m_rootSignature;
		ID3D12PipelineState * m_PSO;
	};
}