#include "ButtonWidget.hpp"
#include "StandardSDEs.hpp"
#include <QtWidgets/qlabel.h>

ButtonWidget::ButtonWidget(QWidget* parent):
    QWidget(parent),
    m_buttonLayout(new QGridLayout(this))
{
    InitializeLayout();
}

auto ButtonWidget::InitializeLayout() -> void
{
    // Main layout
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);

    InitializeParameterLayout();

    layout()->addWidget(new QPushButton("Clear", this));
    
    qobject_cast<QVBoxLayout*>(layout())->addLayout(m_buttonLayout);
    CreateButtons();
}
auto ButtonWidget::InitializeParameterLayout() -> void
{
    auto* paramLayout = new QHBoxLayout(this);
    
    auto* mueInput = new QDoubleSpinBox(this);  // Parent is ButtonWidget
    mueInput->setObjectName("mueInput");
    mueInput->setRange(-10.0, 10.0);
    mueInput->setValue(0.2);
    mueInput->setSingleStep(0.1);
    paramLayout->addWidget(new QLabel("Drift (μ):"));
    paramLayout->addWidget(mueInput);

    auto* sigmaInput = new QDoubleSpinBox(this);  // Parent is ButtonWidget
    sigmaInput->setObjectName("sigmaInput");
    sigmaInput->setRange(0.0, 10.0);
    sigmaInput->setValue(0.2);
    sigmaInput->setSingleStep(0.1);
    paramLayout->addWidget(new QLabel("Volatility (σ):"));
    paramLayout->addWidget(sigmaInput);

    qobject_cast<QVBoxLayout*>(layout())->addLayout(paramLayout);
}

auto ButtonWidget::ButtonLayout() const -> QGridLayout*
{
    return m_buttonLayout;
}

auto ButtonWidget::GetMueValue() const -> double 
{
    auto* mueInput = findChild<QDoubleSpinBox*>("mueInput");
    return mueInput->value();
}

auto ButtonWidget::GetSigmaValue() const -> double 
{
    auto* sigmaInput = findChild<QDoubleSpinBox*>("sigmaInput");
    return sigmaInput->value();
}

auto ButtonWidget::CreateButtons() -> void
{
    double mue = 0.2;
    double sigma = 0.2;

    CreateButton(
        "BM",
        []{ return SDEs::BrownianMotion().Sample(1000, 0.1, 0); },
        "Standard brownian motion."
        );
    
    CreateButton(
        "GBM",
        [&]{ 
            mue = 0.2;
            sigma = 0.2;
            return SDEs::GeometricBrownianMotion(mue, sigma).Sample(1000, 0.01, 1); },
            QString("Geometric brownian motion. Drift is %1, volatility is %2.").arg(mue).arg(sigma)
        );
}

auto ButtonWidget::CreateButton(
    const QString& label,
    std::function<std::vector<double>()> sample,
    const QString& toolTip) -> void
{
    auto* button = new QPushButton(label, this);
    button->setToolTip(toolTip);
    ButtonLayout()->addWidget(button);

    // Button sends clicked signal to buttonwidget which emits sample data to ChartWidget::OnButtonClickSignal
    connect(
        button,
        &QPushButton::clicked,
        this,
        [this, sample, label](){
            emit ChartUpdate(sample(), label);
        });
}