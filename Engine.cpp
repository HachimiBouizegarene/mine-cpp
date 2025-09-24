#include "Engine.h"
#include <memory>
#include <chrono>
#include <thread>
#include "Models.h"
#include "Helper.h"
#include "PSODesc.h"
#include "PSOManager.h"
#include <iostream>

#include "Material.h"


void RenewEngine::Engine::Run()
{
	// SCENE
	auto lastTime = std::chrono::high_resolution_clock::now();
	int frames = 0;
	double fps = 0.0;
	while (!m_window->shouldQuit)
	{
		m_window->PeekMessages();
		ID3D12GraphicsCommandList *commandListPtr =  m_renderer->BeginFrame();
		m_cbCamera->Update(m_camera->GetCBDataPtr());
		m_camera->UpdatePosition(0, 0, -0.01f);
		if (m_gameObject->IsReady())
		{
			m_gameObject->Bind(commandListPtr);
			m_cbCamera->Bind(commandListPtr, 0);
			m_gameObject->Render(commandListPtr);
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
	m_renderer->WaitForGpu();
}


RenewEngine::Engine::Engine(HINSTANCE hInstance)
{
	unsigned width = 1920;
	unsigned height = 1080;

	m_jobSystem = std::make_unique<JobSystem>();
	m_window = std::make_unique<RenewWindow>(hInstance, L"minecraft", width, height);
	m_window->Show();
	m_dx12Context = std::make_unique<DX12Context>(m_window->GetHwnd(), width, height, 2, true);
	m_renderer = std::make_unique<Renderer>(m_window->GetHwnd(), width, height, m_dx12Context.get());
	m_uploadBuffer = std::make_unique<UploadBuffer>(m_dx12Context->GetDevice(), m_jobSystem.get());
	m_psoManager = std::make_unique<PSOManager>(m_dx12Context->GetDevice());


	//Temporart Scene
	m_camera = std::make_unique<Camera>(XMFLOAT3(0.0f, 0.0f, 10.0f), 3.14 / 2, static_cast<float>(width) / height);
	m_cbCamera = std::make_unique<ConstantBuffer>(m_dx12Context->GetDevice(), ConstantBuffer::Type::Camera);

	D3D12_INPUT_ELEMENT_DESC inputElDesc = {};
	inputElDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElDesc.SemanticName = "POSITION";
	inputElDesc.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;

	D3D12_INPUT_ELEMENT_DESC elementsDesc[] = {
		inputElDesc
	};
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc = {};
	inputLayoutDesc.NumElements = 1;
	inputLayoutDesc.pInputElementDescs = elementsDesc;

	std::unique_ptr<IndexBuffer> indexBuffer = std::make_unique<IndexBuffer>(m_uploadBuffer.get(), test_indicies, sizeof(test_indicies));
	std::unique_ptr<VertexBuffer> vertexBuffer = std::make_unique<VertexBuffer>(m_uploadBuffer.get(), test_vertices, sizeof(test_vertices), sizeof(VertexPos));

	m_gameObject = std::make_unique<GameObject>();
	m_gameObject->SetMesh(std::make_unique<Mesh>(std::move(vertexBuffer), std::move(indexBuffer)));
	std::vector<Material::RootParameter> rootParams;
	Material::RootParameter rootParam1 = {};
	rootParam1.ShaderRegister = 0;
	rootParam1.type = Material::RootParameter::Type::CBV;
	rootParams.push_back(rootParam1);

	m_gameObject->SetMaterial(std::make_unique<Material>(m_psoManager.get(), m_dx12Context->GetDevice(),
		L"VertexShader.cso", L"PixelShader.cso", inputLayoutDesc, rootParams, PSODesc::CullMode::None));
}
