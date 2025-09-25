#pragma once
#include <DirectXMath.h>
#include <vector>
#include <memory>
#include "IComponent.h"

#include "PObject.h"
using namespace DirectX;

namespace RenewEngine
{
	class Object : public RenewEnginePublic::Object
	{
	public:
		Object(XMFLOAT3 pos = XMFLOAT3(0.0f, 0.0f, 0.0f));
		void SetPosition(const XMFLOAT3& pos) override;
		void UpdatePosition(const float& x, const float& y, const float& z) override;
		void Update(ID3D12GraphicsCommandList* commandList) {
			for (std::unique_ptr<IComponent>& comp : m_components)
			{
				comp->Update(commandList);
			}
		};


		template<typename T>
		IComponent* GetComponent() {
			static_assert(std::is_base_of<IComponent, T>::value, L"T hase to be base of IComponent");
			for (std::unique_ptr<IComponent>& comp : m_components)
			{
				if (comp->GetType() == type)
				{
					return comp.get();
				}
			}
		};
		void AddComponent(std::unique_ptr<IComponent> component) {
			m_components.push_back(std::move(component));
		}
	protected:
		XMFLOAT3 m_pos;
		std::vector<std::unique_ptr<IComponent>> m_components;
	};
}