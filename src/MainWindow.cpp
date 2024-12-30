#include "Utils.hpp"
#include "MainWindow.hpp"


MainWindow::MainWindow() : QMainWindow(),
    m_mainWindowLayout(new QHBoxLayout),
    m_chartWidget(new ChartWidget(this)),
    m_buttonWidget(new ButtonWidget(this))
{
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    centralWidget->setLayout(m_mainWindowLayout);
    connect(
        m_buttonWidget,
        &ButtonWidget::SampleData, 
        m_chartWidget,
        &ChartWidget::PlotChart);
    InitializeMainWindow();
}

//---------------------------------------------------------------------------//
void MainWindow::InitializeMainWindow() {
    m_mainWindowLayout->addWidget(m_chartWidget, 8);
    m_mainWindowLayout->addWidget(m_buttonWidget, 2);
    setWindowTitle("Qt6 Window with Button");
    resize(1300, 600);
    show();
}
//---------------------------------------------------------------------------//
/* 
void MainWindow::CreateSDEChart(){
    m_SDEChart.setTitle("Testtitle");
    m_SDEChart.setTheme(QChart::ChartThemeBrownSand);
    m_mainChartView.resize(600,600);
    // m_axisX = new QtCharts::QValueAxis;
    // m_axisX->setTitleText("X Axis");
    // m_axisX->setRange(0, 1000);        // Adjust as needed
    // m_axisX->setLabelFormat("%.0f");   // e.g., integer labels

    // m_axisY = new QtCharts::QValueAxis;
    // m_axisY->setTitleText("Y Axis");
    // m_axisY->setRange(-1.0, 1.0);      // Adjust as needed
    // m_axisY->setLabelFormat("%.2f");   // e.g., floating labels

    // // Add axes to the chart
    // m_mainChart.addAxis(m_axisX, Qt::AlignBottom);
    // m_mainChart.addAxis(m_axisY, Qt::AlignLeft);

    m_mainChartView.show();

}
//---------------------------------------------------------------------------//
void MainWindow::CreateButtons() {
    CreateButton(
        "BM1",
        []{ return Utils::BrownianMotion().Sample(1000, 0.05, 1); }
        );
    
    CreateButton(
        "BM2",
        []{ return Utils::BrownianMotion().Sample(500, 0.05, 100); }
        );
}
//---------------------------------------------------------------------------//
void MainWindow::CreateButton(const QString& label, std::function<std::vector<double>()> sample){
    auto* button = new QPushButton(label, this);

    QObject::connect(
        button,
        &QPushButton::clicked,
        [this, sample]()
        {
            m_SDEChart.addSeries(Utils::LineSeries(sample()).release());
            //ResizeIfNeeded();
        }
    );
    // Add the button to the layout and set alignment
    centralWidget()->layout()->addWidget(button);
    centralWidget()->layout()->setAlignment(button, Qt::AlignRight);
}
//---------------------------------------------------------------------------//
 */