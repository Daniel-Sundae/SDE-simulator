#include "MainWindow.hpp"
#include <stdexcept>


MainWindow::MainWindow() : QMainWindow(),
    m_mainWindowLayout(new QHBoxLayout),
    m_chartManager(new ChartManager(this)),
    m_inputManager(new InputManager(this)),
    m_widgetsAdded(0)
{
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    centralWidget->setLayout(m_mainWindowLayout);
    setWindowTitle("Qt6 Window with Button");
    resize(1300, 600);
    show();
}

auto MainWindow::SetChartManager(ChartManager* chartManager) -> void
{
    m_chartManager = chartManager;
}

auto MainWindow::SetInputManager(InputManager* inputManager) -> void
{
    m_inputManager = inputManager;
}

auto MainWindow::AddWidgets() -> void
{
    // DSTODO: what are these numbers   
    if (m_widgetsAdded){
        throw std::runtime_error("Error: Already added widgets")
    }
    if (!m_chartManager || !m_inputManager) {
        throw std::logic_error("Error: Managers not set")
    }
    m_mainWindowLayout->addWidget(m_chartManager, 8);
    m_mainWindowLayout->addWidget(m_inputManager, 2);
}

auto MainWindow::Healthy()
{
    if (!m_widgetsAdded) return 0;
    if (!m_chartManager) return 0;
    if (!m_inputManager) return 0;
    return 1;
}