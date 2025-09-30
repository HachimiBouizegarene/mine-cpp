#pragma once

#include <d3d12.h>
#include "PComponent.h"

namespace RenewEngine {
	class IComponent : public RenewEnginePublic::Component { 
	public:
		enum class Type {
			None, Camera
		};

	public:
		IComponent(Type t) : m_type(t) {};
		virtual void Update(ID3D12GraphicsCommandList* commandList) = 0;
		Type GetType() {
			return m_type;
		}

		static const Type GetStaticType() {
			return Type::None;
		};
	
	protected:
		Type m_type;
	
	};
}