#include "MainPresenter.hpp"
#include "PathEngine.hpp"
#include "InputHandler.hpp"
#include "Transaction.hpp"
#include <cassert>
#include <QtConcurrent/QtConcurrentRun>

MainPresenter::MainPresenter()
    : QObject()
    , m_inputHandler(std::make_unique<InputHandler>(*this))
    , m_outputHandler(std::make_unique<OutputHandler>())
    , m_engine(std::make_unique<PathEngine>())
{
    // Pass ready engine results to output handler
    QObject::connect(this, &MainPresenter::updateProgress,
        this, [this](size_t pathsFinished){
            if(!m_runningJob->isCancelled()) {
                std::println("Finished {} out of {} paths", pathsFinished, m_runningJob->totalPaths);
            }
        }, Qt::QueuedConnection);
    QObject::connect(this, &MainPresenter::resultDone,
        this, [this]{
            if(!m_runningJob->isCancelled()){
                m_outputHandler->onPathsReceived(m_runningJob->get()); // Won't block
            }
            m_runningJob.reset(); // Accept new transactions
        }, Qt::QueuedConnection);
}

MainPresenter::~MainPresenter() = default;

void MainPresenter::onTransactionReceived(const Transaction&& transaction){
    if(m_runningJob.has_value()) return;
    m_outputHandler->prepareGUI(transaction.pathQuery);
    auto& pq = transaction.pathQuery;
    m_outputHandler->onPDFReceived(getField(FieldTags::pdf{},
        pq.processDefinition.type,
        pq.processDefinition.startValueData,
        pq.simulationParameters.time,
        pq.processDefinition.drift.mu(),
        pq.processDefinition.diffusion.sigma()
    ));
    // Block for drift path
    m_outputHandler->onDriftDataReceived(m_engine->processPathQuery(transaction.deterministicQuery).get().at(0));
    m_runningJob.emplace(m_engine->processPathQuery(transaction.pathQuery));
    QThreadPool::globalInstance()->start([this]() -> void {
        while(!m_runningJob->isDone()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
            emit updateProgress(m_runningJob->pathsCompleted->load());
        }
        emit updateProgress(m_runningJob->totalPaths);
        emit resultDone();
        return;
    });
}

void MainPresenter::clear() const{
    m_outputHandler->clear();
}

void MainPresenter::cancel() {
    if(m_runningJob.has_value()) {
        m_runningJob->doCancel();
    }
}

InputHandler* MainPresenter::getInputHandler() const{
    return m_inputHandler.get();
}
OutputHandler* MainPresenter::getOutputHandler() const{
    return m_outputHandler.get();
}
