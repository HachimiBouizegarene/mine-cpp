#pragma once
#include <DirectXMath.h>
#include <vector>
#include <memory>
#include "IComponent.h"
#include <d3d12.h>
#include "PObject.h"
#include "ObjectTransform.h"

using namespace DirectX;

namespace RenewEngine
{
	class Level;

	class Object : public RenewEnginePublic::Object
	{
	public:
		ObjectTransform& GetTransform() {
			return m_transform;
		};
		Object();
		explicit Object(XMFLOAT3 pos);
		void SetPosition(const XMFLOAT3& pos) override;
		void UpdatePosition(const float& x, const float& y, const float& z) override;
		void Update() {
			for (std::unique_ptr<IComponent>& comp : m_components)
			{
				comp->Update();
			}
		};		
		void AddComponent(std::unique_ptr<IComponent> component) {
			m_components.push_back(std::move(component));
		}

		template<typename T>
		T* GetComponent()
		{
			for (std::unique_ptr<IComponent>& c : m_components)
			{
				if(c->GetType() == T::GetStaticType()) return (T*) c.get();
			}
			return nullptr;
		}

 		
	protected:
		RenewEnginePublic::Component* GetComponent(RenewEnginePublic::Component::Type t) override {
			for (std::unique_ptr<IComponent>& c : m_components)
			{
				if (c->GetType() == t) return c.get();
			}
			return nullptr;
		}
		RenewEnginePublic::Component* AddNewComponent(RenewEnginePublic::Component::Type t) override;

	protected:
		ObjectTransform m_transform = {};
		std::vector<std::unique_ptr<IComponent>> m_components;
	};
}