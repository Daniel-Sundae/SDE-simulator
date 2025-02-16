#pragma once
#include "Types.hpp"
#include <unordered_map>
#include <memory>
#include <QtWidgets/qwidget.h>


class ProcessButton;
class InputManager;

class ProcessButtonsManager : public QWidget
{
	Q_OBJECT
public:
	explicit ProcessButtonsManager(InputManager* parent);
	auto OnButtonPressed(ProcessType type) -> void;

private:
	auto InitializeProcessButtonsManager() -> void;

private:
	std::unordered_map<ProcessType, ProcessButton*> m_buttons;
};