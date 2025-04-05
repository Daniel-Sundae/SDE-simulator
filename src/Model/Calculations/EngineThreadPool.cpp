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
	nrThreads = 1;
	m_threads.reserve(nrThreads);
	std::stop_token st = m_stopSource.get_token();
	for (unsigned int i = 0; i < nrThreads; ++i) {
		m_threads.emplace_back([this, st](std::stop_token) {this->DoTasks(st);}); // Do not let jthread create stoptoken.
		//m_threads.emplace_back([this](std::stop_token st) {this->DoTasks(st);}); // Let jthread create stoptoken.
	}
}

auto EngineThreadPool::Enqueue(std::function<Path()> f, Priority prio) -> std::future<Path>
{
	std::packaged_task<Path()> task(std::move(f));
	std::future<Path> future = task.get_future();
	if(prio == Priority::HIGH){
		m_tasks->PushFront(std::move(task));
	}
	else if(prio == Priority::LOW){
		m_tasks->PushBack(std::move(task));
	}
	else{
		throw std::runtime_error("Priority not implemented");
	}
	return future;
}

void EngineThreadPool::Stop()
{
	m_stopSource.request_stop();
	// Alternatively, if jthreads have their own stopsource
	/*for (auto& t : m_threads) {
		t.request_stop();
	}*/
}

void EngineThreadPool::DoTasks(std::stop_token st)
{
	while (!st.stop_requested()) {
		/*m_condition.wait*/
		auto task = m_tasks->Pop();
		if (task.has_value()) {
			task.value()(); // Execute task
		}
	}
}