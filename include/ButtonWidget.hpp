#pragma once
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qpushbutton.h>


class ButtonWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ButtonWidget(QWidget* parent = nullptr);

signals:
    void SampleData(std::vector<double> data);

private:
    auto CreateButtons() -> void;

    auto CreateButton(const QString &label, std::function<std::vector<double>()> sample) -> void;

    QVBoxLayout* m_layout;
    QPushButton* m_plotButton;
};