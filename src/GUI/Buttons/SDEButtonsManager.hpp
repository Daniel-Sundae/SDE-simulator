#pragma once
#include "ButtonWidgetManager.hpp"
#include <unordered_map>
#include <memory>

class SDEButton;

class SDEButtonsManager : public QWidget
{
	Q_OBJECT
public:
	explicit SDEButtonsManager(ButtonWidgetManager* parent);
signals:
	auto ForwardRequestUpdatePathChart(
		const Path& data,
		const SDEType type) -> void;
	auto RequestParameters() -> void;
public slots:
	auto OnParametersReceived(const SDEDefinition& def) -> void;

private:
	auto InitializeSDEButtonsManager() -> void;
	auto CreateSDEButtons() -> void;
	auto SetupConnections() -> void;
	auto SetActive(SDEButton* button) -> void;
	auto ClearActive() -> void;

private:
	std::unordered_map<SDEType, SDEButton*> m_buttons;
	SDEButton* m_activeButton;
};