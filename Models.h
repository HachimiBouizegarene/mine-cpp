#pragma once
#include <DirectXMath.h>
#include <cstdint>
using namespace DirectX;


struct VertexPos {
    XMFLOAT3 pos;
};

static VertexPos test_vertices[3] = {
    XMFLOAT3(0.0f,  0.5f, 0.0f),
      XMFLOAT3(0.5f, -0.5f, 0.0f),
   XMFLOAT3(-0.5f, -0.5f, 0.0f),

};

static UINT test_indicies[3] =
{
    0, 1, 2
};