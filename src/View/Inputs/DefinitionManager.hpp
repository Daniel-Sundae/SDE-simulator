#pragma once

#include "Types.hpp"
#include "InputDispatcher.hpp"
#include <unordered_map>
#include <QtWidgets/qgroupbox.h>

class DefinitionManager : public InputDispatcherGroupBox
{
	Q_OBJECT
public:
	explicit DefinitionManager(InputDispatcher* parent);
private:
	auto UpdateWidgetProperties(ProcessType process, bool initialize = false) -> void;
	auto AddDefinitionWidgets() -> void;
	auto InitializeDesign() -> void;
private:
	std::unordered_map<DefinitionWidget, QWidget*> m_widgets;
};

