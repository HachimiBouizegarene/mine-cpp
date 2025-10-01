#pragma once
#include "PComponent.h"

namespace RenewEnginePublic {
	class CameraComponent : public Component {
	public:
		virtual ~CameraComponent() = default;
		
		static Component::Type GetStaticType()
		{
			return Component::Type::Camera;
		}
	};
}