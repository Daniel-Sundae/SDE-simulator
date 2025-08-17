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
    , m_runningJob(nullptr)
{
    // Pass ready engine results to output handler
    QObject::connect(this, &MainPresenter::updateProgress,
        this, [this](size_t pathsFinished, size_t totalPaths){
            std::println("Finished {} out of {} paths", pathsFinished, totalPaths);
        }, Qt::QueuedConnection);
    QObject::connect(this, &MainPresenter::consumePaths,
        this, [this](Paths paths){
            m_outputHandler->onPathsReceived(std::move(paths));
        }, Qt::QueuedConnection);
    m_jobMonitorer = std::jthread([this](std::stop_token token) -> void {
        while(true){
            m_jobAvailable.acquire();
            if(token.stop_requested()) return;
            auto job = m_runningJob.load();
            if(!job) {
                Utils::fatalError("Expected job to be present");
            }
            while(!job->isDone()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(DefaultConstants::loadingbarUpdateRate));
                emit updateProgress(job->pathsCompleted->load(), job->totalPaths);
            }
            if(!job->isCancelled()){
                emit consumePaths(job->get());
            }
            if(!m_runningJob.exchange(nullptr)) {
                Utils::fatalError("Some other thread removed the job");
            }
        }
    });
}

MainPresenter::~MainPresenter(){
    m_jobMonitorer.request_stop();
    m_jobAvailable.release(); // Ensure monitorer wakes up
    m_jobMonitorer.join();
    m_runningJob = nullptr;
}

void MainPresenter::onTransactionReceived(const Transaction&& transaction){
    if(m_runningJob.load()) {
        m_outputHandler->raiseError(ErrorType::BUSY_ENGINE);
        return;
    }
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
    m_runningJob.store(std::make_shared<Job>(m_engine->processPathQuery(transaction.pathQuery)));
    // Notify monitorer thread that job exists
    m_jobAvailable.release();
}

void MainPresenter::clear() const{
    m_outputHandler->clearGUI();
}

void MainPresenter::cancel() {
    // Copy to prevent untimely pointer reset
    if(auto job = m_runningJob.load()){
        job->doCancel();
    }
    m_outputHandler->clearGUI();
}

InputHandler* MainPresenter::getInputHandler() const{
    return m_inputHandler.get();
}
OutputHandler* MainPresenter::getOutputHandler() const{
    return m_outputHandler.get();
}
