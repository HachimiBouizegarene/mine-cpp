#pragma once
#include <DirectXMath.h>


using namespace DirectX;

namespace RenewEnginePublic {
	class Object {
		virtual ~Object() = default;
		virtual void SetPosition(const XMFLOAT3& pos);
		virtual void UpdatePosition(const float& x, const float& y, const float& z);
	};
}