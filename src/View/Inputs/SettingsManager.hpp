#pragma once
#include "Types.hpp"
#include <QtWidgets/qgroupbox.h>
class InputManager;

class SettingsManager : public QGroupBox
{
	Q_OBJECT
public:
	explicit SettingsManager(InputManager* parent);
private:
	auto AddCheckBoxes() -> void;
	auto AddSeedWidget() -> void;
	auto InitializeDesign() -> void;
	auto Parent() const -> InputManager*;
private:
	std::unordered_map<SettingsWidget, QWidget*> m_widgets;
};