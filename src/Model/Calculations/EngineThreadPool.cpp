#include "EngineThreadPool.hpp"

EngineThreadPool::EngineThreadPool(size_t nrThreads)
    : m_threads{}
    , m_tasks()
    , m_stopSource()
    , m_cv()
    , m_taskMtx()
    , m_nrBusyThreads(0)
{
    nrThreads = std::max(std::thread::hardware_concurrency(), uint32_t(1));
    m_threads.reserve(nrThreads);
    std::stop_token st = m_stopSource.get_token();
    for (size_t i = 0; i < nrThreads; ++i) {
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

size_t EngineThreadPool::nrBusyThreads() const{
    return m_nrBusyThreads.load();
}

std::future<Path> EngineThreadPool::enqueue(std::function<Path()> func){
    std::packaged_task<Path()> task(std::move(func));
    std::future<Path> future = task.get_future();
    {
        std::unique_lock<std::mutex> lock(m_taskMtx);
        m_tasks.push(std::move(task));
    }
    m_cv.notify_one();
    return future;
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
        task(); // When finished, promise is fulfilled and future is ready
        m_nrBusyThreads--;
    }
}