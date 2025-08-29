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

static void initGridLayout(QGridLayout* grid) {
    grid->setContentsMargins(6, 4, 6, 6);
    grid->setHorizontalSpacing(8);
    grid->setVerticalSpacing(2);
}

StatusManager::QueryInfo::QueryInfo(QWidget* parent)
    : QGroupBox("Query", parent)
    , infoLabel(new QLabel(this))
{
    auto* grid = new QGridLayout(this);
    initGridLayout(grid);
    setLayout(grid);
    grid->addWidget(infoLabel);
}

StatusManager::ResultInfo::ResultInfo(QWidget* parent = nullptr)
    : QGroupBox("Results", parent)
    , minEndValue(new QLabel(this))
    , maxEndValue(new QLabel(this))
    , minPathValue(new QLabel(this))
    , maxPathValue(new QLabel(this))
    , expectedValue(new QLabel(this))
    , stdDevValue(new QLabel(this))
{
    auto* grid = new QGridLayout(this);
    initGridLayout(grid);
    setLayout(grid);

    grid->addWidget(new QLabel("Min X(T):"),  0,0);
    grid->addWidget(minEndValue,              0,1);
    grid->addWidget(new QLabel("Max X(T):"),  0,2);
    grid->addWidget(maxEndValue,              0,3);

    grid->addWidget(new QLabel("Min X(t):"),  1,0);
    grid->addWidget(minPathValue,             1,1);
    grid->addWidget(new QLabel("Max X(t):"),  1,2);
    grid->addWidget(maxPathValue,             1,3);

    grid->addWidget(new QLabel("E[X(T)]:"),   2,0);
    grid->addWidget(expectedValue,            2,1);
    grid->addWidget(new QLabel("σ(X(T)):"),   2,2);
    grid->addWidget(stdDevValue,              2,3);
}

StatusManager::StatusInfo::StatusInfo(QWidget* parent)
    : QGroupBox("Status", parent)
    , currentStatus(new QLabel(this))
    , progressBar(new QProgressBar(this))
{
    auto* grid = new QGridLayout(this);
    initGridLayout(grid);
    setLayout(grid);
    progressBar->setFormat("%v/%m");
    grid->addWidget(currentStatus, 0, 0);
    grid->addWidget(progressBar, 1, 0);
}

StatusManager::StatusManager(QWidget* parent)
    : QWidget(parent)
    , m_queryInfo(new QueryInfo(this))
    , m_resultInfo(new ResultInfo(this))
    , m_statusInfo(new StatusInfo(this))
{
    auto layout = new QHBoxLayout(this);
    auto queryInfoLayout = new QVBoxLayout(m_queryInfo);
    queryInfoLayout->addWidget(new QLabel("", this));
    layout->addWidget(m_queryInfo, 3);
    layout->addWidget(m_resultInfo, 2);
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
    m_statusInfo->currentStatus->repaint();
    emit signalReady();
}

void StatusManager::prepareStatusInfo(const size_t totalPaths){
    m_statusInfo->progressBar->setRange(0, static_cast<int>(totalPaths));
    m_statusInfo->progressBar->setValue(0);
    m_statusInfo->currentStatus->setText("Sampling...");
}

void StatusManager::setQueryInfo(const PathQuery& query){
    QString queryInfo;
    QTextStream streamParams(&queryInfo);
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
    m_queryInfo->infoLabel->setText(queryInfo);
    m_queryInfo->infoLabel->repaint();
}

void StatusManager::clear(){
    m_queryInfo->infoLabel->setText("");
    m_statusInfo->progressBar->setRange(0, 1);
    m_statusInfo->progressBar->setValue(0);
    setReady();
}

void StatusManager::cancel(){
    m_statusInfo->currentStatus->setText("Cancelling...");
    m_statusInfo->currentStatus->repaint();
}
