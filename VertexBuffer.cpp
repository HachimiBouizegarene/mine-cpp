#include "VertexBuffer.h"

#include <iostream>

RenewEngine::VertexBuffer::VertexBuffer(UploadBuffer* uploadBuffer, void* vertices, UINT verticesSizeInBytes, UINT strideInBytes) : m_uploadBuffer(uploadBuffer) , m_numVertices(verticesSizeInBytes / strideInBytes)
{
    m_view.SizeInBytes = verticesSizeInBytes;
    m_view.StrideInBytes = strideInBytes;
    //m_view.BufferLocation = m_resource->GetGPUVirtualAddress();
    
    UploadJob uploadJob;
    uploadJob.dataPtr = vertices;
    uploadJob.resourceDestination = &m_resource;
    uploadJob.sizeDataInBytes = verticesSizeInBytes;
    std::cout << "Start Uploading Vertex Buffer !" << std::endl;
    uploadJob.onUploadEnd = [&]() {
        m_view.BufferLocation = m_resource->GetGPUVirtualAddress();
        MarkReady();
        std::cout << "Vertex Buffer Ready !" << std::endl;
        };
    uploadBuffer->Upload(uploadJob);

}

void RenewEngine::VertexBuffer::Bind(ID3D12GraphicsCommandList* commandList) {
    commandList->IASetVertexBuffers(0, 1, &m_view);
}


D3D12_VERTEX_BUFFER_VIEW* RenewEngine::VertexBuffer::GetViewPtr()
{
    return &m_view;
}
