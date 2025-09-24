#pragma once

#include "Shader.h"

namespace RenewEngine
{
	class VertexShader : public Shader
	{
	public:
		VertexShader(LPCWSTR filenameCSO, const D3D12_INPUT_LAYOUT_DESC& layoutDesc);
		inline D3D12_INPUT_LAYOUT_DESC& GetInputLayoutDesc() { return m_layoutDesc; };
	private:
		D3D12_INPUT_LAYOUT_DESC m_layoutDesc;
	};

}