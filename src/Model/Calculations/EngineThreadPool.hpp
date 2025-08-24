#pragma once
#include "Types.hpp"
#include "PathQuery.hpp"
#include <queue>
#include <future>
#include <thread>
#include <expected>

class EngineThreadPool {
public:
    explicit EngineThreadPool(size_t nrThreads = 0);
    ~EngineThreadPool();
    template<class F>
    requires std::invocable<F&> &&
        (std::same_as<std::remove_cvref_t<std::invoke_result_t<F&>>, Path> ||
        std::same_as<std::remove_cvref_t<std::invoke_result_t<F&>>, std::optional<State>>)
    auto enqueue(F&& f) -> std::future<std::remove_cvref_t<std::invoke_result_t<F&>>>
    {
        using R = std::remove_cvref_t<std::invoke_result_t<F&>>;
        std::packaged_task<R()> task(std::forward<F>(f));
        auto fut = task.get_future();
        {
            std::scoped_lock lock(m_taskMtx);
            // Type erase to allow for multiple task types
            // This pattern makes sense since workers don't care about the result
            m_tasks.push([t = std::move(task)]() mutable -> void { t(); });
        }
        m_cv.notify_one();
        return fut;
    }

    size_t nrBusyThreads() const;
private:
    void doTasks();
private:
    std::vector<std::thread> m_threads{};
    std::queue<std::move_only_function<void()>> m_tasks{};
    std::condition_variable m_cv{};
    std::mutex m_taskMtx{};
    std::atomic<uint32_t> m_nrBusyThreads{0};
    std::atomic<bool> m_shutdownInProgress{false};
};