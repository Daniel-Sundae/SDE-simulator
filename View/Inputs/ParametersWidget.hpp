#pragma once
#include "ButtonWidgetManager.hpp"
#include "Types.hpp"

class SDEButtonsManager;

class ParametersWidget : public QWidget
{
	Q_OBJECT
public:
	explicit ParametersWidget(ButtonWidgetManager* parent);
signals:
	auto EmitParameters(const SDEDefinition& def) -> void;
public slots:
	auto RequestParameters() -> void;
protected:
	auto GetMuValue() const -> double;
	auto GetSigmaValue() const -> double;
	auto GetStartValue() const -> double;
private:
	auto InitializeParametersWidget() -> void;
	friend class SDEButtonsManager;
};