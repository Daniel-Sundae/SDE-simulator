#pragma once
#include "Types.hpp"
#include "PathQuery.hpp"
#include <QtWidgets/QWidget>

class QGroupBox;
class StatusManager : public QWidget
{
    Q_OBJECT
public:
    explicit StatusManager(QWidget* parent);
    auto SetStatus(const StatusSignal s) -> void;
    auto SetQueryInfo(const PathQuery &pQuery) -> void;
    auto ClearStatus() -> void;

private:
    QGroupBox* m_queryDefinition;
    QGroupBox* m_queryParameters;
    QGroupBox* m_statusInfo;
    StatusSignal m_status;
};