#pragma once
#include "Types.hpp"
#include <QtWidgets/qpushbutton.h>

class ProcessButtonsManager;
class QPushButton;

class ProcessButton : public QPushButton {
	Q_OBJECT
public:
	explicit ProcessButton(ProcessButtonsManager* parent, ProcessType type);

private:
	auto RequestPlotPath() -> void;
	const ProcessType m_ProcessType;
};