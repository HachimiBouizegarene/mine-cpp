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
	PSODesc psoDesc = {};

	psoDesc.VS = std::make_unique<Shader>(L"VertexShader.cso");
	psoDesc.PS = std::make_unique<Shader>(L"PixelShader.cso");

	psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	psoDesc.numRtv = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
	psoDesc.rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	psoDesc.samplerDesc = { 1, 0 };
	psoDesc.primitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.blendDesc.RenderTarget[0].BlendEnable = FALSE;
	psoDesc.blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.NumParameters = 0;
	rootSignatureDesc.NumStaticSamplers = 0;
	rootSignatureDesc.pParameters = nullptr;
	rootSignatureDesc.pStaticSamplers = nullptr;

	ComPtr<ID3DBlob> rootSignatureBlob;
	ComPtr<ID3DBlob> rootSignatureErrorBlob;
	ThrowIfFailed(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &rootSignatureBlob, &rootSignatureErrorBlob));

	D3D12_INPUT_ELEMENT_DESC inputElDesc = {};
	inputElDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElDesc.SemanticName = "POSITION";
	inputElDesc.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;

	D3D12_INPUT_ELEMENT_DESC elementsDesc[] = {
		inputElDesc
	};
	psoDesc.layoutDesc.NumElements = 1;
	psoDesc.layoutDesc.pInputElementDescs = elementsDesc;

	ThrowIfFailed(m_renderer->GetDevice()->CreateRootSignature(0, rootSignatureBlob->GetBufferPointer(), rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(&(psoDesc.rootSignature))));

	std::unique_ptr<IndexBuffer> indexBuffer = std::make_unique<IndexBuffer>(m_uploadBuffer.get(), test_indicies, sizeof(test_indicies));
	std::unique_ptr<VertexBuffer> vertexBuffer = std::make_unique<VertexBuffer>(m_uploadBuffer.get(), test_vertices, sizeof(test_vertices), sizeof(VertexPos));
	
	m_gameObject = std::make_unique<GameObject>();
	m_gameObject->SetMesh(std::make_unique<Mesh>(std::move(vertexBuffer), std::move(indexBuffer)));
	m_gameObject->SetMaterial(std::make_unique<Material>(m_psoManager->GetOrCreatePSO(psoDesc), psoDesc.rootSignature));

	auto lastTime = std::chrono::high_resolution_clock::now();
	int frames = 0;
	double fps = 0.0;
	while (!m_window->shouldQuit)
	{
		m_window->PeekMessages();
		ID3D12GraphicsCommandList *commandListPtr =  m_renderer->BeginFrame();

		if (m_gameObject->IsReady())
		{
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
	m_renderer = std::make_unique<Renderer>();
	m_renderer->EnableDebugLayer();
	m_renderer->Init(m_window->GetHwnd(), width, height);
	m_uploadBuffer = std::make_unique<UploadBuffer>(m_renderer->GetDevice(), m_jobSystem.get());
	m_psoManager = std::make_unique<PSOManager>(m_renderer->GetDevice());


}
