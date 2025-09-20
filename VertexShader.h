#pragma once
#include <d3d12.h>
#include <wrl.h>


using namespace Microsoft::WRL;


namespace RenewEngine {
	class VertexShader
	{
	public:
		VertexShader(LPCWSTR filenameCSO);
		ID3DBlob *GetBlob();

	private:
		ComPtr<ID3DBlob> m_blob;
	};
}


