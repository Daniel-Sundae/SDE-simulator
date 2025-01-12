#pragma once
#include "ButtonWidgetManager.hpp"

class SDEButtonsWidget : public QWidget
{
	Q_OBJECT
public:
	explicit SDEButtonsWidget(ButtonWidgetManager* parent);
signals:
	void SDEButtonPathSignal(
		const Path& data,
		const QString& label);
private:
	auto InitializeSDEButtonsWidget() -> void;
	auto CreateSDEButton(
		const SDEMetaData& metaData,
		std::function<std::vector<double>()> sample) -> QPushButton*;
};