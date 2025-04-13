#include "EngineThreadPool.hpp"

EngineThreadPool::EngineThreadPool(unsigned int nrThreads)
	: m_threads{}
	, m_tasks()
	, m_stopSource()
	, m_cv()
	, m_taskMtx()
{
	if (!nrThreads) {
		nrThreads = std::thread::hardware_concurrency();
	}
	m_threads.reserve(nrThreads);
	std::stop_token st = m_stopSource.get_token();
	for (unsigned int i = 0; i < nrThreads; ++i) {
		m_threads.emplace_back([this, st](std::stop_token) {this->DoTasks(st);}); // Do not let jthread create stoptoken.
		//m_threads.emplace_back([this](std::stop_token st) {this->DoTasks(st);}); // Let jthread create stoptoken.
	}
}

EngineThreadPool::~EngineThreadPool()
{
	ClearTasks();
	{
		std::unique_lock<std::mutex> lock(m_taskMtx);
		m_stopSource.request_stop();
	}
	m_cv.notify_all(); // Kills threads
}

auto EngineThreadPool::ClearTasks() -> void
{
	std::queue<Task> empty;
	{
		std::scoped_lock lock(m_taskMtx);
		std::swap(m_tasks, empty);
	}
}

auto EngineThreadPool::NrTasks() const -> std::size_t
{
	std::scoped_lock lock(m_taskMtx);
	return m_tasks.size();
}

auto EngineThreadPool::Enqueue(std::function<Path()> f) -> std::future<Path>
{
	std::packaged_task<Path()> task(std::move(f));
	std::future<Path> future = task.get_future();
	{
		std::unique_lock<std::mutex> lock(m_taskMtx);
		m_tasks.push(std::move(task));
	}
	m_cv.notify_one();
	return future;
}

void EngineThreadPool::DoTasks(std::stop_token st)
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
		task(); // Execute task
	}
}