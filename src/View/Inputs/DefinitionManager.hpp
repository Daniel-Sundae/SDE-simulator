#pragma once

#include "Types.hpp"
#include "InputHandler.hpp"
#include <unordered_map>
#include <QtWidgets/qgroupbox.h>

class InputManager;
class QPushButton;
class QDoubleSpinBox;

class DefinitionManager : public QGroupBox
{
	Q_OBJECT
public:
	explicit DefinitionManager(InputManager* parent);
private:
	auto GetMuValue() const -> double;
	auto GetSigmaValue() const -> double;
	auto GetStartValue() const -> double;
	auto AddDefinitionWidgets() -> void;
	auto InitializeDesign() -> void;
	auto Parent() const -> InputManager*;
	auto OnProcessTypeModified(const ProcessType newType) const -> void;
	auto OnProcessDefinitionModified(const ModifiedDefinitionParam param) const -> void;
private:
	std::unordered_map<ModifiedDefinitionParam, QWidget*> m_widgets;
};

