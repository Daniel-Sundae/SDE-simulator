#pragma once
#include "Types.hpp"
#include <unordered_map>
#include <memory>
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qgroupbox.h>


class ProcessButton;
class InputManager;

class ProcessButtonsManager : public QGroupBox
{
	Q_OBJECT
public:
	explicit ProcessButtonsManager(InputManager* parent);
	auto OnButtonPressed(ProcessType type) -> void;

private:
	auto InitializeProcessButtonsManager() -> void;
	auto InitializeDesign() -> void;

private:
	std::unordered_map<ProcessType, ProcessButton*> m_buttons;
};