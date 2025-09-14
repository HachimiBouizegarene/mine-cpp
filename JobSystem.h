#pragma once
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <functional>


namespace RenewEngine {

	class JobState {
		std::mutex m_mutex;
		std::condition_variable m_cv;
		bool m_ready = false;
	public:

		inline void Wait()
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_cv.wait(lock, [this]() {
				return m_ready;
			});
		}
		inline void NotifyReady() {
			m_ready = true;
			m_cv.notify_one();
		}
	};


	struct JobHandle {
		std::shared_ptr<JobState> state = nullptr;
		std::function<void()> job = nullptr;
		std::function<void()> endJob = nullptr;
	};
	

	class JobSystem {
	public : 
		JobSystem(size_t numWorkers = std::thread::hardware_concurrency());
		~JobSystem();
		void Stop();
		std::shared_ptr<JobState> RegisterJob(std::function<void()> fn);
		std::shared_ptr<JobState> RegisterJob(std::function<void()> fn, std::function<void()> onEndFn);
	private:
		void ThreadLoop();
	private :
		size_t m_numWorkers;
		std::vector<std::thread> m_workers;
		std::queue<std::unique_ptr<JobHandle>> m_jobs;
		std::condition_variable m_cv;
		std::mutex m_mutex;
		std::atomic<bool> m_shouldQuit = false;
	};
}