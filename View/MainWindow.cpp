#include "MainWindow.hpp"
#include "InputManager.hpp"
#include "OutputManager.hpp"

MainWindow::MainWindow()
    : QMainWindow()
    , m_centralWidget(new QWidget(this))
    , m_mainWindowLayout(new QHBoxLayout(m_centralWidget))
    , m_outputManager(new OutputManager(this))
    , m_inputManager(new InputManager(this))
{
    setCentralWidget(m_centralWidget);
    m_centralWidget->setLayout(m_mainWindowLayout);
    setWindowTitle("Qt6 Window with Button");
    resize(1300, 600);
    AddWidgets();
    show();
}

auto MainWindow::AddWidgets() -> void
{
    m_mainWindowLayout->addWidget(m_outputManager, 8);
    m_mainWindowLayout->addWidget(m_inputManager, 2);
}

auto MainWindow::GetInputManager() const -> InputManager*
{
    return m_inputManager;
}

auto MainWindow::GetOutputManager() const -> OutputManager*
{
    return m_outputManager;
}
