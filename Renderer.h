#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <vector>

using namespace Microsoft::WRL;

namespace RenewEngine {
	class Renderer
	{
	public:
		~Renderer();
	public:
		void Init(HWND hwnd, unsigned int width, unsigned int height);
		void BeginFrame();
		void EndFrame();
		void WaitForGpu();
		void EnableDebugLayer();

		ID3D12Device* GetDevice();

	private:
		ComPtr<ID3D12Device2> m_device;
		ComPtr<IDXGISwapChain4> m_swapChain;
		ComPtr<ID3D12DescriptorHeap> m_RTVDescriptorHeap;
		ComPtr<ID3D12Resource> m_depthStencilBuffer;
		ComPtr<ID3D12DescriptorHeap> m_DSVDescriptorHeap;
		ComPtr<ID3D12CommandQueue> m_commandQueue;
		std::vector<ComPtr<ID3D12Resource>> m_backBuffers;
		std::vector<ComPtr<ID3D12CommandAllocator>> m_commandAllocators;
		ComPtr<ID3D12GraphicsCommandList> m_commandList;
		ComPtr<ID3D12Fence> m_fence;
		uint64_t m_fenceValue = 0;
		ComPtr<ID3D12Debug> m_debug;
		HANDLE m_fenceEvent;
		unsigned int m_backBuffersCount = 2;
		BOOL m_supportTearing = false;
		std::vector<uint64_t> m_fenceValues;
		unsigned int m_currentBackBufferIndex;
	};
}
