#pragma once
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qpushbutton.h>


class ButtonWidget : public QWidget
{
public:
    explicit ButtonWidget(QWidget* parent = nullptr);

signals:
    void newDataGenerated(std::vector<double> data);

private:
    auto CreateButtons() -> void;

    auto CreateButton(const QString &label, std::function<std::vector<double>()> sample) -> void;

    QVBoxLayout* m_layout;
    QPushButton* m_plotButton;
};