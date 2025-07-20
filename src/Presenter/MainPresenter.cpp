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

void MainPresenter::samplePaths(const PathQuery& pQuery){
    m_engine->samplePathsAsync(
        pQuery,
        [this](Paths results) mutable {
            listener()->handleWorkerResult(std::move(results));
        }
    );
}

void MainPresenter::sampleDriftCurve(const PathQuery& deterministicQuery){
    if(deterministicQuery.simulationParameters.samples != 1){
        Utils::fatalError("Deterministic query should have exactly one sample, got: {}",
            deterministicQuery.simulationParameters.samples);
    }
    listener()->onDriftDataReceived(m_engine->sampleOnePath(deterministicQuery));
}

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
    samplePaths(transaction.pathQuery);
    sampleDriftCurve(transaction.deterministicQuery);
}

void MainPresenter::clear() const{
    if(m_engine->isBusy()){
        return;
    }
    listener()->clear();
}

void MainPresenter::cancel() const{
    m_engine->requestcancel();
}

InputHandler* MainPresenter::getInputHandler() const{
    return m_inputHandler.get();
}
OutputHandler* MainPresenter::getOutputHandler() const{
    return m_outputHandler.get();
}
