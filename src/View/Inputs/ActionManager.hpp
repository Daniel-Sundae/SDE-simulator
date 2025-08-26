#pragma once

#include "InputHandler.hpp"
#include "InputDispatcher.hpp"

class QPushButton;

class ActionManager : public InputDispatcherGroupBox
{
    Q_OBJECT
public:
    explicit ActionManager(InputDispatcher* parent);
    void setReady();
private:
    void addActionWidgets();
private:
    QPushButton* m_goButton;
    QPushButton* m_clearButton;
    QPushButton* m_cancelButton;
};

