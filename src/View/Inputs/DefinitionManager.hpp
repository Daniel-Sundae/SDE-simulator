#pragma once

#include "Types.hpp"
#include "InputDispatcher.hpp"
#include <unordered_map>
#include <QtWidgets/QGroupBox>

class DefinitionManager : public InputDispatcherGroupBox
{
    Q_OBJECT
public:
    explicit DefinitionManager(InputDispatcher* parent);
private:
    void updateWidgetProperties(ProcessType process, bool initialize = false);
    void addDefinitionWidgets();
    void initializeDesign();
private:
    std::unordered_map<DefinitionWidget, QWidget*> m_widgets;
};
