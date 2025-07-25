#include "MainPresenter.hpp"
#include "PathEngine.hpp"
#include "InputHandler.hpp"
#include "Transaction.hpp"
#include <cassert>

MainPresenter::MainPresenter()
    : IPresenterComponent()
    , m_inputHandler(std::make_unique<InputHandler>())
    , m_outputHandler(std::make_unique<OutputHandler>())
    , m_engine(std::make_unique<PathEngine>())
{}

void MainPresenter::onTransactionReceived(const Transaction&& transaction){
    if(m_engine->isBusy()){
        return;
    }
    listener()->prepareGUI(transaction.pathQuery);
    auto& pq = transaction.pathQuery;
    listener()->onPDFReceived(getField(FieldTags::pdf{},
        pq.processDefinition.type,
        pq.processDefinition.startValueData,
        pq.simulationParameters.time,
        pq.processDefinition.drift.mu(),
        pq.processDefinition.diffusion.sigma()
    ));
    std::future<Paths> pathsFut = m_engine->samplePathsAsync(transaction.pathQuery);
    std::future<Paths> driftFut = m_engine->samplePathsAsync(transaction.deterministicQuery);
}

void MainPresenter::clear() const{
    if(m_engine->isBusy()){
        return;
    }
    listener()->clear();
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
