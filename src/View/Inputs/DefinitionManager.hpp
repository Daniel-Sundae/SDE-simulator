#pragma once

#include "Types.hpp"
#include "InputManager.hpp"
#include <unordered_map>
#include <QtWidgets/qgroupbox.h>

class DefinitionManager : public InputManagerGroupBox
{
	Q_OBJECT
public:
	explicit DefinitionManager(InputManager* parent);
private:
	auto UpdateWidgetProperties(ProcessType process, bool initialize = false) -> void;
	auto AddDefinitionWidgets() -> void;
	auto InitializeDesign() -> void;
private:
	std::unordered_map<DefinitionWidget, QWidget*> m_widgets;
};

