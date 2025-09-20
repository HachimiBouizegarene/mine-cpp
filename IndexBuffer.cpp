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
    std::cout << "Start Uploading Index Buffer !" << std::endl;
    uploadJob.onUploadEnd = [&]() {
        m_view.BufferLocation = m_resource->GetGPUVirtualAddress();
        m_ready = true;
        std::cout << "Index Buffer Ready !" << std::endl;
    };
    uploadBuffer->Upload(uploadJob);
}

void RenewEngine::IndexBuffer::Bind(ID3D12GraphicsCommandList* commandList) {
    commandList->IASetIndexBuffer(&m_view);
}

bool RenewEngine::IndexBuffer::IsReady() {
    return m_ready;
}

D3D12_INDEX_BUFFER_VIEW& RenewEngine::IndexBuffer::GetView() {
    return m_view;
}

