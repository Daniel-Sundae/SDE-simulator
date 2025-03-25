#include "ThreadPool.hpp"

ThreadPool::ThreadPool() {
	const unsigned int number_of_threads = std::thread::hardware_concurrency();
	m_threads.reserve(number_of_threads);
}