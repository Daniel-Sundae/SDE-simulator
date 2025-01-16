#pragma once
#include "ButtonWidgetManager.hpp"
#include <unordered_map>
class SDEButtonsWidget : public QWidget
{
	Q_OBJECT
public:
	explicit SDEButtonsWidget(ButtonWidgetManager* parent);
signals:
	void RequestUpdatePathChart(
		const Path& data,
		const QString& label);
private:
	auto InitializeSDEButtonsWidget() -> void;
	auto CreateSDEButtons() -> void;
	auto CreateSDEButton(
		const SDEMetaData& metaData,
		std::function<std::vector<double>()> sample) -> QPushButton*;
private:
	std::unordered_map<std::string, QPushButton*> m_buttons;
};