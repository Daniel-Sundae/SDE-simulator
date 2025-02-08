#include "MainWindow.hpp"

MainWindow::MainWindow() : QMainWindow(),
    m_mainWindowLayout(new QHBoxLayout),
    m_chartManager(new ChartManager(this)),
    m_inputManager(new InputManager(this))
{
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    centralWidget->setLayout(m_mainWindowLayout);
    setWindowTitle("Qt6 Window with Button");
    resize(1300, 600);
    show();
}

auto MainWindow::AddWidgets() -> void
{
    m_mainWindowLayout->addWidget(m_chartManager, 8);
    m_mainWindowLayout->addWidget(m_inputManager, 2);
}