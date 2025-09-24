#include "Renderer.h"
#include "Helper.h"
#include "DX12Context.h"

RenewEngine::Renderer::Renderer(HWND hwnd, unsigned int width, unsigned int height, DX12Context* dx12Context) : m_dx12Context(dx12Context)
{
	m_width = width;
	m_height = height;
	m_fenceValues.resize(dx12Context->GetNumBackBuffers());
	m_commandAllocators.resize(dx12Context->GetNumBackBuffers());

	for (size_t i = 0; i < dx12Context->GetNumBackBuffers(); i++)
	{
		ThrowIfFailed(dx12Context->GetDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocators[i])));
	}
	ThrowIfFailed(dx12Context->GetDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocators[0].Get(), nullptr, IID_PPV_ARGS(&m_commandList)));

	ThrowIfFailed(dx12Context->GetDevice()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));

	// Create Fence Event
	m_fenceEvent = CreateEventW(NULL, FALSE, FALSE, NULL);
	if (!m_fenceEvent) throw std::exception();

	ThrowIfFailed(m_commandList->Close());
}


ID3D12GraphicsCommandList* RenewEngine::Renderer::BeginFrame()
{
	UINT actualBackBufferIndex  = m_dx12Context->GetActualBackBufferIndex();
	ThrowIfFailed(m_commandAllocators[actualBackBufferIndex]->Reset());
    ThrowIfFailed(m_commandList->Reset(m_commandAllocators[actualBackBufferIndex].Get(), nullptr));

	D3D12_RESOURCE_BARRIER barrier;
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = m_dx12Context->GetActualBackBuffer();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	m_commandList->ResourceBarrier(1, &barrier);


	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_dx12Context->GetActualBackBufferCpuDescHandle();
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = m_dx12Context->GetDSVCpuDescHandle();

	m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);
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
	m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	m_commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	return m_commandList.Get();
};

void RenewEngine::Renderer::EndFrame()
{
	D3D12_RESOURCE_BARRIER barrier;
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = m_dx12Context->GetActualBackBuffer();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	m_commandList->ResourceBarrier(1, &barrier);
	ThrowIfFailed(m_commandList->Close());
	ID3D12CommandList* commandList[] = {
		m_commandList.Get()
	};
	m_dx12Context->GetCommandQueue()->ExecuteCommandLists(1, commandList);
	m_fenceValue++;
	ThrowIfFailed(m_dx12Context->GetCommandQueue()->Signal(m_fence.Get(), m_fenceValue));
	m_fenceValues[m_dx12Context->GetActualBackBufferIndex()] = m_fenceValue;
	ThrowIfFailed(m_dx12Context->GetSwapChain()->Present(0, 0));


	if (m_fence->GetCompletedValue() < m_fenceValues[m_dx12Context->GetActualBackBufferIndex()]) {
		m_fence->SetEventOnCompletion(m_fenceValues[m_dx12Context->GetActualBackBufferIndex()], m_fenceEvent);
		WaitForSingleObject(m_fenceEvent, INFINITE);
	}
}

void RenewEngine::Renderer::WaitForGpu()
{
	m_fenceValue++;
	ThrowIfFailed(m_dx12Context->GetCommandQueue()->Signal(m_fence.Get(), m_fenceValue));
	if (m_fence->GetCompletedValue() < m_fenceValue)
	{
		ThrowIfFailed(m_fence->SetEventOnCompletion(m_fenceValue, m_fenceEvent));
		WaitForSingleObject(m_fenceEvent, INFINITE);
	}
}


