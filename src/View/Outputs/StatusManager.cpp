#include "StatusManager.hpp"
#include "ViewUtils.hpp"
#include <unordered_map>
#include <QtCore/QString>
#include <QtWidgets/QLabel>
#include <QtWidgets/QHBoxLayout>

static const std::unordered_map<SolverType, QString> solverToString = {
    std::pair{SolverType::EULER_MARUYAMA, QString("Euler-Maruyama")},
    std::pair{SolverType::RUNGE_KUTTA, QString("Runge-Kutta")},
    std::pair{SolverType::MILSTEIN, QString("Milstein")}
};

StatusManager::StatusInfo::StatusInfo(QWidget* parent)
    : QGroupBox(parent)
    , currentStatus(new QLabel(this))
    , errorStatus(new QLabel(this))
    , progressBar(new QProgressBar(this))
{
    progressBar->setFormat("%v/%m");
    auto layout = new QVBoxLayout(this);
    layout->addWidget(currentStatus);
    layout->addWidget(errorStatus);
    layout->addWidget(progressBar);
    setLayout(layout);
}

StatusManager::StatusManager(QWidget* parent)
: QWidget(parent)
, m_queryInfo(new QGroupBox(this))
, m_statusInfo(new StatusInfo(this))
{
    auto layout = new QHBoxLayout(this);
    auto infoLayout = new QVBoxLayout(m_queryInfo);
    infoLayout->addWidget(new QLabel("", this));
    layout->addWidget(m_queryInfo, 3);
    layout->addWidget(m_statusInfo, 1);
    setStyleSheet(GUI::groupBoxDescription());
    setReady();
}

void StatusManager::setProgress(const size_t pathsCompleted){
    m_statusInfo->progressBar->setValue(static_cast<int>(pathsCompleted));
    if (pathsCompleted == m_statusInfo->progressBar->maximum()){
        m_statusInfo->currentStatus->setText("Rendering...");
        m_statusInfo->currentStatus->repaint();
    }
}

void StatusManager::setReady(){
    m_statusInfo->currentStatus->setText("Ready");
}

void StatusManager::prepareStatusInfo(const size_t totalPaths){
    m_statusInfo->progressBar->setRange(0, static_cast<int>(totalPaths));
    m_statusInfo->progressBar->setValue(0);
    m_statusInfo->currentStatus->setText("Sampling...");
}

void StatusManager::setQueryInfo(const PathQuery& query){
    QString infoDefinition;
    QString infoParams;
    QTextStream streamParams(&infoParams);
    streamParams
                << QString::fromUtf8( getField(FieldTags::name{}, query.processDefinition.type) ) << ": "
                << QString::fromUtf8( getField(FieldTags::definition{}, query.processDefinition.type) ) << "\n"
                << "μ = " << QString::number(query.processDefinition.drift.mu()) << ", "
                << "σ = " << QString::number(query.processDefinition.diffusion.sigma()) << ", "
                << "X" << QString::fromUtf8("\u2080") << " = " << QString::number(query.processDefinition.startValue) << "\n"
                << "Solver = " << solverToString.at(query.simulationParameters.solver) << ", "
                << "T = " << QString::number(query.simulationParameters.time) << ", "
                << "dt = " << QString::number(query.simulationParameters.dt) << ", "
                << "Samples = " << QString::number(query.simulationParameters.samples) << "\n"
                << "Multithreading = " << (query.settingsParameters.useThreading ? "Yes" : "No") << ", "
                << "Seed: " << (query.settingsParameters.seed ? QString::number(query.settingsParameters.seed.value()) : "Random");
    m_queryInfo->findChild<QLabel*>()->setText(infoParams);
}

void StatusManager::clear(){
    m_queryInfo->findChild<QLabel*>()->setText("");
    setReady();
    m_statusInfo->progressBar->setRange(0, 1);
    m_statusInfo->progressBar->setValue(0);
}

void StatusManager::cancel(){
    m_statusInfo->currentStatus->setText("Cancelling...");
    m_statusInfo->currentStatus->repaint();
}
