#pragma once
#include <memory>
#include "RenewWindow.h"
#include <Windows.h>
#include "VertexBuffer.h"
#include "JobSystem.h"
#include "Renderer.h"
#include "Level.h"
#include "IndexBuffer.h"
#include "UploadBuffer.h"
#include "Camera.h"
#include "PSOManager.h"
#include <iostream>
#include "GameObject.h"
#include "DX12Context.h"
#include "ConstantBuffer.h"
#include "PEngine.h"
namespace RenewEngine {
	class Engine : public RenewEnginePublic::Engine
	{
	public:
		Engine(HINSTANCE hInstance);
		void Run() override;
	private:
		std::unique_ptr<RenewWindow> m_window;
		std::unique_ptr<Renderer> m_renderer;
		std::unique_ptr<JobSystem> m_jobSystem;
		std::unique_ptr<UploadBuffer> m_uploadBuffer;
		std::unique_ptr<PSOManager> m_psoManager;
		//Temporary
	private:
		std::unique_ptr<Camera> m_camera;
		std::unique_ptr<GameObject> m_gameObject;
		std::unique_ptr<ConstantBuffer> m_cbCamera;
		std::unique_ptr<Level> m_level;
		std::unique_ptr<DX12Context> m_dx12Context;
	};
}


