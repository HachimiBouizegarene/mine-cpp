#pragma once
#include "IComponent.h"
#include <iostream>
#include <DirectXMath.h>
#include "ConstantBuffer.h"
#include <memory>
using namespace DirectX;

namespace RenewEngine
{
	class CameraComponent : public IComponent{
	public:

		struct CameraCBData {
			XMMATRIX proj;
			XMMATRIX view;
		};

		CameraComponent(Object* owner) : IComponent(owner) {
			//m_cb = std::make_unique<ConstantBuffer<CameraCBData>>(device);
		}
		void Update() override;
		void OnDraw(ID3D12GraphicsCommandList* commandList) override;
		CameraCBData m_data = {};

	private:
		std::unique_ptr<ConstantBuffer<CameraCBData>> m_cb = nullptr;
	};
}