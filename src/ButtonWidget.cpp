#include "ButtonWidget.hpp"
#include "Utils.hpp"

ButtonWidget::ButtonWidget(QWidget* parent)
    : QWidget(parent)
    , m_layout(new QVBoxLayout(this))
    , m_plotButton(nullptr)
{
    setLayout(m_layout);
    CreateButtons();
}

auto ButtonWidget::CreateButtons() -> void
{
    CreateButton(
        "BM1",
        []{ return Utils::BrownianMotion().Sample(1000, 0.05, 1); }
        );
    
    CreateButton(
        "BM2",
        []{ return Utils::BrownianMotion().Sample(500, 0.05, 100); }
        );
}

auto ButtonWidget::CreateButton(const QString& label, std::function<std::vector<double>()> sample) -> void
{
    auto* button = new QPushButton(label, this);
    layout()->addWidget(button);

    // Button sends clicked signal to buttonwidget which emits sample data
    connect(button, &QPushButton::clicked, this, [this, sample](){
        emit SampleData(sample());
    });
}