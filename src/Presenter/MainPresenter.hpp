#pragma once

#include <memory>
#include <semaphore>
#include "OutputHandler.hpp"
#include "PathEngine.hpp"
#include <QtCore/QFutureWatcher>

struct PathQuery;
class InputHandler;
struct Transaction;
class JobHandler;

class MainPresenter : public QObject {
    Q_OBJECT
public:
    explicit MainPresenter();
    ~MainPresenter(); // Out-of-line destructor for QT
    void clear() const;
    void cancel() const;
    InputHandler* getInputHandler() const;
    OutputHandler* getOutputHandler() const;
    void onTransactionReceived(Transaction&& transaction);
private:
    std::unique_ptr<InputHandler> m_inputHandler;
    std::unique_ptr<OutputHandler> m_outputHandler;
    std::unique_ptr<PathEngine> m_engine;
    std::unique_ptr<JobHandler> m_jobHandler;
};
