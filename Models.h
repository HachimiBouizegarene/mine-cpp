#pragma once
#include <DirectXMath.h>
#include <cstdint>
using namespace DirectX;


struct VertexPos {
    XMFLOAT3 pos;
};

static VertexPos test_vertices[8] = {
      { XMFLOAT3(-0.5f, -0.5f, -0.5f) },
    { XMFLOAT3(-0.5f, +0.5f, -0.5f) },
    { XMFLOAT3(+0.5f, +0.5f, -0.5f) },
    { XMFLOAT3(+0.5f, -0.5f, -0.5f) },
    { XMFLOAT3(-0.5f, -0.5f, +0.5f) },
    { XMFLOAT3(-0.5f, +0.5f, +0.5f) },
    { XMFLOAT3(+0.5f, +0.5f, +0.5f) },
    { XMFLOAT3(+0.5f, -0.5f, +0.5f) }

};

static UINT test_indicies[36] =
{
      4, 5, 6,
    4, 6, 7,
    // Back face
    0, 2, 1,
    0, 3, 2,
    // Left face
    0, 1, 5,
    0, 5, 4,
    // Right face
    3, 7, 6,
    3, 6, 2,
    // Top face
    1, 2, 6,
    1, 6, 5,
    // Bottom face
    0, 4, 7,
    0, 7, 3
};