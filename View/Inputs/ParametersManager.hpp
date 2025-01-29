#pragma once
#include "InputPresenter.hpp"
#include "Types.hpp"

class ProcessButtonsManager;

class ParametersManager : public QWidget
{
	Q_OBJECT
public:
	explicit ParametersManager(InputManager* parent, std::shared_ptr<InputPresenter> parameterPresenter);
protected:
	auto GetMuValue() const -> double;
	auto GetSigmaValue() const -> double;
	auto GetStartValue() const -> double;
private:
	std::shared_ptr<InputPresenter> m_parameterPresenter;
	auto InitializeParametersWidget() -> void;
};