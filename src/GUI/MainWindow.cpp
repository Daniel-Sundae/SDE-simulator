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
    InitializeMainWindow();
    ConnectButtons();
}
//---------------------------------------------------------------------------//
auto MainWindow::InitializeMainWindow() -> void{
    m_mainWindowLayout->addWidget(m_chartWidget, 8);
    m_mainWindowLayout->addWidget(m_buttonWidget, 2);
    setWindowTitle("Qt6 Window with Button");
    resize(1300, 600);
    show();
}
//---------------------------------------------------------------------------//
auto MainWindow::ConnectButtons() const -> void{
    connect(
        m_buttonWidget,
        &ButtonWidget::ChartUpdate, 
        m_chartWidget,
        &ChartWidget::OnButtonClickSignal
        );
    
}
