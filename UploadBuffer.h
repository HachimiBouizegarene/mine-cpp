#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <vector>
#include <queue>
#include "JobSystem.h"

using namespace Microsoft::WRL;

namespace RenewEngine {


	struct UBJob
	{
		ComPtr<ID3D12Resource>* resourceDestination = nullptr;
		void* dataPtr = nullptr;
		size_t sizeDataInBytes = 0;
		std::atomic<bool>* readyPtr = nullptr;
		std::function<void()> onUploadEnd = nullptr;
	};

	struct UploadCommandSlot
	{
		ComPtr<ID3D12CommandAllocator> commandAllocator;
		ComPtr<ID3D12GraphicsCommandList> commandList;
	};


	class UploadBuffer {
	public :
		UploadBuffer(ID3D12Device* device, JobSystem* jobSystem) ;
		~UploadBuffer();
		void RegisterJob(UBJob uploadjob);
	private:
		void ThreadLoop();
		void Stop();
	private: 
		ComPtr<ID3D12CommandQueue> m_commandQueue;
		ComPtr<ID3D12Fence> m_fence;

		ID3D12Device* m_device = nullptr;
		std::vector<UploadCommandSlot> m_commandSlots;

		std::atomic<uint64_t> m_fenceValue = 0;
		JobSystem* m_jobSystem;

		std::atomic<bool> m_shouldQuit = false;

		std::mutex m_fenceMutex;
		std::mutex m_mutex;

		std::thread m_thread;

		std::queue<UBJob> m_jobs;
		std::queue<UploadCommandSlot*> m_freeCommandSlots;

		std::condition_variable m_cvFreeCommandSlot;
		std::condition_variable m_cvUploadJobs;

		size_t m_numCommandSlots = 3;
	};
}


