#include "EngineThreadPool.hpp"

EngineThreadPool::EngineThreadPool(unsigned int nrThreads)
	: m_threads{}
	, m_tasks(std::make_unique<TaskQueue>())
	, m_stopSource()
	, m_goSignal()
{
	if (!nrThreads) {
		nrThreads = std::thread::hardware_concurrency();
	}
	m_threads.reserve(nrThreads);
	for (unsigned int i = 0; i < nrThreads; ++i) {
		m_threads.emplace_back(&DoWork, this, m_stopSource.get_token());
	}
}

auto EngineThreadPool::Enqueue(std::function<Path()> f, TaskPriority prio) -> std::future<Path>
{
	std::promise<Path> promise;

	std::future<Path> future = promise.get_future();
	auto task = [callable = std::move(f),
		promise = std::move(promise)]() mutable {
		promise.set_value(f());
	};
	if(prio == TaskPriority::HIGH){
		m_tasks->PushFront(task);
	}
	else {
		m_tasks->PushBack(task);
	}
	return future;
}

void EngineThreadPool::Stop()
{
	int dummy = 0;
	(void)dummy;
}

void EngineThreadPool::DoWork(std::stop_token st)
{
	while (!st.stop_requested()) {
		/*m_condition.wait*/
		while (!m_tasks->Empty()) {
			m_tasks->Pop();
		}
	}
}