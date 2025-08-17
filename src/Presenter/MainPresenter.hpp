#pragma once

#include <memory>
#include <semaphore>
#include "OutputHandler.hpp"
#include "PathEngine.hpp"
#include <QtCore/QFutureWatcher>

struct PathQuery;
class InputHandler;
struct Transaction;

class MainPresenter final : public QObject {
    Q_OBJECT
public:
    explicit MainPresenter();
    ~MainPresenter();
    void clear() const;
    void cancel();
    InputHandler* getInputHandler() const;
    OutputHandler* getOutputHandler() const;
    void onTransactionReceived(const Transaction &&transaction);
private:
    std::unique_ptr<InputHandler> m_inputHandler;
    std::unique_ptr<OutputHandler> m_outputHandler;
    std::unique_ptr<PathEngine> m_engine;
    std::atomic<std::shared_ptr<Job>> m_runningJob;
    std::binary_semaphore m_jobAvailable{0};
    std::jthread m_jobMonitorer;
signals:
    void updateProgress(size_t pathsFinished, size_t totalPaths);
    void consumePaths(Paths paths);
};
