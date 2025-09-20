#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <Windows.h>
#include<memory>
#include "PixelShader.h"
#include "VertexShader.h"
using namespace Microsoft::WRL;


namespace RenewEngine {
	class PSO
	{
	public:
		PSO(ID3D12Device* device, LPCWSTR vsFilename, LPCWSTR psFilename);
		void Bind(ID3D12GraphicsCommandList* commandList);

	private:
		std::unique_ptr<VertexShader> m_VS;
		std::unique_ptr<PixelShader> m_PS;
		ComPtr<ID3D12PipelineState> m_pso;
		ComPtr<ID3D12RootSignature> m_rootSignature;

	};
}


