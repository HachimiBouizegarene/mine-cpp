#pragma once
#include "PComponent.h"

namespace RenewEnginePublic {
	class MeshComponent : public Component {
	public:
		virtual ~MeshComponent() = default;

		static Component::Type GetStaticType()
		{
			return Component::Type::Mesh;
		}
	};
}