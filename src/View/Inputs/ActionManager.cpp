#include "ActionManager.hpp"
#include "InputDispatcher.hpp"
#include "ViewUtils.hpp"

ActionManager::ActionManager(InputDispatcher *parent)
    : InputDispatcherGroupBox(parent)
{
    addActionWidgets();
}


void ActionManager::addActionWidgets(){
    auto* goButton = new QPushButton("GO", this);
    goButton->setToolTip("Generate sample.");
    goButton->setStyleSheet("QPushButton { background-color: green; }");
    auto* clearButton = new QPushButton("CLEAR", this);
    clearButton->setToolTip("Remove all samples.");
    clearButton->setStyleSheet("QPushButton { background-color: red; padding: 2px;}");
    auto* cancelButton = new QPushButton("CANCEL", this);
    cancelButton->setToolTip("Cancel current sampling.");
    cancelButton->setStyleSheet("QPushButton { background-color: red; padding: 2px;}");
    connect(
        goButton,
        &QPushButton::clicked,
        this,
        [this]() { Parent()->onGoButtonClicked(); });
    connect(
        clearButton,
        &QPushButton::clicked,
        this,
        [this]() { Parent()->onClearButtonClicked(); });
    connect(
        cancelButton,
        &QPushButton::clicked,
        this,
        [this]() { Parent()->onCancelButtonClicked(); });
    auto layout = new QHBoxLayout(this);
    layout->addWidget(goButton);
    layout->addWidget(clearButton);
    layout->addWidget(cancelButton);
    setStyleSheet(GUI::groupBoxDescription());
}