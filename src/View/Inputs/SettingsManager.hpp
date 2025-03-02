#pragma once
#include "Types.hpp"
#include "InputManager.hpp"
#include <QtWidgets/qgroupbox.h>

class SettingsManager : public InputManagerGroupBox
{
	Q_OBJECT
public:
	explicit SettingsManager(InputManager* parent);
private:
	auto AddCheckBoxes() -> void;
	auto AddSeedWidget() -> void;
	auto InitializeDesign() -> void;
private:
	std::unordered_map<SettingsWidget, QWidget*> m_widgets;
};