#pragma once
#include "Types.hpp"
#include "InputDispatcher.hpp"
#include <QtWidgets/QGroupBox>

class SettingsManager : public InputDispatcherGroupBox
{
	Q_OBJECT
public:
	explicit SettingsManager(InputDispatcher* parent);
private:
	auto AddMultiThreadCheckBox() -> void;
	auto AddSeedWidget() -> void;
	auto InitializeDesign() -> void;
private:
	std::unordered_map<SettingsWidget, QWidget*> m_widgets;
};
