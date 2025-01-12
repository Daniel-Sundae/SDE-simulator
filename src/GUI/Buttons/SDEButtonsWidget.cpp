#include "SDEButtonsWidget.hpp"
#include "CreateSDE.hpp"
#include "Types.hpp"

SDEButtonsWidget::SDEButtonsWidget(ButtonWidgetManager* parent):
    QWidget(parent)
{
    InitializeSDEButtonsWidget();
}

auto SDEButtonsWidget::InitializeSDEButtonsWidget() -> void
{
    auto* buttonLayout = new QGridLayout(this);

    buttonLayout->addWidget(CreateSDEButton(
        SDEMetaData::Create(SDEType::BM),
        [this] {
            return CreateSDE::BrownianMotion(GetStartValue()).Sample(1000, 0.1);
        }
    ));

    buttonLayout->addWidget(CreateSDEButton(
        SDEMetaData::Create(SDEType::GBM),
        [this] {
            return CreateSDE::GeometricBrownianMotion(GetMueValue(), GetSigmaValue(), GetStartValue()).Sample(1000, 0.01);
        }
    ));
    setLayout(buttonLayout);
}
//---------------------------------------------------------------------------//
auto SDEButtonsWidget::CreateSDEButton(
    const SDEMetaData& metaData,
    std::function<std::vector<double>()> sample) -> QPushButton*
{
    auto* button = new QPushButton(this);
    button->setText(metaData.acronym);
    button->setToolTip(metaData.description);
    QString chartTitle = metaData.name + ": " + metaData.definition;
    // Button sends clicked signal to buttonwidget which emits sample data to ChartWidget::OnButtonClickSignal
    connect(
        button,
        &QPushButton::clicked,
        this,
        [this, sample, chartTitle]() {
            emit ChartUpdate(sample(), chartTitle);
        });
    return button;
}
//---------------------------------------------------------------------------//

