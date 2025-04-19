#pragma once
#include <memory>
#include <vector>
#include <QtWidgets/QApplication>

class MainWindow;
class MainPresenter;

class AppInitializer {
public:
	explicit AppInitializer(QApplication& app);
	~AppInitializer();
private:
	auto InitializeComponents() -> void;

	auto SetStyle() -> void;

	QApplication& m_app;
	std::unique_ptr<MainWindow> m_mainWindow;
	std::unique_ptr<MainPresenter> m_mainPresenter;
};
