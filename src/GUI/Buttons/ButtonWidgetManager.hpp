#pragma once
#include "SDEMetaData.hpp"
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qspinbox.h>

class ParametersWidget;
class SDEButtonsWidget;

class ButtonWidgetManager : public QWidget
{
    Q_OBJECT

public:
    explicit ButtonWidgetManager(QWidget* parent = nullptr);

signals:
    void ButtonWidgetPathSignal(
        const Path& data,
        const QString& label);

private:
    auto InitializeLayout() -> void;
    auto ConnectSignals() -> void;
private:
    ParametersWidget* m_parametersWidget;
    SDEButtonsWidget* m_SDEButtonsWidget;
};

