#include "StatusManager.hpp"
#include "ViewUtils.hpp"
#include <unordered_map>
#include <QtCore/QString>
#include <QtWidgets/QLabel>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QGroupBox>

static const std::unordered_map<StatusSignal, QString> statusToString = {
    {StatusSignal::READY, QString("Ready")},
    {StatusSignal::SAMPLING, QString("Sampling...")},
    {StatusSignal::RENDERING, QString("Rendering...")}
};

static const std::unordered_map<SolverType, QString> solverToString = {
    std::pair{SolverType::EULER_MARUYAMA, QString("Euler-Maruyama")},
    std::pair{SolverType::RUNGE_KUTTA, QString("Runge-Kutta")},
    std::pair{SolverType::MILSTEIN, QString("Milstein")}
};

StatusManager::StatusManager(QWidget* parent)
: QWidget(parent)
, m_queryDefinition(new QGroupBox(this))
, m_queryParameters(new QGroupBox(this))
, m_statusInfo(new QGroupBox(this))
, m_status(StatusSignal::READY)
{
    auto layout = new QHBoxLayout(this);
    auto qdefLayout = new QVBoxLayout(m_queryDefinition);
    auto qparLayout = new QVBoxLayout(m_queryParameters);
    auto statusLayout = new QVBoxLayout(m_statusInfo);
    qdefLayout->addWidget(new QLabel("", this));
    qparLayout->addWidget(new QLabel("", this));
    statusLayout->addWidget(new QLabel("Ready", this));
    layout->addWidget(m_queryDefinition, 3);
    layout->addWidget(m_queryParameters, 5);
    layout->addWidget(m_statusInfo, 1);
    setStyleSheet(GUI::groupBoxDescription());
}

void StatusManager::setStatus(const StatusSignal s){
    m_status = s;
    m_statusInfo->findChild<QLabel*>()->setText(statusToString.at(s));
}

void StatusManager::setQueryInfo(const PathQuery& pQuery){
    QString infoDefinition;
    QTextStream streamDefinition(&infoDefinition);
    streamDefinition << "Process: " << QString::fromUtf8(ProcessData::getName(pQuery.processDefinition.type)) << "\n"
                     << "Definition: " << QString::fromUtf8(ProcessData::getDefinition(pQuery.processDefinition.type));
    QString infoParams;
    QTextStream streamParams(&infoParams);
    streamParams << "Definition parameters: μ = " << QString::number(pQuery.processDefinition.drift.mu()) << ", σ = " << QString::number(pQuery.processDefinition.diffusion.sigma()) << ", X" << QString::fromUtf8("\u2080") << " = " << QString::number(pQuery.processDefinition.startValueData) << "\n"
                 << "Simulation parameters: Solver = " << solverToString.at(pQuery.simulationParameters.solver) << ", Time = " << QString::number(pQuery.simulationParameters.time) << ", dt = " << QString::number(pQuery.simulationParameters.dt) << ", Samples = " << QString::number(pQuery.simulationParameters.samples) << "\n"
                 << "Settings: Multithreading = " << (pQuery.settingsParameters.useThreading ? "Yes" : "No") << ", Seed: " << (pQuery.settingsParameters.useSeed.first ? QString::number(pQuery.settingsParameters.useSeed.second) : "Random");
    m_queryDefinition->findChild<QLabel*>()->setText(infoDefinition);
    m_queryParameters->findChild<QLabel*>()->setText(infoParams);
}

void StatusManager::clearStatus(){
    m_queryDefinition->findChild<QLabel*>()->setText("");
    m_queryParameters->findChild<QLabel*>()->setText("");
}
