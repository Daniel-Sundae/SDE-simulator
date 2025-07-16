#pragma once

#include "Types.hpp"
#include "InputHandler.hpp"
#include "InputDispatcher.hpp"
#include <unordered_map>

class QPushButton; // TODO: Remove?
class QDoubleSpinBox; // TODO: Remove?

class ActionManager : public InputDispatcherGroupBox
{
	Q_OBJECT
public:
	explicit ActionManager(InputDispatcher* parent);
private:
	auto AddActionWidgets() -> void;
private:
	std::unordered_map<ActionWidget, QWidget*> m_widgets;
};

