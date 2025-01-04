#pragma once
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
    auto InitializeParameterLayout() -> void;
    auto ButtonLayout() const -> QGridLayout*;
    auto GetMueValue() const -> double;
    auto GetSigmaValue() const -> double;
    auto CreateButtons() -> void;
    auto CreateButton(
        const QString &label,
        std::function<std::vector<double>()> sample,
        const QString &toolTip = "") -> void;

    QGridLayout* const m_buttonLayout;

};

