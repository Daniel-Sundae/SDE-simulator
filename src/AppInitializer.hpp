#pragma once
#include <memory>
#include <vector>

class MainWindow;
class MainPresenter;

class AppInitializer {
public:
	explicit AppInitializer();
	~AppInitializer();
private:
	auto InitializeComponents() -> void;
	
	std::unique_ptr<MainWindow> m_mainWindow;
	std::unique_ptr<MainPresenter> m_mainPresenter;
};