#include "MainPresenter.hpp"
#include "PathEngine.hpp"
#include "InputHandler.hpp"
#include "Transaction.hpp"
#include "JobHandler.hpp"
#include "Utils.hpp"
#include <cassert>
#include <iostream>

static std::atomic<size_t> s_currentTransaction{0};

MainPresenter::MainPresenter()
    : QObject()
    , m_inputHandler(std::make_unique<InputHandler>(*this))
    , m_outputHandler(std::make_unique<OutputHandler>())
    , m_engine(std::make_unique<PathEngine>())
    , m_jobHandler(std::make_unique<JobHandler>())
{
    // Pass ready engine results to output handler
    QObject::connect(m_jobHandler.get(), &JobHandler::jobProgress,
        this, [this](size_t pathsFinished){
            m_outputHandler->jobProgress(pathsFinished);
        }, Qt::QueuedConnection);
    QObject::connect(m_jobHandler.get(), &JobHandler::fullPathsDone,
        this, [this](std::shared_ptr<Job> job){
            Paths paths = job->fullPaths.get();
            switch (job->type) {
            case Job::Type::Deterministic:
                Utils::assertTrue(paths.size() == 1,
                    "Expected deterministic job to return exactly one path, got: {}", paths.size());
                m_outputHandler->onDriftDataReceived(paths.front());
                break;
            case Job::Type::Stochastic:
                m_outputHandler->onPathsReceived(paths);
                break;
            default:
                Utils::fatalError("Unknown job type received in MainPresenter");
                break;
            }
        }, Qt::QueuedConnection);
    QObject::connect(m_jobHandler.get(), &JobHandler::distributionDone,
        this, [this](std::shared_ptr<Job> job){
            Utils::assertTrue(job->distribution.wait_for(std::chrono::seconds(0)) == std::future_status::ready,
                "Expected distribution to be ready");
            if (job->transactionNr != s_currentTransaction.load()) {
                std::clog << "Dropping stale transaction" << std::endl;
                std::clog << "Stale transaction: " << job->transactionNr << std::endl;
                std::clog << "Current transaction: " << s_currentTransaction.load() << std::endl;
                return;
            }

            Distribution distribution = job->distribution.get();
            switch (job->type) {
            case Job::Type::Stochastic:
                m_outputHandler->onDistributionReceived(std::move(distribution));
                break;
            default:
                break;
            }
        }, Qt::QueuedConnection);
}

MainPresenter::~MainPresenter() = default;

void MainPresenter::onTransactionReceived(const Transaction& transaction){
    if (m_jobHandler->jobRunning()) {
        m_outputHandler->setError(ErrorType::BUSY_ENGINE);
        return;
    }
    m_outputHandler->onStartTransaction(transaction.pathQuery);
    const auto& pq = transaction.pathQuery;
    m_outputHandler->onPDFReceived(getField(FieldTags::pdf{},
        pq.processDefinition.type,
        pq.processDefinition.startValue,
        pq.simulationParameters.time,
        pq.processDefinition.drift.mu(),
        pq.processDefinition.diffusion.sigma()
    ));
    Job deterministicJob = m_engine->processPathQuery(transaction.deterministicQuery);
    deterministicJob.type = Job::Type::Deterministic;
    Job stochasticJob = m_engine->processPathQuery(transaction.pathQuery);
    stochasticJob.type = Job::Type::Stochastic;
    const size_t transactionNr = ++s_currentTransaction;
    deterministicJob.transactionNr = transactionNr;
    stochasticJob.transactionNr = transactionNr;
    m_jobHandler->postJobs(std::move(deterministicJob), std::move(stochasticJob));
}

void MainPresenter::clear() const{
    m_outputHandler->clearGUI();
}

void MainPresenter::cancel() const {
    if (!m_jobHandler->jobRunning()) return;
    m_outputHandler->cancelGUI();
    m_jobHandler->cancel();
}

InputHandler* MainPresenter::getInputHandler() const{
    return m_inputHandler.get();
}

OutputHandler* MainPresenter::getOutputHandler() const{
    return m_outputHandler.get();
}
