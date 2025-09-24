#pragma once
#include <DirectXMath.h>
#include <vector>
#include <memory>
#include "IComponent.h"
using namespace DirectX;

namespace RenewEngine
{
	class Object
	{
	public:
		Object(XMFLOAT3 pos = XMFLOAT3(0.0f, 0.0f, 0.0f));
		void SetPosition(const XMFLOAT3& pos);
		void UpdatePosition(const float& x, const float& y, const float& z);
		void Update() {
			for (std::unique_ptr<IComponent>& comp : m_components)
			{
				comp->Update();
			}
		};
		IComponent* GetComponent(IComponent::Type type) {
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