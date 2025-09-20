#pragma once
#include <memory>
#include "RenewWindow.h"
#include <Windows.h>
#include "VertexBuffer.h"
#include "JobSystem.h"
#include "Renderer.h"
#include "IndexBuffer.h"
#include "UploadBuffer.h"
#include "PSO.h"
#include <iostream>
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
		std::unique_ptr<PSO> m_pso;
		//Temporary
	private:
		std::unique_ptr<VertexBuffer> m_vertexBuffer = nullptr;
		std::unique_ptr<IndexBuffer> m_indexBuffer = nullptr;
	};
}


