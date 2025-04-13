#include "EngineThreadPool.hpp"

EngineThreadPool::EngineThreadPool(unsigned int nrThreads)
	: m_threads{}
	, m_tasks(std::make_unique<TaskQueue>())
	, m_stopSource()
	, m_cv()
	, m_taskMtx()
{
	if (!nrThreads) {
		nrThreads = std::thread::hardware_concurrency();
	}
	nrThreads = 1;
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
	{
		std::unique_lock<std::mutex> lock(m_taskMtx);
		m_tasks->Clear(); // Perhaps not needed? std::deque<std::packaged_task> will probably be cleared automatically
	}
}

auto EngineThreadPool::Enqueue(std::function<Path()> f, Priority prio) -> std::future<Path>
{
	std::packaged_task<Path()> task(std::move(f));
	std::future<Path> future = task.get_future();
	{
		std::unique_lock<std::mutex> lock(m_taskMtx);
		if(prio == Priority::HIGH){
			m_tasks->PushFront(std::move(task));
		}
		else if(prio == Priority::LOW){
			m_tasks->PushBack(std::move(task));
		}
		else{
			throw std::runtime_error("Priority not implemented");
		}
	}
	m_cv.notify_one();
	return future;
}

void EngineThreadPool::DoTasks(std::stop_token st)
{
	std::optional<Task> task;
	while (!st.stop_requested()) {
		{
			std::unique_lock<std::mutex> lock(m_taskMtx);
			m_cv.wait(lock, [this, &st]() { return st.stop_requested() || !m_tasks->Empty(); });
			if (st.stop_requested()) {
				return; // Kill this thread
			}
			task = m_tasks->Pop();
		}
		if (!task.has_value()) {
			throw std::logic_error("Task queue invariant violated: Pop returned empty after non-empty check");
		}
		task.value()(); // Execute task
	}
}