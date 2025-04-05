#include "ActionManager.hpp"
#include "InputDispatcher.hpp"
#include "ViewUtils.hpp"

ActionManager::ActionManager(InputDispatcher *parent)
    : InputDispatcherGroupBox(parent)
{
    AddActionWidgets();
}


auto ActionManager::AddActionWidgets() -> void
{
    auto* goButton = new QPushButton("GO", this);
    goButton->setToolTip("Generate sample.");
    goButton->setStyleSheet("QPushButton { background-color: green; }");
    auto* clearButton = new QPushButton("CLEAR", this);
    clearButton->setToolTip("Remove all samples.");
    clearButton->setStyleSheet("QPushButton { background-color: red; }");
    connect(
        goButton,
        &QPushButton::clicked,
        this,
        [this]() { Parent()->OnGoButtonClicked(); });
    connect(
        clearButton,
        &QPushButton::clicked,
        this,
        [this]() { Parent()->OnClearButtonClicked(); });
    auto layout = new QHBoxLayout(this);
    layout->addWidget(goButton);
    layout->addWidget(clearButton);
    setStyleSheet(GUI::GroupBoxDescription());
}