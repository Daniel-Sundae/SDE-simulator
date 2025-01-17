#pragma once
#include "SDEMetaData.hpp"
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qspinbox.h>

class ParametersWidget;
class SDEButtonsManager;

class ButtonWidgetManager : public QWidget
{
    Q_OBJECT

public:
    explicit ButtonWidgetManager(QWidget* parent = nullptr);

signals:
    void ForwardRequestUpdatePathChart(
        const Path& data,
        const SDEType type);

private:
    auto InitializeLayout() -> void;
    auto SetupConnections() -> void;

private:
    ParametersWidget* m_parametersWidget;
    SDEButtonsManager* m_SDEButtonsManager;
};

