#include "IndexBuffer.h"
#include <iostream>

RenewEngine::IndexBuffer::IndexBuffer(UploadBuffer* uploadBuffer, UINT indices[], UINT indicesSizeInBytes) : m_uploadBuffer(uploadBuffer), m_numIndices(indicesSizeInBytes / 4)
{
    
    m_view.SizeInBytes = indicesSizeInBytes;
    m_view.Format = DXGI_FORMAT_R32_UINT;
    UBJob uploadJob;
    uploadJob.dataPtr = indices;
    uploadJob.resourceDestination = &m_resource;
    uploadJob.sizeDataInBytes = indicesSizeInBytes;
    uploadJob.onUploadEnd = [&]() {
        m_view.BufferLocation = m_resource->GetGPUVirtualAddress();
        MarkReady();
    };
    uploadBuffer->RegisterJob(uploadJob);
}

void RenewEngine::IndexBuffer::Bind(ID3D12GraphicsCommandList* commandList) {
    commandList->IASetIndexBuffer(&m_view);
}

D3D12_INDEX_BUFFER_VIEW* RenewEngine::IndexBuffer::GetViewPtr() {
    return &m_view;
}

