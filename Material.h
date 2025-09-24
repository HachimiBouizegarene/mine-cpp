#pragma once
#include <d3d12.h>
#include <wrl.h>
#include "Shader.h"
#include <vector>
#include "VertexShader.h"
#include "PSOManager.h"
#include <memory>
using namespace Microsoft::WRL;
namespace RenewEngine
{
	class Material
	{
	public:
		struct RootParameter
		{
			enum class Type {
				CBV
			};
			Type type;
			UINT ShaderRegister;
		};

	public:
		Material(PSOManager* psoManager,
			ID3D12Device* device,
			LPCWSTR VSFilename,
			LPCWSTR PSFilename,
			const D3D12_INPUT_LAYOUT_DESC& elementDesc,
			const std::vector<RootParameter>& rootParameters,
			PSODesc::CullMode cullMode);
		void Bind(ID3D12GraphicsCommandList* commandList);
		bool IsReady();

		//inline ID3D12RootSignature* GetRootSignature() { return m_rootSignature.Get(); }
		//inline ID3D12PipelineState* GetPso() { return m_PSO; };
	private:
		std::unique_ptr<Shader> m_VS;
		PSODesc::CullMode m_cullMode = PSODesc::CullMode::Back;
		bool m_depthTest = true;
		std::unique_ptr<Shader> m_PS;
		std::vector<D3D12_ROOT_PARAMETER> m_rootParams;
		ID3D12PipelineState * m_PSO;
		PSODesc m_psoDesc = {};
	};
}