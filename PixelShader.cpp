#include "PixelShader.h"
#include <d3dcompiler.h>
RenewEngine::PixelShader::PixelShader(LPCWSTR filenameCSO)
{
	D3DReadFileToBlob(filenameCSO, &m_blob);
}

ID3DBlob* RenewEngine::PixelShader::GetBlob() {
	return m_blob.Get();
}