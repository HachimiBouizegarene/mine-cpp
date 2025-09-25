#pragma once
#include <DirectXMath.h>


using namespace DirectX;

namespace RenewEnginePublic {
	class Object {
		virtual void SetPosition(const XMFLOAT3& pos) = 0;
		virtual void UpdatePosition(const float& x, const float& y, const float& z) = 0;
	};
}