#include "AppInitializer.hpp"
#include "MainWindow.hpp"
#include "InputHandler.hpp"
#include "OutputHandler.hpp"
#include "MainPresenter.hpp"
#include "InputManager.hpp"

AppInitializer::AppInitializer()
	: m_mainWindow(std::make_unique<MainWindow>())
	, m_mainPresenter(std::make_unique<MainPresenter>())
{
	InitializeComponents();
}

auto AppInitializer::InitializeComponents() -> void
{

	m_mainPresenter->GetOutputHandler()->SetListener(m_mainWindow->GetOutputManager());
	m_mainPresenter->SetListener(m_mainPresenter->GetOutputHandler());
	m_mainPresenter->GetInputHandler()->SetListener(m_mainPresenter.get());
	m_mainWindow->GetInputManager()->SetInputHandler(m_mainPresenter->GetInputHandler());
}