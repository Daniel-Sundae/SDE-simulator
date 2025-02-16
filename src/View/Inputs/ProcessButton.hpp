#pragma once
#include "Types.hpp"
#include <QtWidgets/qpushbutton.h>

class ProcessButtonsManager;
class QPushButton;

class ProcessButton : public QPushButton {
	Q_OBJECT
public:
	explicit ProcessButton(ProcessButtonsManager* parent, ProcessType type);

	auto PlotPath() const -> void;

private:
	const ProcessType m_processType;
};