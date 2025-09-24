#pragma once

#include <d3d12.h>
#include <vector>
#include <wrl.h>
#include <dxgi1_6.h>
#include <iostream>

using namespace Microsoft::WRL;

namespace RenewEngine
{
	class DX12Context
	{
	public :
		DX12Context(HWND hwnd, unsigned int width, unsigned int height, unsigned int numBackBuffers, bool enableDebugLayer);
		void EnableDebugLayer();		

	public:
		 unsigned int& GetNumBackBuffers() { return m_numBackBuffers; };
		 ID3D12Device* GetDevice() { return m_device.Get(); };
		 ID3D12Resource* GetBackBuffer(const unsigned short index) { return m_backBuffers.at(index).Get(); };
		 ID3D12Resource* GetActualBackBuffer() { return m_backBuffers[GetActualBackBufferIndex()].Get(); }
		 	const UINT GetActualBackBufferIndex() { return m_swapChain->GetCurrentBackBufferIndex(); };
		 D3D12_CPU_DESCRIPTOR_HANDLE GetActualBackBufferCpuDescHandle()
		{
			D3D12_CPU_DESCRIPTOR_HANDLE h = m_RTVHeap->GetCPUDescriptorHandleForHeapStart();
			h.ptr += m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV) * GetActualBackBufferIndex();
			return h;
		}
		 ID3D12CommandQueue* GetCommandQueue() {
			return m_commandQueue.Get();
		};

		 D3D12_CPU_DESCRIPTOR_HANDLE GetDSVCpuDescHandle()
		{
			return m_DSVHeap->GetCPUDescriptorHandleForHeapStart();
		}
		 IDXGISwapChain3* GetSwapChain()
		{
			return m_swapChain.Get();
		}

	private:
		ComPtr<IDXGISwapChain3> m_swapChain;
		ComPtr<ID3D12Device> m_device;
		ComPtr<ID3D12CommandQueue> m_commandQueue;
		ComPtr<ID3D12DescriptorHeap> m_RTVHeap;
		ComPtr<ID3D12DescriptorHeap> m_DSVHeap;
		ComPtr<ID3D12Resource> m_depthResource;
		unsigned int m_numBackBuffers;
		std::vector<ComPtr<ID3D12Resource>> m_backBuffers;
	};
}