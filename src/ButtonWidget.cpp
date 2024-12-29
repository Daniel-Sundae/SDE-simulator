#include "ButtonWidget.hpp"
#include "Utils.hpp"

ButtonWidget::ButtonWidget(QWidget* parent)
    : QWidget(parent)
    , m_layout(new QVBoxLayout(this))
    , m_plotButton(nullptr)
{
    CreateButtons();
}

auto ButtonWidget::CreateButtons() -> void
{
    CreateButton();
    CreateButton(
        "BM1",
        []{ return Utils::BrownianMotion().Sample(1000, 0.05, 1); }
        );
    
    CreateButton(
        "BM2",
        []{ return Utils::BrownianMotion().Sample(500, 0.05, 100); }
        );
    setLayout(m_layout);
    
}

auto ButtonWidget::CreateButton(const QString& label, std::function<std::vector<double>()> sample) -> void
{
    auto* button = new QPushButton(label, this);

    QObject::connect(
        button,
        &QPushButton::clicked,
        [sample]()
        {
            m_SDEChart.addSeries(Utils::LineSeries(sample()).release());
            //ResizeIfNeeded();
        }
    );
    // Add the button to the layout and set alignment
    layout()->addWidget(button);
    //layout()->setAlignment(button, Qt::AlignRight);
}