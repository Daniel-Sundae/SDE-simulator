#include "EngineThreadPool.hpp"

EngineThreadPool::EngineThreadPool(std::uint32_t nrThreads)
	: m_threads{}
	, m_tasks()
	, m_stopSource()
	, m_cv()
	, m_taskMtx()
	, m_nrBusyThreads(0)
{
	if (!nrThreads) {
		nrThreads = std::thread::hardware_concurrency();
	}
	// Need at least 2 threads. One mainTask thread and 1 worker thread.
	nrThreads = std::max(nrThreads, static_cast<std::uint32_t>(2));
	m_threads.reserve(nrThreads);
	std::stop_token st = m_stopSource.get_token();
	for (std::uint32_t i = 0; i < nrThreads; ++i) {
		m_threads.emplace_back([this, st](std::stop_token) {this->doTasks(st);}); // Do not let jthread create stoptoken.
	}
}

EngineThreadPool::~EngineThreadPool()
{
	clearTasks();
	{
		std::unique_lock<std::mutex> lock(m_taskMtx);
		m_stopSource.request_stop();
	}
	m_cv.notify_all(); // Kill threads
}

void EngineThreadPool::clearTasks(){
	std::scoped_lock lock(m_taskMtx);
	while (!m_tasks.empty()) {
		m_tasks.pop();
	}
}

std::uint32_t EngineThreadPool::nrBusyThreads() const{
	return m_nrBusyThreads.load();
}

void EngineThreadPool::enqueue(std::function<void()> task){
	{
		std::unique_lock<std::mutex> lock(m_taskMtx);
		m_tasks.push(std::move(task));
	}
	m_cv.notify_one();
}

void EngineThreadPool::doTasks(std::stop_token st)
{
	Task task;
	while (!st.stop_requested()) {
		{
			std::unique_lock<std::mutex> lock(m_taskMtx);
			m_cv.wait(lock, [this, &st]() { return st.stop_requested() || !m_tasks.empty(); });
			if (st.stop_requested()) {
				return; // Kill this thread
			}
			task = std::move(m_tasks.front());
			m_tasks.pop();
		}
		m_nrBusyThreads++;
		task();
		m_nrBusyThreads--;
	}
}