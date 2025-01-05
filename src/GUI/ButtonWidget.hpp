#pragma once
#include "SDEMetaData.hpp"
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qspinbox.h>


class ButtonWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ButtonWidget(QWidget* parent = nullptr);

signals:
    void ChartUpdate(
        std::vector<double> data,
        const QString& label);

private:
    auto InitializeLayout() -> void;
    auto CreateParameterLayout() -> QHBoxLayout *;
    auto CreateButtonLayout() -> QGridLayout *;
    auto ButtonLayout() const -> QGridLayout *;
    auto GetMueValue() const -> double;
    auto GetSigmaValue() const -> double;
    auto GetStartValue() const -> double;
    auto CreateSDEButton(
        const SDEMetaData &metaData, 
        std::function<std::vector<double>()> sample) -> QPushButton *;
};

