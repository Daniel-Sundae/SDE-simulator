#pragma once

#include "Types.hpp"
#include "InputHandler.hpp"
#include <unordered_map>
#include <QtWidgets/qgroupbox.h>

class InputManager;
class QPushButton;
class QDoubleSpinBox;



class ActionManager : public QGroupBox
{
	Q_OBJECT
public:
	explicit ActionManager(InputManager* parent);
private:
	auto AddActionWidgets() -> void;
	auto Parent() const -> InputManager*;
private:
	std::unordered_map<ActionWidget, QWidget*> m_widgets;
};

