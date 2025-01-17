#pragma once
#include "Types.hpp"
#include <QtWidgets/qpushbutton.h>

class SDEButtonsManager;
class QPushButton;

class SDEButton : public QPushButton {
	Q_OBJECT
    /* Path Generation Sequence:
     * 1. User clicks button
     * 2. StartSamplePath() is called and emits RequestParameters()
     * 3. External system receives RequestParameters,
     *	  fetches current parameters and calls OnParametersReceived() with value
     * 4. OnParametersReceived() generates path and emits RequestUpdatePathChart with path
     */
public:
	explicit SDEButton(SDEButtonsManager* parent, SDEType type);
signals:
	auto RequestParameters() -> void;
	auto RequestUpdatePathChart(
		const Path& data,
		const SDEType type) -> void;
	auto SetActive(SDEButton*) -> void;

public slots:
	auto OnParametersReceived(const SDEDefinition& def) -> void;
private slots:
	auto StartSamplePath() -> void;
private:
	const SDEType m_sdeType;
};