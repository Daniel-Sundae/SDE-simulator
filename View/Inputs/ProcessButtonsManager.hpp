#pragma once
#include "ButtonWidgetManager.hpp"
#include <unordered_map>
#include <memory>

class ProcessButton;

class ProcessButtonsManager : public QWidget
{
	Q_OBJECT
public:
	explicit ProcessButtonsManager(ButtonWidgetManager* parent);
signals:
	auto ForwardRequestUpdatePathChart(
		const Path& data,
		const ProcessType type) -> void;
	auto RequestParameters() -> void;
public slots:
	auto OnParametersReceived(const ProcesProcessfinition& def) -> void;

private:
	auto InitializeProcessButtonsManager() -> void;
	auto CreateProcessButtons() -> void;
	auto SetupConnections() -> void;
	auto SetActive(ProcessButton* button) -> void;
	auto ClearActive() -> void;

private:
	std::unordered_map<ProcessType, ProcessButton*> m_buttons;
	ProcessButton* m_activeButton;
};