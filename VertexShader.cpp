#include "VertexShader.h"
#include <d3dcompiler.h>
RenewEngine::VertexShader::VertexShader(LPCWSTR filenameCSO)
{
	D3DReadFileToBlob(filenameCSO, &m_blob);
}

ID3DBlob* RenewEngine::VertexShader::GetBlob()
{
	return m_blob.Get();
}
