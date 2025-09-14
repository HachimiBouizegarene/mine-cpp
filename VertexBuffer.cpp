#include "VertexBuffer.h"

#include <iostream>

RenewEngine::VertexBuffer::VertexBuffer(UploadBuffer* uploadBuffer, void* vertices, unsigned int verticesSizeInBytes, UINT strideInBytes) : m_uploadBuffer(uploadBuffer)
{
    m_view.SizeInBytes = verticesSizeInBytes;
    m_view.StrideInBytes = strideInBytes;
    //m_view.BufferLocation = m_resource->GetGPUVirtualAddress();
    
    UploadJob uploadJob;
    uploadJob.dataPtr = vertices;
    uploadJob.resourceDestination = &m_resource;
    uploadJob.sizeDataInBytes = strideInBytes;
    uploadJob.onUploadEnd = [&]() {
        m_view.BufferLocation = m_resource->GetGPUVirtualAddress();
        };
    uploadBuffer->Upload(uploadJob);
}



D3D12_VERTEX_BUFFER_VIEW& RenewEngine::VertexBuffer::GetView()
{
    return m_view;
}
