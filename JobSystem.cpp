#include "JobSystem.h"
#include <iostream>

RenewEngine::JobSystem::JobSystem(size_t numWorkers) : m_numWorkers(numWorkers)
{
	m_workers.reserve(numWorkers);
	for (size_t i = 0; i < numWorkers; i++)
	{
		m_workers.emplace_back([this]() {
			ThreadLoop();
		});
	}
}

RenewEngine::JobSystem::~JobSystem()
{
	Stop();
}

void RenewEngine::JobSystem::Stop()
{

	if (!m_shouldQuit) 
	{
	
		m_shouldQuit = true;
		m_cv.notify_all();
		for (std::thread& thread : m_workers)
		{
			thread.join();
		}
	}
}

std::shared_ptr<RenewEngine::JobState> RenewEngine::JobSystem::RegisterJob(std::function<void()> fn)
{
	return RegisterJob(fn, nullptr);
}

std::shared_ptr<RenewEngine::JobState> RenewEngine::JobSystem::RegisterJob(std::function<void()> fn, std::function<void()> onEndFn)
{
	std::unique_ptr<JobHandle> jobHandle = std::make_unique<JobHandle>();
	std::shared_ptr<JobState> jobState = std::make_shared<JobState>();

	jobHandle->job = std::move(fn);
	jobHandle->state = jobState;
	jobHandle->endJob = onEndFn;
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_jobs.push(std::move(jobHandle));
		m_cv.notify_one();
	}
	
	return jobState;
}

void RenewEngine::JobSystem::ThreadLoop()
{

	while (true)
	{
		std::unique_ptr<JobHandle> jobHandle;
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_cv.wait(lock, [this]() {
				return m_jobs.size() > 0 || m_shouldQuit;
				});
			if (m_shouldQuit) return;
			jobHandle = std::move(m_jobs.front());
			m_jobs.pop();
		}
		jobHandle->job();
		if (jobHandle->endJob)
		{
			jobHandle->endJob();
		}
		jobHandle->state->NotifyReady();
	}
}


