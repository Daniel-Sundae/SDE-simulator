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
                m_outputHandler->onDriftDataReceived(std::move(paths.front()));
                break;
            case Job::Type::Stochastic:
                m_outputHandler->onPathsReceived(std::move(paths));
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

            Distribution distribution = job->distribution.get();
            switch (job->type) {
            case Job::Type::Stochastic:
                m_outputHandler->onDistributionReceived(std::move(distribution),
                    job->atomicData->minPathEndVal.load(), job->atomicData->maxPathEndVal.load());
                break;
            default:
                break;
            }
        }, Qt::QueuedConnection);
}

MainPresenter::~MainPresenter() = default;

void MainPresenter::onTransactionReceived(const Transaction& transaction){
    Utils::assertTrue(!m_jobHandler->jobRunning(), "Expected no job to be running");
    m_outputHandler->onStartTransaction(transaction.pathQuery);
    Job deterministicJob = m_engine->processPathQuery(transaction.deterministicQuery);
    deterministicJob.type = Job::Type::Deterministic;
    Job stochasticJob = m_engine->processPathQuery(transaction.pathQuery);
    stochasticJob.type = Job::Type::Stochastic;
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
