#pragma once
#include <wrl.h>
#include "PSODesc.h"
#include <d3d12.h>
#include <vector>


using namespace Microsoft::WRL;
namespace RenewEngine
{
	class PSOManager
	{
	
	public:
		PSOManager(ID3D12Device* devicePtr);
		ID3D12PipelineState* GetOrCreatePSO(const PSODesc& desc);


	private:
		ID3D12Device* m_devicePtr;
		std::vector<ComPtr<ID3D12PipelineState>> m_chachedPSOs;
	};

}