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
    void setStatus(const StatusSignal s);
    void setQueryInfo(const PathQuery &pQuery);
    void clearStatus();

private:
    QGroupBox* m_queryDefinition;
    QGroupBox* m_queryParameters;
    QGroupBox* m_statusInfo;
    StatusSignal m_status;
};