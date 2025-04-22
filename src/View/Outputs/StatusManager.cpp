#include "StatusManager.hpp"
#include "ViewUtils.hpp"
#include <unordered_map>
#include <QtCore/QString>
#include <QtWidgets/QLabel>
#include <QtWidgets/QHBoxLayout>

static const std::unordered_map<StatusSignal, QString> statusToString = {
    {StatusSignal::READY, QString("Ready")},
    {StatusSignal::SAMPLING, QString("Sampling...")},
    {StatusSignal::RENDERING, QString("Rendering...")},
};

StatusManager::StatusManager(QWidget* parent)
: QGroupBox(parent)
, m_queryInfo(new QLabel("", this))
, m_statusInfo(new QLabel("Ready", this))
, m_status(StatusSignal::READY)
{
    auto layout = new QHBoxLayout(this);
    layout->addWidget(m_queryInfo, 5);
    layout->addWidget(m_statusInfo, 1);
    setStyleSheet(GUI::GroupBoxDescription());
}

auto StatusManager::SetStatus(const StatusSignal s) -> void
{
    m_status = s;
    m_statusInfo->setText(statusToString.at(s));
}

auto StatusManager::SetQueryInfo(const PathQuery& pQuery) -> void
{
    QString info;
    QTextStream stream(&info);
    stream << "Definition: "
           << QString::fromUtf8(ProcessData::GetName(pQuery.processDefinition.type))
           << ": "
           << QString::fromUtf8(ProcessData::GetDefinition(pQuery.processDefinition.type))
           << " with μ = "
           << QString::number(pQuery.processDefinition.drift.Mu())
           << ", σ = "
           << QString::number(pQuery.processDefinition.diffusion.Sigma())
           << ", X<sub>0</sub> = "
           << QString::number(pQuery.processDefinition.startValueData)
           << '\n'
           << "Simulation: Time = "
           << QString::number(pQuery.simulationParameters.time)
           << ", dt = "
           << QString::number(pQuery.simulationParameters.dt);
    stream.flush();
    m_queryInfo->setText(info);
}
