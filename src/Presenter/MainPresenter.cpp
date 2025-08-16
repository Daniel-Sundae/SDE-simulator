#include "MainPresenter.hpp"
#include "PathEngine.hpp"
#include "InputHandler.hpp"
#include "Transaction.hpp"
#include <cassert>
#include <QtConcurrent/QtConcurrentRun>

MainPresenter::MainPresenter()
    : QObject()
{
    // Pass ready engine results to output handler
    QObject::connect(m_pathsWatcher, &QFutureWatcher<Job>::finished, this, [this]{
        m_outputHandler->onPathsReceived(m_pathsWatcher->result());
    });
    QObject::connect(m_driftWatcher, &QFutureWatcher<Job>::finished, this, [this]{
        m_outputHandler->onDriftDataReceived(m_driftWatcher->result());
    });
}

MainPresenter::~MainPresenter() = default;

void MainPresenter::onTransactionReceived(const Transaction&& transaction){
    if(m_runningJobs){
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
    m_runningJobs = m_engine->processTransaction(transaction);
    QFuture<std::array<Job, Transaction::numQueries>> jobs = QtConcurrent::run([jobs = m_runningJobs]() mutable -> Path {
        deterministicJob = jobs[0].getResult();
        stochasticJob = jobs[1].getResult();
    });
    m_driftWatcher->setFuture(std::move(jobs));
}

void MainPresenter::clear() const{
    if(m_engine->isBusy()){
        return;
    }
    m_outputHandler->clear();
}

void MainPresenter::cancel() const{
    m_engine->cancelJobs();
}

InputHandler* MainPresenter::getInputHandler() const{
    return m_inputHandler.get();
}
OutputHandler* MainPresenter::getOutputHandler() const{
    return m_outputHandler.get();
}
