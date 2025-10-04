#pragma once
#include <DirectXMath.h>
#include "PComponent.h"
#include "PCameraComponent.h"
#include "PComponent.h"
#include <memory>

using namespace DirectX;

namespace RenewEnginePublic {
	class Object {
	public:
		virtual ~Object() = default;
		virtual void SetPosition(const XMFLOAT3& pos) = 0;
		virtual void UpdatePosition(const float& x, const float& y, const float& z) = 0;

		template<typename T>
		T* GetComponent() {
			static_assert(std::is_base_of<Component, T>::value, "T has to be a Component or inherited !");
			return (T*) GetComponent(T::GetStaticType());
		};
	
		template<typename T>
		T* AddNewComponent()
		{
			static_assert(std::is_base_of<Component, T>::value, "T has to be a Component or inherited !");
			return (T*) AddNewComponent(T::GetStaticType());
		}

	protected:
		virtual Component* GetComponent(Component::Type t) = 0;
		virtual Component* AddNewComponent(Component::Type t) = 0;
	};

	template<typename T>
	std::unique_ptr<T> CreateComponent() {
		static_assert(std::is_base_of<Component, T>::value, "T has to be a Component or inherited !");
		return std::make_unique<T>();
	}


	std::unique_ptr<Object> CreateObject();
}