#include "AppInitializer.hpp"
#include "MainWindow.hpp"
#include "InputManager.hpp"
#include "ChartManager.hpp"

AppInitializer::AppInitializer()
{
	InitializeComponents();
	ConnectComponents();
}

auto AppInitializer::InitializeComponents -> void
{
	auto* mainWindow = std::make_unique<MainWindow>();
	m_viewComponents.push_back();
	m_viewComponents.push_back(std::make_unique<InputManager>());
	m_viewComponents.push_back(std::make_unique<ChartManager>());
	mainWindow->SetInputManager(inputManager);
	mainWindow->SetChartManager(chartManager);


}

auto AppInitializer::AppHealthy() const -> void
