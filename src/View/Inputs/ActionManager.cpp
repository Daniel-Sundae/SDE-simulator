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
    clearButton->setStyleSheet("QPushButton { background-color: red; padding: 2px;}");
    auto* cancelButton = new QPushButton("CANCEL", this);
    cancelButton->setToolTip("Cancel current sampling.");
    cancelButton->setStyleSheet("QPushButton { background-color: red; padding: 2px;}");
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
    connect(
        cancelButton,
        &QPushButton::clicked,
        this,
        [this]() { Parent()->OnCancelButtonClicked(); });
    auto layout = new QHBoxLayout(this);
    layout->addWidget(goButton);
    layout->addWidget(clearButton);
    layout->addWidget(cancelButton);
    setStyleSheet(GUI::GroupBoxDescription());
}