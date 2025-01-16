#pragma once
#include "Types.hpp"
#include <QtWidgets/qpushbutton.h>

class SDEButton : public QPushButton {
	Q_OBJECT
public:
	explicit SDEButton(const SDEButtonWidget* parent, std::function<Path()> onClick);
private:
	SDEType m_SDE;
	std::function<Path()> m_onClick;
};