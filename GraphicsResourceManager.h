#pragma once

#include <d3d12.h>
#include "ConstantBuffer.h"
namespace RenewEngine
{
	class GraphicsResourceManager
	{
	public:
		GraphicsResourceManager() : m_device()
		{

		};

	private:
		ID3D12Device* m_device;

	};
}