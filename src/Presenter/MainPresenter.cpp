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
    QObject::connect(m_jobHandler.get(), &JobHandler::jobMetaData,
        this, [this](size_t pathsCompleted, State minXT, State maxXT, State minXt, State maxXt){
            m_outputHandler->jobMetaData(pathsCompleted, minXT, maxXT, minXt, maxXt);
        }, Qt::QueuedConnection);

    QObject::connect(m_jobHandler.get(), &JobHandler::driftDone,
        this, [this](Path path){
            m_outputHandler->onDriftDataReceived(std::move(path));
        }, Qt::QueuedConnection);
    
    QObject::connect(m_jobHandler.get(), &JobHandler::fullPathsDone,
        this, [this](std::shared_ptr<StochasticFullPathsJob> job){
            m_outputHandler->onPathsReceived(job->fullPaths.get(),
                job->metaData->minXt.load(), job->metaData->maxXt.load());
        }, Qt::QueuedConnection);
    });

    QObject::connect(m_jobHandler.get(), &JobHandler::distributionDone,
        this, [this](std::shared_ptr<Job> job){
            Utils::assertTrue(job->distribution.wait_for(std::chrono::seconds(0)) == std::future_status::ready,
                "Expected distribution to be ready");

            Distribution distribution = job->distribution.get();
            switch (job->type) {
            case Job::Type::Stochastic:
                m_outputHandler->onDistributionReceived(std::move(distribution),
                    job->metaData->minXT.load(), job->metaData->maxXT.load());
                break;
            default:
                break;
            }
        }, Qt::QueuedConnection);
}

MainPresenter::~MainPresenter() = default;

void MainPresenter::onTransactionReceived(const Transaction& transaction){
    Utils::assertTrue(!m_jobHandler->jobRunning(), "Expected no job to be running");
    m_outputHandler->onStartTransaction(transaction.stochasticQuery);
    DeterministicJob deterministicJob = std::get<DeterministicJob>(m_engine->processQuery(transaction.deterministicQuery));
    StochasticJob stochasticJob = std::get<StochasticJob>(m_engine->processQuery(transaction.stochasticQuery));
    StochasticFullPathsJob stochasticFullPathJob = std::get<StochasticFullPathsJob>(m_engine->processQuery(transaction.stochasticFullPathQuery));
    m_jobHandler->postJobs(std::move(deterministicJob), std::move(stochasticJob), std::move(stochasticFullPathJob));
}

void MainPresenter::clear() const{
    m_outputHandler->clearGUI();
}

void MainPresenter::cancel() const {
    Utils::assertTrue(m_jobHandler->jobRunning(), "Expected a job to be running");
    m_outputHandler->cancelGUI();
    m_jobHandler->cancel();
}

InputHandler* MainPresenter::getInputHandler() const{
    return m_inputHandler.get();
}

OutputHandler* MainPresenter::getOutputHandler() const{
    return m_outputHandler.get();
}
