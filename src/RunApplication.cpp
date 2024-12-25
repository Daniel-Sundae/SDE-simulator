#include "Utils.hpp"
#include "RunApplication.hpp"
#include <QLineSeries>
#include <QPushButton>
#include <QMessageBox>


RunApplication::RunApplication():
    m_mainWindow(),
    m_mainChart(),
    m_mainChartView(&m_mainChart, &m_mainWindow),
    m_layout()
{
    InitializeMainWindow();
}
//---------------------------------------------------------------------------//
void RunApplication::InitializeMainWindow() {
    // Create the main window widget with layout
    
    m_mainWindow.setWindowTitle("Qt6 Window with Button");
    m_mainWindow.resize(700, 600);
    m_mainChart.setTitle("Testtitle")
    m_mainChartView.resize(600,600);
    m_mainChartView.show();
    CreateDefaultButtons();

    m_mainWindow.setLayout(&m_layout);

    m_mainWindow.show();

}
//---------------------------------------------------------------------------//
void RunApplication::CreateDefaultButtons() {
    CreateButton(
        "BM",
        []{ return Utils::BrownianMotion().Sample(1000, 0.05, 1); }
        );
    
    CreateButton(
        "BM2",
        []{ return Utils::BrownianMotion().Sample(500, 0.05, 100); }
        );
}
//---------------------------------------------------------------------------//
void RunApplication::CreateButton(const QString& label, std::function<std::vector<double>()> sample){
    auto* button = new QPushButton(label, &m_mainWindow);

    QObject::connect(
        button,
        &QPushButton::clicked,
        [this, sample]()
        {
            m_mainChart.addSeries(Utils::LineSeries(sample()).release());
            ResizeIfNeeded();
        }
    );
    // Add the button to the layout and set alignment
    m_layout.addWidget(button);
    m_layout.setAlignment(button, Qt::AlignRight);
}
//---------------------------------------------------------------------------//
