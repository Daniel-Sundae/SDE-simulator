#pragma once

#include <memory>
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
    ~MainPresenter(); // Out of line needed here
    void clear() const;
    void cancel() const;
    InputHandler* getInputHandler() const;
    OutputHandler* getOutputHandler() const;
    void onTransactionReceived(const Transaction &&transaction);
private:
    std::unique_ptr<InputHandler> m_inputHandler = std::make_unique<InputHandler>(*this);
    std::unique_ptr<OutputHandler> m_outputHandler = std::make_unique<OutputHandler>();
    std::unique_ptr<PathEngine> m_engine = std::make_unique<PathEngine>();
    QFutureWatcher<Job>* m_pathsWatcher = new QFutureWatcher<Job>(this);
    QFutureWatcher<Job>* m_driftWatcher = new QFutureWatcher<Job>(this);
    std::array<Job, Transaction::numQueries> m_runningJobs = {};
};
