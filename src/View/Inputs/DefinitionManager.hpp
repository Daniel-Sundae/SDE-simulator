#pragma once

#include "Types.hpp"
#include "InputHandler.hpp"
#include <unordered_map>
#include <QtWidgets/qwidget.h>

class InputManager;
class QDoubleSpinBox;

class DefinitionManager : public QWidget
{
	Q_OBJECT
public:
	explicit DefinitionManager(InputManager* parent);
private:
	auto GetMuValue() const -> double;
	auto GetSigmaValue() const -> double;
	auto GetStartValue() const -> double;
	auto InitializeDefinitionManager() -> void;
	auto Parent() const -> InputManager*;
	auto OnProcessDefinitionModified(const ModifiedDefinitionParam param) const -> void;

private:
	std::unordered_map<ModifiedDefinitionParam, QDoubleSpinBox*> m_inputs;
};

