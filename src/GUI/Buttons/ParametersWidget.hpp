#pragma once
#include "ButtonWidgetManager.hpp"

class SDEButtonsWidget;

class ParametersWidget : public QWidget
{
	Q_OBJECT
public:
	explicit ParametersWidget(ButtonWidgetManager* parent);
protected:
	auto GetMuValue() const -> double;
	auto GetSigmaValue() const -> double;
	auto GetStartValue() const -> double;
private:
	auto InitializeParametersWidget() -> void;
	friend class SDEButtonsWidget;
};