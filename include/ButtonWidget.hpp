#pragma once
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qpushbutton.h>


class ButtonWidget : public QWidget
{
public:
    explicit ButtonWidget(QWidget* parent = nullptr);

private:
    auto CreateButtons() -> void;
    auto CreateButton() -> void;

    QVBoxLayout* m_layout;
    QPushButton* m_plotButton;
};