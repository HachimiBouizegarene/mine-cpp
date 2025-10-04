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
#include "PSOManager.h"
#include <iostream>
#include "GameObject.h"
#include "DX12Context.h"
#include "ConstantBuffer.h"
#include "PEngine.h"
#include "PLevel.h"

namespace RenewEngine {
	class Engine : public RenewEnginePublic::Engine
	{
		enum API
		{
			DIRECTX12,
		};


		class Context
		{
			JobSystem* jobSystem;
		};
	public:
		Engine(HINSTANCE hInstance);
		void Run() override;
		RenewEnginePublic::Level* AddLevel(std::unique_ptr<RenewEnginePublic::Level> level) override;

		ID3D12Device* GetDevice()
		{
			return m_dx12Context->GetDevice();
		}
	private:
		std::unique_ptr<RenewWindow> m_window;
		std::unique_ptr<Renderer> m_renderer;
		std::unique_ptr<JobSystem> m_jobSystem;
		std::unique_ptr<UploadBuffer> m_uploadBuffer;
		std::unique_ptr<PSOManager> m_psoManager;
		std::unique_ptr<DX12Context> m_dx12Context;
		//Temporary
	
		Level* m_activelevel = nullptr;
		std::vector<std::unique_ptr<Level>> m_levels;
	};
}


