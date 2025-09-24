#include "VertexShader.h"

RenewEngine::VertexShader::VertexShader(LPCWSTR filenameCSO, const D3D12_INPUT_LAYOUT_DESC& layoutDesc) : Shader(filenameCSO), m_layoutDesc(layoutDesc)
{

}
