#pragma once

#include <d3d12.h>
#include "PComponent.h"

namespace RenewEngine {
	class IComponent : public RenewEnginePublic::Component { 
	public:


	public:
		virtual void Update(ID3D12GraphicsCommandList* commandList) = 0;
	
	};
}