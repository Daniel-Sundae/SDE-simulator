#pragma once
#include "Types.hpp"
#include "PathQuery.hpp"
#include <QtWidgets/QWidget>
#include <QtWidgets/QGroupBox>

class QProgressBar;
class QLabel;

class StatusManager : public QWidget
{
    Q_OBJECT
private:
    struct QueryInfo : public QGroupBox {
        explicit QueryInfo(QWidget* parent);
        QLabel* infoLabel;
    };

    struct ResultInfo : public QGroupBox {
        explicit ResultInfo(QWidget* parent);
        QLabel* minEndValue;
        QLabel* maxEndValue;
        QLabel* minPathValue;
        QLabel* maxPathValue;
    };

    struct StatusInfo : public QGroupBox {
        explicit StatusInfo(QWidget* parent);
        QLabel* currentStatus;
        QLabel* errorStatus;
        QProgressBar* progressBar;
    };
    
public:
    explicit StatusManager(QWidget* parent);
    void setProgress(const size_t completed);
    void setReady();
    void prepareStatusInfo(const size_t totalPaths);
    void setQueryInfo(const PathQuery& query);
    void clear();
    void cancel();
signals:
    void signalReady();

private:
    QueryInfo* m_queryInfo;
    ResultInfo* m_resultInfo;
    StatusInfo* m_statusInfo;
};