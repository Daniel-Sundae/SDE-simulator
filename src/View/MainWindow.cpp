#include "MainWindow.hpp"
#include "InputManager.hpp"
#include "OutputManager.hpp"

MainWindow::MainWindow()
    : QMainWindow()
    , m_centralWidget(new QWidget(this))
    , m_mainWindowLayout(new QHBoxLayout(m_centralWidget))
    , m_outputManager(new OutputManager(m_centralWidget))
    , m_inputManager(new InputManager(m_centralWidget))
{
    setCentralWidget(m_centralWidget);
    setWindowTitle("Qt6 Window with Button");
    resize(1300, 600);
    AddWidgets();
    show();
}

auto MainWindow::AddWidgets() -> void
{
    m_mainWindowLayout->addWidget(m_outputManager, 5);
    m_mainWindowLayout->addWidget(m_inputManager, 1);
}

auto MainWindow::GetInputManager() const -> InputManager*
{
    return m_inputManager;
}

auto MainWindow::GetOutputManager() const -> OutputManager*
{
    return m_outputManager;
}
