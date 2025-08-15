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
    , m_pathsWatcher(new QFutureWatcher<Paths>(this))
    , m_driftWatcher(new QFutureWatcher<Path>(this))
{
    // Pass ready engine results to output handler
    QObject::connect(m_pathsWatcher, &QFutureWatcher<Paths>::finished, this, [this]{
        m_outputHandler->onPathsReceived(m_pathsWatcher->result());
    });
    QObject::connect(m_driftWatcher, &QFutureWatcher<Path>::finished, this, [this]{
        m_outputHandler->onDriftDataReceived(m_driftWatcher->result());
    });
}

MainPresenter::~MainPresenter() = default;

void MainPresenter::onTransactionReceived(const Transaction&& transaction){
    if(m_engine->isBusy()){
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
    QFuture<Path> driftFuture = QtConcurrent::run([f2 = m_engine->samplePathsAsync(transaction.deterministicQuery)]() mutable -> Path {
        return f2.get().front();
    });
    QFuture<Paths> pathFuture = QtConcurrent::run([f = m_engine->samplePathsAsync(transaction.pathQuery)]() mutable -> Paths {
        return f.get();
    });
    m_driftWatcher->setFuture(std::move(driftFuture));
    m_pathsWatcher->setFuture(std::move(pathFuture));
}

void MainPresenter::clear() const{
    if(m_engine->isBusy()){
        return;
    }
    m_outputHandler->clear();
}

void MainPresenter::cancel() const{
    m_engine->requestCancel();
}

InputHandler* MainPresenter::getInputHandler() const{
    return m_inputHandler.get();
}
OutputHandler* MainPresenter::getOutputHandler() const{
    return m_outputHandler.get();
}
