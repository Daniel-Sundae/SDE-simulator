#pragma once
#include "Types.hpp"
#include "InputDispatcher.hpp"
#include <QtWidgets/qgroupbox.h>

class SettingsManager : public InputDispatcherGroupBox
{
	Q_OBJECT
public:
	explicit SettingsManager(InputDispatcher* parent);
private:
	auto AddCheckBoxes() -> void;
	auto AddSeedWidget() -> void;
	auto InitializeDesign() -> void;
private:
	std::unordered_map<SettingsWidget, QWidget*> m_widgets;
};