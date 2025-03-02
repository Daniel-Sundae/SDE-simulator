#pragma once

#include "Types.hpp"
#include "InputHandler.hpp"
#include "InputManager.hpp"
#include <unordered_map>

class QPushButton;
class QDoubleSpinBox;

class ActionManager : public InputManagerGroupBox
{
	Q_OBJECT
public:
	explicit ActionManager(InputManager* parent);
private:
	auto AddActionWidgets() -> void;
private:
	std::unordered_map<ActionWidget, QWidget*> m_widgets;
};

