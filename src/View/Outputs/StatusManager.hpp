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
        QLabel* minXT;
        QLabel* maxXT;
        QLabel* minXt;
        QLabel* maxXt;
        QLabel* expectedValue;
        QLabel* stdDevValue;
    };

    struct StatusInfo : public QGroupBox {
        explicit StatusInfo(QWidget* parent);
        QLabel* currentStatus;
        QProgressBar* progressBar;
    };

public:
    explicit StatusManager(QWidget* parent);
    void setProgress(const size_t completed);
    void setResults(State minXT, State maxXT, State minXt, State maxXt);
    void setReady();
    void setEVSTDInfo(State mu, State sigma);
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