#pragma once
#include <memory>
#include "RenewWindow.h"
#include <Windows.h>
#include "VertexBuffer.h"
#include "JobSystem.h"
#include "Renderer.h"
#include "IndexBuffer.h"
#include "UploadBuffer.h"
#include "PSOManager.h"
#include <iostream>
#include "GameObject.h"
namespace RenewEngine {
	class Engine
	{
	public:
		Engine(HINSTANCE hInstance);
		void Run();
	private:
		std::unique_ptr<RenewWindow> m_window;
		std::unique_ptr<Renderer> m_renderer;
		std::unique_ptr<JobSystem> m_jobSystem;
		std::unique_ptr<UploadBuffer> m_uploadBuffer;
		std::unique_ptr<PSOManager> m_psoManager;
		//Temporary
	private:
		std::unique_ptr<GameObject> m_gameObject = nullptr;
	};
}


