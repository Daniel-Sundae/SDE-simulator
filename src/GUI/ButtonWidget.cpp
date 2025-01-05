#include "ButtonWidget.hpp"
#include "CreateSDE.hpp"
#include "Types.hpp"
#include <QtWidgets/qlabel.h>

ButtonWidget::ButtonWidget(QWidget* parent):
    QWidget(parent)
{
    InitializeLayout();
}

auto ButtonWidget::InitializeLayout() -> void
{
    // Main layout
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(new QPushButton("Clear", this));
    mainLayout->addLayout(CreateParameterLayout());
    mainLayout->addLayout(CreateButtonLayout());
    mainLayout->setSpacing(0);
    setLayout(mainLayout);
}
auto ButtonWidget::CreateParameterLayout() -> QHBoxLayout*
{
    auto* paramLayout = new QHBoxLayout(this);
    
    auto* mueInput = new QDoubleSpinBox(this);
    mueInput->setObjectName("mueValue");
    mueInput->setRange(-10.0, 10.0);
    mueInput->setValue(0.2);
    mueInput->setSingleStep(0.1);
    paramLayout->addWidget(new QLabel("Drift (μ):"));
    paramLayout->addWidget(mueInput);

    auto* sigmaInput = new QDoubleSpinBox(this);
    sigmaInput->setObjectName("sigmaInput");
    sigmaInput->setRange(0.0, 10.0);
    sigmaInput->setValue(0.2);
    sigmaInput->setSingleStep(0.1);
    paramLayout->addWidget(new QLabel("Volatility (σ):"));
    paramLayout->addWidget(sigmaInput);

    auto* startValueInput = new QDoubleSpinBox(this);
    startValueInput->setObjectName("startValue");
    startValueInput->setRange(-20, 20.0);
    startValueInput->setValue(0);
    startValueInput->setSingleStep(1);
    paramLayout->addWidget(new QLabel("Start Value:"));
    paramLayout->addWidget(startValueInput);

    return paramLayout;
}

auto ButtonWidget::CreateButtonLayout() -> QGridLayout*
{
    auto* buttonLayout = new QGridLayout(this);

    buttonLayout->addWidget(CreateSDEButton(
        SDEMetaData::Create(SDEType::BM),
        [this]{
            return CreateSDE::BrownianMotion(GetStartValue()).Sample(1000, 0.1);
        }
        )
    );

    buttonLayout->addWidget(CreateSDEButton(
        SDEMetaData::Create(SDEType::GBM),
        [this]{ 
            return CreateSDE::GeometricBrownianMotion(GetMueValue(), GetSigmaValue(), GetStartValue()).Sample(1000, 0.01);
        }
        )
    );
    return buttonLayout;
}

auto ButtonWidget::GetMueValue() const -> double 
{
    auto* mueInput = findChild<QDoubleSpinBox*>("mueValue");
    return mueInput->value();
}

auto ButtonWidget::GetSigmaValue() const -> double 
{
    auto* sigmaInput = findChild<QDoubleSpinBox*>("sigmaValue");
    return sigmaInput->value();
}

auto ButtonWidget::GetStartValue() const -> double 
{
    auto* startValueInput = findChild<QDoubleSpinBox*>("startValue");
    return startValueInput->value();
}

auto ButtonWidget::CreateSDEButton(
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
        [this, sample, chartTitle](){
            emit ChartUpdate(sample(), chartTitle);
        });
    return button;
}