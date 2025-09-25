#pragma once
#include "IComponent.h"

namespace RenewEngine
{
	class CameraComponent : public IComponent{

	public:
		CameraComponent() : IComponent(IComponent::Type::Camera){
			
		};
		void Update(ID3D12GraphicsCommandList * commandList) override {
			


		};

		static const Type GetStaticType() 
		{
			return Type::Camera;
		}
	};
}