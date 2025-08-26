#pragma once
#include "Types.hpp"
#include "InputDispatcher.hpp"

class QCheckBox;
class QWidget;

class SettingsManager : public InputDispatcherGroupBox
{
    Q_OBJECT
public:
    explicit SettingsManager(InputDispatcher* parent);
private:
    void addMultiThreadCheckBox();
    void addSeedWidget();
    void initializeDesign();
private:
    QCheckBox* m_multiThreadingCheckBox;
    QWidget* m_seedWidget;
};
