#pragma once

#include "Types.hpp"
#include <unordered_map>
#include <QWidget>

class InputManager;

class ParametersManager : public QWidget
{
	Q_OBJECT
public:
	explicit ParametersManager(InputManager* parent);
private:
	auto GetMuValue() const -> double;
	auto GetSigmaValue() const -> double;
	auto GetStartValue() const -> double;
	auto InitializeParametersManager() -> void;
	auto Parent() const -> InputManager*;
	enum class InputType {
		MU = 0,
		SIGMA,
		STARTVALUE
	};
	std::unordered_map<InputType, QWidget*> m_inputs;

private slots:
	auto OnParametersChanged() const -> void;
};

