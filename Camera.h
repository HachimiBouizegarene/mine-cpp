#pragma once
#include <DirectXMath.h>
#include "Object.h"
using namespace DirectX;
namespace RenewEngine
{
	struct CameraCBData {
		XMMATRIX proj;
		XMMATRIX view;
	};


	class Camera : public Object
	{
		
	public:
		Camera(XMFLOAT3 pos, float fov, float aspectRatio);
		void SetPosition(const XMFLOAT3& pos);
		void UpdatePosition(const float& x, const float& y, const float& z);
		CameraCBData *GetCBDataPtr() { return &m_data; };
	private:
		CameraCBData m_data;
	};
}