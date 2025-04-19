#pragma once
#include "Types.hpp"
#include "PathQuery.hpp"
#include <QtWidgets/QGroupBox>

class QLabel;

class StatusManager : public QGroupBox
{
    Q_OBJECT
public:
    explicit StatusManager(QWidget* parent);
    auto SetStatus(const StatusSignal s) -> void;
    auto SetQueryInfo(const PathQuery &pQuery) -> void;

private:
    QLabel* m_queryInfo;
    QLabel* m_statusInfo;
    StatusSignal m_status;
};