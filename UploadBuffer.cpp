#include "UploadBuffer.h"
#include "Helper.h"
RenewEngine::UploadBuffer::UploadBuffer(ID3D12Device* device, JobSystem* jobSystem) : m_device(device), m_jobSystem(jobSystem) 
{
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc = {};
	commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_COPY;
	
	ThrowIfFailed(device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&m_commandQueue)));
	
	m_commandSlots.reserve(m_numCommandSlots);
	for (size_t i = 0; i < m_numCommandSlots; ++i) {
		m_commandSlots.emplace_back(); 
	}

	for (int i = 0 ; i < m_numCommandSlots; i++)
	{
		ThrowIfFailed(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_COPY, IID_PPV_ARGS(&m_commandSlots[i].commandAllocator)));
		ThrowIfFailed(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_COPY, m_commandSlots[i].commandAllocator.Get(), nullptr, IID_PPV_ARGS(&m_commandSlots[i].commandList)));
		m_commandSlots[i].commandList->Close();
		m_freeCommandSlots.push(&(m_commandSlots[i]));
	}

	ThrowIfFailed(device->CreateFence(m_fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));

	m_thread = std::thread([this]() {
		ThreadLoop();
	});
}

void RenewEngine::UploadBuffer::Stop()
{
	if (!m_shouldQuit)
	{
		m_shouldQuit = true;
		m_cvUploadJobs.notify_one();
		m_cvFreeCommandSlot.notify_one();
		if (m_thread.joinable())
		{
			m_thread.join();
		}
	}
}

RenewEngine::UploadBuffer::~UploadBuffer()
{
	Stop();
}



void RenewEngine::UploadBuffer::ThreadLoop()
{
	while (true)
	{
		UBJob job;
		UploadCommandSlot* commandSlot;
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_cvUploadJobs.wait(lock, [this]() {
				return m_jobs.size() > 0 || m_shouldQuit;
				});
			if (m_shouldQuit) return;
			job = m_jobs.front();
			m_jobs.pop();
	
			m_cvFreeCommandSlot.wait(lock, [this]() {
				return m_freeCommandSlots.size() > 0 || m_shouldQuit;
				});
			if (m_shouldQuit) return;
			commandSlot = m_freeCommandSlots.front();
			m_freeCommandSlots.pop();
		}
		commandSlot->commandAllocator->Reset();
		commandSlot->commandList->Reset(commandSlot->commandAllocator.Get(), nullptr);

		D3D12_RESOURCE_DESC resourceDesc = {};
		resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resourceDesc.MipLevels = 1;
		resourceDesc.DepthOrArraySize = 1;
		resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		resourceDesc.Height = 1;
		resourceDesc.Width = job.sizeDataInBytes;
		resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		resourceDesc.SampleDesc = { 1, 0 };

		D3D12_HEAP_PROPERTIES props = {};
		props.Type = D3D12_HEAP_TYPE_DEFAULT;

		ThrowIfFailed(m_device->CreateCommittedResource(&props, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&(*(job.resourceDestination)))));

		ComPtr<ID3D12Resource> uploadResource;
		D3D12_HEAP_PROPERTIES uploadProps = {};
		uploadProps.Type = D3D12_HEAP_TYPE_UPLOAD;

		ThrowIfFailed(m_device->CreateCommittedResource(&uploadProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&uploadResource)));

		void* ptr;
		ThrowIfFailed(uploadResource->Map(0, nullptr, &ptr));
		memcpy(ptr, job.dataPtr, job.sizeDataInBytes);
		uploadResource->Unmap(0, nullptr);
		m_fenceValue++;
		commandSlot->commandList->CopyBufferRegion((job.resourceDestination)->Get(), 0, uploadResource.Get(), 0, job.sizeDataInBytes);
		commandSlot->commandList->Close();
		ID3D12CommandList* commandLists[] = {commandSlot->commandList.Get()};
		m_commandQueue->ExecuteCommandLists(1, commandLists);
		ThrowIfFailed(m_commandQueue->Signal(m_fence.Get(), m_fenceValue));

		//std::cout << "Uploaded Resource CPU Done" << std::endl;

		std::shared_ptr<JobState> jobState = m_jobSystem->RegisterJob([this, job,commandSlot, uploadResource]() {
			bool shouldWait = false;
			HANDLE event = nullptr;
			{
				if (m_fence->GetCompletedValue() < m_fenceValue)
				{
					event = CreateEvent(nullptr, FALSE, FALSE, nullptr);
					if (!event) throw std::exception();
					m_fence->SetEventOnCompletion(m_fenceValue, event);
					shouldWait = true;
				}
			}
			if (shouldWait && event) WaitForSingleObject(event, INFINITE);
			{
				std::unique_lock<std::mutex> lock(m_mutex);
				m_freeCommandSlots.push(commandSlot);
				m_cvFreeCommandSlot.notify_one();
			}
		
			if (job.onUploadEnd) {
				job.onUploadEnd();
			}
			if (job.readyPtr) *(job.readyPtr) = true;
		});

	}
}



void RenewEngine::UploadBuffer::RegisterJob(UBJob job)
{
	m_jobs.push(job);
	m_cvUploadJobs.notify_one();
}
