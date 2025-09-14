#include "IndexBuffer.h"
#include <iostream>

RenewEngine::IndexBuffer::IndexBuffer(UploadBuffer* uploadBuffer, void* vertices, unsigned int verticesSizeInBytes) : m_uploadBuffer(uploadBuffer)
{
    
    m_view.SizeInBytes = verticesSizeInBytes;
    m_view.Format = DXGI_FORMAT_R32_UINT;

    UploadJob uploadJob;
    uploadJob.dataPtr = vertices;
    uploadJob.resourceDestination = &m_resource;
    uploadJob.sizeDataInBytes = verticesSizeInBytes;
    uploadJob.onUploadEnd = [&]() {
        m_view.BufferLocation = m_resource->GetGPUVirtualAddress();
    };
    uploadBuffer->Upload(uploadJob);
}


D3D12_INDEX_BUFFER_VIEW& RenewEngine::IndexBuffer::GetView() {
    return m_view;
}

