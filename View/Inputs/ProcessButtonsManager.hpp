#pragma once
#include "ButtonWidgetManager.hpp"
#include <unordered_map>
#include <memory>

class ProcessButton;

class ProcessButtonsManager : public QWidget
{
	Q_OBJECT
public:
	explicit ProcessButtonsManager(InputManager* parent, std::shared_pointer<InputPresenter> listener));
	auto OnButtonPressed(ProcessType type) -> void;

private:
	auto InitializeProcessButtonsManager() -> void;

private:
	std::unordered_map<ProcessType, ProcessButton*> m_buttons;
	std::shared_pointer<InputPresenter> m_listener;
};