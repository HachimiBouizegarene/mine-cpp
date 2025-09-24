
#include "DX12Context.h"
#include "Helper.h"

RenewEngine::DX12Context::DX12Context(HWND hwnd, unsigned int width, unsigned int height, unsigned int numBackBuffers, bool enableDebugLayer) : m_numBackBuffers(numBackBuffers)
{
	if (enableDebugLayer) EnableDebugLayer();
	ComPtr<IDXGIFactory3> idxgiFactory;
	CreateDXGIFactory(IID_PPV_ARGS(&idxgiFactory));


	ThrowIfFailed(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&m_device)));

	D3D12_COMMAND_QUEUE_DESC cqDesc = {};
	cqDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	ThrowIfFailed(m_device->CreateCommandQueue(&cqDesc, IID_PPV_ARGS(&m_commandQueue)));

	DXGI_SWAP_CHAIN_DESC1 scDesc = {};
	scDesc.BufferCount = numBackBuffers;
	scDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
	scDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	scDesc.SampleDesc = { 1, 0 };
	scDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	scDesc.Height = height;
	scDesc.Width = width;
	scDesc.Stereo = FALSE;
	scDesc.Scaling = DXGI_SCALING_STRETCH;

	ComPtr <IDXGISwapChain1> swapChainTemp;

	ThrowIfFailed(idxgiFactory->CreateSwapChainForHwnd(m_commandQueue.Get(), hwnd, &scDesc, nullptr, nullptr, &swapChainTemp));
	ThrowIfFailed(swapChainTemp.As(&m_swapChain));


	D3D12_DESCRIPTOR_HEAP_DESC RTVHeapDesc = {};
	RTVHeapDesc.NumDescriptors = m_numBackBuffers;
	RTVHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	ThrowIfFailed(m_device->CreateDescriptorHeap(&RTVHeapDesc, IID_PPV_ARGS(&m_RTVHeap)));

	m_backBuffers.reserve(numBackBuffers);
	for (int i = 0; i < numBackBuffers; i++)
	{
		ComPtr<ID3D12Resource> backBuffer;
		ThrowIfFailed(m_swapChain->GetBuffer(i, IID_PPV_ARGS(&backBuffer)));
		m_backBuffers.push_back(backBuffer);
		D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandle = m_RTVHeap->GetCPUDescriptorHandleForHeapStart();
		cpuDescHandle.ptr += m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV) * i;
		m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, cpuDescHandle);
	}


	D3D12_HEAP_PROPERTIES dsvHeapDesc = {};
	dsvHeapDesc.Type = D3D12_HEAP_TYPE_DEFAULT;
	

	D3D12_RESOURCE_DESC dsvResourceDesc = {};
	dsvResourceDesc.MipLevels = 1;
	dsvResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	dsvResourceDesc.DepthOrArraySize = 1;
	dsvResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	dsvResourceDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvResourceDesc.Width = width;
	dsvResourceDesc.Height= height;
	dsvResourceDesc.SampleDesc = { 1, 0 };

	D3D12_CLEAR_VALUE clearValue = {};
	clearValue.Format = DXGI_FORMAT_D32_FLOAT; 
	clearValue.DepthStencil.Depth = 1.0f;


	ThrowIfFailed(m_device->CreateCommittedResource(&dsvHeapDesc, D3D12_HEAP_FLAG_NONE, &dsvResourceDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &clearValue, IID_PPV_ARGS(&m_depthResource)));

	D3D12_DESCRIPTOR_HEAP_DESC DSVHeapDesc = {};
	DSVHeapDesc.NumDescriptors = m_numBackBuffers;
	DSVHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	ThrowIfFailed(m_device->CreateDescriptorHeap(&DSVHeapDesc, IID_PPV_ARGS(&m_DSVHeap)));
	
	m_device->CreateDepthStencilView(m_depthResource.Get(), nullptr, m_DSVHeap->GetCPUDescriptorHandleForHeapStart());
	
}

void RenewEngine::DX12Context::EnableDebugLayer()
{
	ComPtr<ID3D12Debug>debug;
	ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debug)));
	debug->EnableDebugLayer();
	std::cout << "DX12 Debug Layer Enabled !" << std::endl;
}

