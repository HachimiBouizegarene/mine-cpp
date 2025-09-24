#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <vector>
#include "DX12Context.h"
using namespace Microsoft::WRL;

namespace RenewEngine {
	class Renderer
	{
	public:
		Renderer(HWND hwnd, unsigned int width, unsigned int height, DX12Context* dx12Context);

	public:
		ID3D12GraphicsCommandList* BeginFrame();
		void EndFrame();
		void WaitForGpu();

	private:
		std::vector<ComPtr<ID3D12CommandAllocator>> m_commandAllocators;
		ComPtr<ID3D12GraphicsCommandList> m_commandList;
		ComPtr<ID3D12Fence> m_fence;
		uint64_t m_fenceValue = 0;
		HANDLE m_fenceEvent;
		UINT m_width;
		UINT m_height;
		std::vector<uint64_t> m_fenceValues;
		DX12Context* m_dx12Context;
	};
}
