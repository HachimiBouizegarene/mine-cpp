#pragma once
#include "IComponent.h"

namespace RenewEngine
{
	class CameraComponent : public IComponent{

	public:
		CameraComponent() : IComponent(IComponent::Type::Camera){
			
		};
		void Update() override {
			
		};
	};
}