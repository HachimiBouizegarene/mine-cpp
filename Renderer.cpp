#include "Renderer.h"
#include "Helper.h"

RenewEngine::Renderer::~Renderer()
{
	WaitForGpu();
}

void RenewEngine::Renderer::Init(HWND hwnd, unsigned int width, unsigned int height)
{
	m_width = width;
	m_height = height;
	m_fenceValues.resize(m_backBuffersCount);
	m_commandAllocators.resize(m_backBuffersCount);
	ComPtr<IDXGIFactory5> dxgiFactory;
	ThrowIfFailed(CreateDXGIFactory2(NULL, IID_PPV_ARGS(&dxgiFactory)));

	ThrowIfFailed(dxgiFactory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &m_supportTearing, sizeof(m_supportTearing)));
	if (m_supportTearing) std::cout << "Support Tearing !" << std::endl;
	else  std::cout << "Do Not Support Tearing !" << std::endl;

	ThrowIfFailed(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&m_device)));

	D3D12_COMMAND_QUEUE_DESC commandQueueDesc = {};
	commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	commandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	commandQueueDesc.NodeMask = 0;
	commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	ThrowIfFailed(m_device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&m_commandQueue)));
	for (size_t i = 0; i < m_backBuffersCount; i++)
	{
		ThrowIfFailed(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocators[i])));
	}
	ThrowIfFailed(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocators[0].Get(), nullptr, IID_PPV_ARGS(&m_commandList)));

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.BufferCount = m_backBuffersCount;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	// Pour la VR
	swapChainDesc.Stereo = false;
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	if (m_supportTearing) swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
	else swapChainDesc.Flags = 0;
	
	// MSAA
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	ComPtr<IDXGISwapChain1> swapChain1;
	ThrowIfFailed(dxgiFactory->CreateSwapChainForHwnd(m_commandQueue.Get(), hwnd, &swapChainDesc, nullptr, nullptr, &swapChain1));
	ThrowIfFailed(swapChain1.As(&m_swapChain));

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};

	heapDesc.NumDescriptors = m_backBuffersCount;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heapDesc.NodeMask = 0;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	ThrowIfFailed(m_device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_RTVDescriptorHeap)));

	unsigned int rtvDescriptorHandleSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptor = m_RTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	m_backBuffers.resize(m_backBuffersCount);
	for (unsigned int i = 0; i < m_backBuffersCount ; i++)
	{
		ThrowIfFailed(m_swapChain->GetBuffer(i, IID_PPV_ARGS(&m_backBuffers[i])));
		m_device->CreateRenderTargetView(m_backBuffers[i].Get(), nullptr, cpuDescriptor);
		cpuDescriptor.ptr += rtvDescriptorHandleSize;
	}

	ThrowIfFailed(m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));

	// Create Fence Event
	m_fenceEvent = CreateEventW(NULL, FALSE, FALSE, NULL);
	if (!m_fenceEvent) throw std::exception();

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.NodeMask = 0;
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	ThrowIfFailed(m_device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_DSVDescriptorHeap)));

	D3D12_RESOURCE_DESC depthStencilDesc = {};
	depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilDesc.SampleDesc = { 1, 0 };
	depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	depthStencilDesc.DepthOrArraySize = 1;

	D3D12_HEAP_PROPERTIES dsvHeapProperties = {};
	dsvHeapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
	dsvHeapProperties.VisibleNodeMask = 0;
	dsvHeapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	dsvHeapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	dsvHeapProperties.CreationNodeMask = 0;

	D3D12_CLEAR_VALUE optimizedClearValue = {};
	optimizedClearValue.Format = DXGI_FORMAT_D32_FLOAT;
	optimizedClearValue.DepthStencil = { 1.0f, 0 };
	ThrowIfFailed(m_device->CreateCommittedResource(&dsvHeapProperties, D3D12_HEAP_FLAG_NONE, &depthStencilDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &optimizedClearValue, IID_PPV_ARGS(&m_depthStencilBuffer)));

	m_device->CreateDepthStencilView(m_depthStencilBuffer.Get(), &dsvDesc, m_DSVDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
	if (m_depthStencilBuffer == nullptr) throw std::exception();

	m_currentBackBufferIndex = m_swapChain->GetCurrentBackBufferIndex();
	ThrowIfFailed(m_commandList->Close());
}

ID3D12GraphicsCommandList* RenewEngine::Renderer::BeginFrame()
{
	ThrowIfFailed(m_commandAllocators[m_currentBackBufferIndex]->Reset());
    ThrowIfFailed(m_commandList->Reset(m_commandAllocators[m_currentBackBufferIndex].Get(), nullptr));

	D3D12_RESOURCE_BARRIER barrier;
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = m_backBuffers[m_currentBackBufferIndex].Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	m_commandList->ResourceBarrier(1, &barrier);

	D3D12_CPU_DESCRIPTOR_HANDLE RTVHandle = m_RTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	RTVHandle.ptr += m_currentBackBufferIndex * m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	D3D12_CPU_DESCRIPTOR_HANDLE DSVHandle = m_DSVDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

	m_commandList->OMSetRenderTargets(1, &RTVHandle, FALSE, &DSVHandle);
	D3D12_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<float>(m_width);
	viewport.Height = static_cast<float>(m_height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	m_commandList->RSSetViewports(1, &viewport);

	D3D12_RECT scissorRect = {};
	scissorRect.left = 0;
	scissorRect.top = 0;
	scissorRect.right = m_width;
	scissorRect.bottom = m_height;
	m_commandList->RSSetScissorRects(1, &scissorRect);
	FLOAT clearColor[] = { 1.0f, 0.0f, 0.0f, 0.0f };
	m_commandList->ClearRenderTargetView(RTVHandle, clearColor, 0, nullptr);
	m_commandList->ClearDepthStencilView(DSVHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	return m_commandList.Get();
};

void RenewEngine::Renderer::EndFrame()
{
	D3D12_RESOURCE_BARRIER barrier;
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = m_backBuffers[m_currentBackBufferIndex].Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	m_commandList->ResourceBarrier(1, &barrier);
	ThrowIfFailed(m_commandList->Close());
	ID3D12CommandList* commandList[] = {
		m_commandList.Get()
	};
	m_commandQueue->ExecuteCommandLists(1, commandList);
	m_fenceValue++;
	ThrowIfFailed(m_commandQueue->Signal(m_fence.Get(), m_fenceValue));
	m_fenceValues[m_currentBackBufferIndex] = m_fenceValue;
	ThrowIfFailed(m_swapChain->Present(0, 0));

	m_currentBackBufferIndex = m_swapChain->GetCurrentBackBufferIndex();

	if (m_fence->GetCompletedValue() < m_fenceValues[m_currentBackBufferIndex]) {
		m_fence->SetEventOnCompletion(m_fenceValues[m_currentBackBufferIndex], m_fenceEvent);
		WaitForSingleObject(m_fenceEvent, INFINITE);
	}
}

void RenewEngine::Renderer::WaitForGpu()
{
	m_fenceValue++;
	ThrowIfFailed(m_commandQueue->Signal(m_fence.Get(), m_fenceValue));
	if (m_fence->GetCompletedValue() < m_fenceValue)
	{
		ThrowIfFailed(m_fence->SetEventOnCompletion(m_fenceValue, m_fenceEvent));
		WaitForSingleObject(m_fenceEvent, INFINITE);
	}
}

void RenewEngine::Renderer::EnableDebugLayer()
{
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&m_debug)))){
		m_debug->EnableDebugLayer();
		std::cout << "DirectX Debug Layer enabled" << std::endl;
	}	
}

ID3D12Device* RenewEngine::Renderer::GetDevice()
{
	return m_device.Get();
}
