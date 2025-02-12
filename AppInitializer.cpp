#include "AppInitializer.hpp"
#include "MainWindow.hpp"
#include "InputManager.hpp"
#include "ChartManager.hpp"
#include "InputHandler.hpp"
#include "OutputHandler.hpp"
#include "MainPresenter.hpp"
#include "PathEngine.hpp"

AppInitializer::AppInitializer()
{
	InitializeComponents();
}

auto AppInitializer::InitializeComponents() -> void
{
	// View components
	auto m_view = std::make_unique<MainWindow>();

	// Presenter components
	auto inputHandler = std::make_unique<InputHandler>();
	auto inputHandlerPtr = inputHandler.get();
	m_presenterComponents.push_back(std::move(inputHandler));
	auto outputHandler = std::make_unique<OutputHandler>();
	auto outputHandlerPtr = outputHandler.get();
	m_presenterComponents.push_back(std::move(outputHandler));
	auto mainPresenter = std::make_unique<MainPresenter>();
	auto mainPresenterPtr = mainPresenter.get();
	m_presenterComponents.push_back(std::move(mainPresenter));
	inputHandlerPtr->SetListener(mainPresenterPtr);
	mainPresenterPtr->SetListener(outputHandlerPtr);

	// Model component
	auto engine = std::make_unique<PathEngine>();
	auto enginePtr = engine.get();
	m_modelComponents.push_back(std::move(engine));

	// Connect view and presenter
	mainWindow->InputManager()->SetListener(inputHandler.get());
	outputHandler->SetListener(mainWindow->ChartManager());
}