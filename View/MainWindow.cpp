#include "Utils.hpp"
#include "MainWindow.hpp"


MainWindow::MainWindow() : QMainWindow(),
    m_mainWindowLayout(new QHBoxLayout),
    m_chartWidget(new ChartWidget(this)),
    m_buttonWidget(new InputManager(this))
{
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    centralWidget->setLayout(m_mainWindowLayout);
    InitializeMainWindow();
    SetupConnections();
}

auto MainWindow::InitializeMainWindow() -> void{
    m_mainWindowLayout->addWidget(m_chartWidget, 8);
    m_mainWindowLayout->addWidget(m_buttonWidget, 2);
    setWindowTitle("Qt6 Window with Button");
    resize(1300, 600);
    show();
}

auto MainWindow::SetupConnections() const -> void{
    connect(
        m_buttonWidget,
        &InputManager::ForwardRequestUpdatePathChart, 
        m_chartWidget,
        &ChartWidget::ProcessButtonClicked
    );
}

//connect(
//    sender,            // m_buttonWidget (the object that sends the signal)
//    signalFunc,            // &InputManager::ChartUpdate (the signal to listen for) (signal is sent when "emit ChartUpdate()" line is run)
//    receiver,          // m_chartWidget (the object that receives the signal)
//    slotFunc              // &ChartWidget::OnButtonClickSignal (the slot to be called)
//);

/*
To emit a function, the function must be under signal.
The receiver object must have the slotFunc under slots (perhaps private slots if only itra object transmission)
*/