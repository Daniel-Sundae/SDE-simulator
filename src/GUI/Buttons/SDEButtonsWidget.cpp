#include "SDEButtonsWidget.hpp"
#include "CreateSDE.hpp"
#include "Types.hpp"

SDEButtonsWidget::SDEButtonsWidget(ButtonWidgetManager* parent):
    QWidget(parent)
{
    CreateSDEButtons();
    InitializeSDEButtonsWidgetLayout();
}



auto SDEButtonsWidget::InitializeSDEButtonsWidget() -> void
{
    auto* buttonLayout = new QGridLayout(this);

    buttonLayout->addWidget();

    buttonLayout->addWidget(CreateSDEButton(
        SDEMetaData::Create(SDEType::GBM),
        [this] {
            return CreateSDE::GeometricBrownianMotion(GetMuValue(), GetSigmaValue(), GetStartValue()).Sample(1000, 0.01);
        }
    ));
    setLayout(buttonLayout);
}
auto SDEButtonsWidget::CreateSDEButtons() -> void
{
    CreateSDEButton(
        SDEMetaData::Create(SDEType::BM),
        [this] {
            return SDE<SDEType::BM>::Create(GetStartValue()).Sample(1000, 0.1);
        }
    )
}

auto SDEButtonsWidget::CreateSDEButton(
    const SDEMetaData& metaData,
    std::function<Path()> sample) -> QPushButton*
{
    auto* button = new QPushButton(this);
    button->setText(metaData.acronym);
    button->setToolTip(metaData.description);
    QString chartTitle = metaData.name + ": " + metaData.definition;
    // Button sends clicked signal to SDEButtonsWidget (this) which emits sample data to ChartWidget::OnButtonClickSignal
    connect(
        button,
        &QPushButton::clicked,
        this,
        [this, sample, chartTitle]() {
            emit RequestUpdatePathChart(sample(), chartTitle);
        });
    return button;
}


