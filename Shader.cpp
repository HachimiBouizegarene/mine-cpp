#include "Shader.h"
#include <d3dcompiler.h>
#include "Helper.h"
RenewEngine::Shader::Shader(LPCWSTR filenameCSO)
{
	ThrowIfFailed(D3DReadFileToBlob(filenameCSO, &m_blob));
}

ID3DBlob* RenewEngine::Shader::Shader::GetBlob() {
	return m_blob.Get();
}