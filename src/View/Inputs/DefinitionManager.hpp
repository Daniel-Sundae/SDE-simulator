#pragma once

#include "Types.hpp"
#include "InputDispatcher.hpp"

class QComboBox;
class QDoubleSpinBox;

class DefinitionManager : public InputDispatcherGroupBox
{
    Q_OBJECT
public:
    explicit DefinitionManager(InputDispatcher* parent);
private:
    void updateWidgetProperties(ProcessType process);
    void addDefinitionWidgets();
    void initializeDesign();
private:
    QComboBox* m_process;
    QDoubleSpinBox* m_muWidget;
    QDoubleSpinBox* m_sigmaWidget;
    QDoubleSpinBox* m_X0Widget;
};
