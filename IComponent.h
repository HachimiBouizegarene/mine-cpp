#pragma once

#include <d3d12.h>
#include "PComponent.h"
#include "ObjectTransform.h"

namespace RenewEngine {

	class Object;

	class IComponent : public RenewEnginePublic::Component { 
	public:
		IComponent(Object* owner) :m_owner(owner) {};
	
	protected:
		Object* GetOwner()
		{
			return m_owner;
		};

	private:
		Object* m_owner;


	public:
		virtual void Update() = 0;
		virtual void OnDraw(ID3D12GraphicsCommandList* commandList) = 0;
	};
}