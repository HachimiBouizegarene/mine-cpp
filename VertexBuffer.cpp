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
    uploadJob.sizeDataInBytes = verticesSizeInBytes;
    std::cout << "Start Uploading Vertex Buffer !" << std::endl;
    uploadJob.onUploadEnd = [&]() {
        m_view.BufferLocation = m_resource->GetGPUVirtualAddress();
        m_ready = true;
        std::cout << "Vertex Buffer Ready !" << std::endl;
        };
    uploadBuffer->Upload(uploadJob);
}

void RenewEngine::VertexBuffer::Bind(ID3D12GraphicsCommandList* commandList) {
    commandList->IASetVertexBuffers(0, 1, &m_view);
}

bool RenewEngine::VertexBuffer::IsReady()
{
    return m_ready;
}



D3D12_VERTEX_BUFFER_VIEW& RenewEngine::VertexBuffer::GetView()
{
    return m_view;
}
