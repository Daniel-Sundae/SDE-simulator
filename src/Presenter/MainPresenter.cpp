#include "MainPresenter.hpp"
#include "PathEngine.hpp"
#include "InputHandler.hpp"
#include "Transaction.hpp"
#include "JobHandler.hpp"
#include "Utils.hpp"
#include <cassert>
#include <QtConcurrent/QtConcurrentRun>

MainPresenter::MainPresenter()
    : QObject()
    , m_inputHandler(std::make_unique<InputHandler>(*this))
    , m_outputHandler(std::make_unique<OutputHandler>())
    , m_engine(std::make_unique<PathEngine>())
    , m_jobHandler(std::make_unique<JobHandler>())
{
    // Pass ready engine results to output handler
    QObject::connect(m_jobHandler.get(), &JobHandler::jobProgress,
        this, [this](size_t pathsFinished, size_t totalPaths){
            std::println("Finished {} out of {} paths", pathsFinished, totalPaths);
        }, Qt::QueuedConnection);
    QObject::connect(m_jobHandler.get(), &JobHandler::jobStatus,
        this, [this](Job::Status status){
            std::println("Job status updated: {}", std::to_underlying(status));
        }, Qt::QueuedConnection);
    QObject::connect(m_jobHandler.get(), &JobHandler::jobDone,
        this, [this](Paths paths, Job::Type type, size_t jobNr){
            switch (type) {
            case Job::Type::Deterministic:
                if(jobNr != Job::s_deterministicJobCounter.load()) return; // Drop stale job
                m_outputHandler->onDriftDataReceived(std::move(paths.at(0)));
                break;
            case Job::Type::Stochastic:
                if(jobNr != Job::s_stochasticJobCounter.load()) return; // Drop stale job
                m_outputHandler->onPathsReceived(std::move(paths));
                break;  
            default:
                Utils::fatalError("Unknown job type received in MainPresenter");
                break;
            }
        }, Qt::QueuedConnection);
}

MainPresenter::~MainPresenter() = default;

void MainPresenter::onTransactionReceived(Transaction&& transaction){
    if(m_jobHandler->jobRunning()) {
        m_outputHandler->setError(ErrorType::BUSY_ENGINE);
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
    m_jobHandler->postJob(m_engine->processPathQuery(transaction.deterministicQuery));
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // TEMP SLEEP SINCE BUFFER IS OF SIZE 1
    m_jobHandler->postJob(m_engine->processPathQuery(transaction.pathQuery));
}

void MainPresenter::clear() const{
    m_outputHandler->clearGUI();
}

void MainPresenter::cancel() {
    m_outputHandler->clearGUI();
    m_jobHandler->cancel();    
}

InputHandler* MainPresenter::getInputHandler() const{
    return m_inputHandler.get();
}

OutputHandler* MainPresenter::getOutputHandler() const{
    return m_outputHandler.get();
}
