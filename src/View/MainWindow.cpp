#include "MainWindow.hpp"
#include "InputDispatcher.hpp"
#include "OutputDispatcher.hpp"

MainWindow::MainWindow()
    : QMainWindow()
    , m_centralWidget(new QWidget(this))
    , m_mainWindowLayout(new QHBoxLayout(m_centralWidget))
    , m_outputDispatcher(new OutputDispatcher(m_centralWidget))
    , m_inputDispatcher(new InputDispatcher(m_centralWidget))
{
    setCentralWidget(m_centralWidget);
    setWindowTitle("SDE Simulator");
    resize(1500, 700);
    addWidgets();
    show();
}

void MainWindow::addWidgets(){
    m_mainWindowLayout->addWidget(m_outputDispatcher, 5);
    m_mainWindowLayout->addWidget(m_inputDispatcher, 1);
}

InputDispatcher* MainWindow::getInputDispatcher() const{
    return m_inputDispatcher;
}

OutputDispatcher* MainWindow::getOutputDispatcher() const{
    return m_outputDispatcher;
}
