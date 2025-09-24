

#pragma once
#include <d3d12.h>
#include <wrl.h>


using namespace Microsoft::WRL;


namespace RenewEngine {
	class Shader
	{
	public:
		Shader(LPCWSTR filenameCSO);
		
		ID3DBlob* GetBlob();
	protected:
		ComPtr<ID3DBlob> m_blob;
	};
}


