#include "ThreadPool.hpp"

EngineThreadPool::EngineThreadPool(unsigned int nrThreads)
	: m_threads{}
	, m_tasks()
	, m_stopSource()
{
	m_stopSource.get_token();
	if (!nrThreads) {
		nrThreads = std::thread::hardware_concurrency();
	}
	m_threads.reserve(nrThreads);
	for (unsigned int i = 0; i < number_of_threads; ++i) {
		m_threads.emplace_back(std::jthread());
	}
}