#include "AppInitializer.hpp"
#include "InputHandler.hpp"
#include "OutputHandler.hpp"
#include "InputDispatcher.hpp"
#include "MainPresenter.hpp"
#include "MainWindow.hpp"
#include "PathQuery.hpp"
#include "Types.hpp"
#include <QMetaType>
#include <QtGui/QPalette>
#include <QtGui/QColor>

AppInitializer::AppInitializer(QApplication& app)
    : m_app(app)
	, m_mainWindow(std::make_unique<MainWindow>())
	, m_mainPresenter(std::make_unique<MainPresenter>())
{
    // Register custom type to move data from enginethread to GUI thread
    qRegisterMetaType<Paths>("Paths");

	InitializeComponents();
    SetStyle();
}

AppInitializer::~AppInitializer() = default;

auto AppInitializer::InitializeComponents() -> void
{
	m_mainPresenter->GetOutputHandler()->SetListener(m_mainWindow->GetOutputDispatcher());
	m_mainPresenter->SetListener(m_mainPresenter->GetOutputHandler());
	m_mainPresenter->GetInputHandler()->SetListener(m_mainPresenter.get());
	m_mainWindow->GetInputDispatcher()->SetInputHandler(m_mainPresenter->GetInputHandler());
}

auto AppInitializer::SetStyle() -> void
{
    m_app.setStyle("Fusion");
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::black);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);
    m_app.setPalette(darkPalette);
}
