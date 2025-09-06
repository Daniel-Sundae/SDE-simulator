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
    grid->addWidget(infoLabel);
}

StatusManager::ResultInfo::ResultInfo(QWidget* parent = nullptr)
    : QGroupBox("Results", parent)
    , minXT(new QLabel(this))
    , maxXT(new QLabel(this))
    , minXt(new QLabel(this))
    , maxXt(new QLabel(this))
    , expectedValue(new QLabel(this))
    , stdDevValue(new QLabel(this))
{
    auto* grid = new QGridLayout(this);
    initGridLayout(grid);

    grid->addWidget(new QLabel("Min X(T):"),  0,0);
    grid->addWidget(minXT,                    0,1);
    grid->addWidget(new QLabel("Max X(T):"),  0,2);
    grid->addWidget(maxXT,                    0,3);

    grid->addWidget(new QLabel("Min X(t):"),  1,0);
    grid->addWidget(minXt,                    1,1);
    grid->addWidget(new QLabel("Max X(t):"),  1,2);
    grid->addWidget(maxXt,                    1,3);

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
    layout->addWidget(m_queryInfo, 3);
    layout->addWidget(m_resultInfo, 2);
    layout->addWidget(m_statusInfo, 1);
    setStyleSheet(GUI::groupBoxDescription());
    setReady();
}

void StatusManager::setProgress(const size_t pathsCompleted){
    m_statusInfo->progressBar->setValue(static_cast<int>(pathsCompleted));
}

void StatusManager::setResults(State minXT, State maxXT, State minXt, State maxXt) {
    m_resultInfo->minXT->setText(QString::number(minXT, 'f', 4));
    m_resultInfo->maxXT->setText(QString::number(maxXT, 'f', 4));
    m_resultInfo->minXt->setText(QString::number(minXt, 'f', 4));
    m_resultInfo->maxXt->setText(QString::number(maxXt, 'f', 4));
    m_resultInfo->minXT->repaint();
    m_resultInfo->maxXT->repaint();
    m_resultInfo->minXt->repaint();
    m_resultInfo->maxXt->repaint();
}

void StatusManager::setReady(){
    m_statusInfo->currentStatus->setText("Ready");
    m_statusInfo->currentStatus->repaint();
    emit signalReady();
}

void StatusManager::setEVSTDInfo(State mu, State sigma){
    m_resultInfo->expectedValue->setText(QString::number(mu, 'f', 4));
    m_resultInfo->stdDevValue->setText(QString::number(sigma, 'f', 4));
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
                << QString::fromUtf8( getField(FieldTags::name{}, query.definitionParameters.type) ) << ": "
                << QString::fromUtf8( getField(FieldTags::definition{}, query.definitionParameters.type) ) << "\n"
                << "μ = " << QString::number(query.definitionParameters.drift.mu()) << ", "
                << "σ = " << QString::number(query.definitionParameters.diffusion.sigma()) << ", "
                << "X" << QString::fromUtf8("\u2080") << " = " << QString::number(query.definitionParameters.X0) << "\n"
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
    m_resultInfo->minXT->setText("");
    m_resultInfo->maxXT->setText("");
    m_resultInfo->minXt->setText("");
    m_resultInfo->maxXt->setText("");
    m_resultInfo->expectedValue->setText("");
    m_resultInfo->stdDevValue->setText("");
    m_statusInfo->progressBar->setRange(0, 1);
    m_statusInfo->progressBar->setValue(0);
    setReady();
}

void StatusManager::cancel(){
    m_statusInfo->currentStatus->setText("Cancelling...");
    m_statusInfo->currentStatus->repaint();
}
