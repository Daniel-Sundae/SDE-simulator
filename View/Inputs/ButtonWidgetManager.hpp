#pragma once
#include "ProcessMetaData.hpp"
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qspinbox.h>

class ParametersWidget;
class ProcessButtonsManager;

class ButtonWidgetManager : public QWidget
{
    Q_OBJECT

public:
    explicit ButtonWidgetManager(QWidget* parent = nullptr);

signals:
    void ForwardRequestUpdatePathChart(
        const Path& data,
        const ProcessType type);

private:
    auto InitializeLayout() -> void;
    auto SetupConnections() -> void;

private:
    ParametersWidget* m_parametersWidget;
    ProcessButtonsManager* m_ProcessButtonsManager;
};

