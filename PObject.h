#pragma once
#include <DirectXMath.h>
#include "PComponent.h"
#include <memory>

using namespace DirectX;

namespace RenewEnginePublic {
	class Object {
		virtual void SetPosition(const XMFLOAT3& pos) = 0;
		virtual void UpdatePosition(const float& x, const float& y, const float& z) = 0;

		template<typename T>
		Component* GetComponent() {
			static_assert(std::is_base_of<Component, T>::value, "T has to be a Component or inherited !");
			return
		}

	protected:
		virtual Component* GetComponent() = 0;
	};

	template<typename T>
	std::unique_ptr<T> CreateComponent() {
		static_assert(std::is_base_of<Component, T>::value, "T has to be a Component or inherited !");
		return std::make_unique<T>();
	}


	std::unique_ptr<Object> CreateObject();
}