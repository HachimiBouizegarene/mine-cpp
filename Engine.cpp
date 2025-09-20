#include "Engine.h"
#include <memory>
#include <chrono>
#include <thread>
#include "Models.h"
#include <iostream>
void RenewEngine::Engine::Run()
{


	auto lastTime = std::chrono::high_resolution_clock::now();
	int frames = 0;
	double fps = 0.0;
	while (!m_window->shouldQuit)
	{
		m_window->PeekMessages();
		ID3D12GraphicsCommandList *commandListPtr =  m_renderer->BeginFrame();

		if (m_vertexBuffer->IsReady() && m_indexBuffer->IsReady())
		{
			m_vertexBuffer->Bind(commandListPtr);
			m_indexBuffer->Bind(commandListPtr);
			m_pso->Bind(commandListPtr);
			commandListPtr->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			commandListPtr->DrawInstanced(3, 1, 0, 0);
		}
		
		m_renderer->EndFrame();
		frames++;
		auto now = std::chrono::high_resolution_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - lastTime);
		if (elapsed.count() >= 1)
		{
			fps = frames / static_cast<double>(elapsed.count());
			std::cout << "FPS : " << fps << std::endl;
			frames = 0;
			lastTime = now;
		}
	} 
}


RenewEngine::Engine::Engine(HINSTANCE hInstance)
{
	unsigned width = 1920;
	unsigned height = 1080;

	m_jobSystem = std::make_unique<JobSystem>();
	m_window = std::make_unique<RenewWindow>(hInstance, L"minecraft", width, height);
	m_window->Show();
	m_renderer = std::make_unique<Renderer>();
	m_renderer->EnableDebugLayer();
	m_renderer->Init(m_window->GetHwnd(), width, height);
	m_uploadBuffer = std::make_unique<UploadBuffer>(m_renderer->GetDevice(), m_jobSystem.get());


	// TEMPORARY
	m_indexBuffer = std::make_unique<IndexBuffer>(m_uploadBuffer.get(), test_indicies, sizeof(test_indicies));
	m_vertexBuffer = std::make_unique<VertexBuffer>(m_uploadBuffer.get(), test_vertices, sizeof(test_vertices), sizeof(VertexPos));
	m_pso = std::make_unique<RenewEngine::PSO>(m_renderer->GetDevice(), L"VertexShader.cso", L"PixelShader.cso");
	// ---------------- END TEMPORARY

}
