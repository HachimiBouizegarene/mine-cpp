#pragma once
#include <DirectXMath.h>

using namespace DirectX;

namespace RenewEngine
{
	class Object
	{
	public:
		Object(const XMFLOAT3& pos);
		void SetPosition(const XMFLOAT3& pos);
		void UpdatePosition(const float& x, const float& y, const float& z);

	protected:
		XMFLOAT3 m_pos;
	};
}