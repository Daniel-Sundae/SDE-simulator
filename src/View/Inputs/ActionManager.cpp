#include "ActionManager.hpp"
#include "InputDispatcher.hpp"
#include "ViewUtils.hpp"

ActionManager::ActionManager(InputDispatcher *parent)
    : InputDispatcherGroupBox(parent)
    , m_goButton(new QPushButton("GO", this))
    , m_clearButton(new QPushButton("CLEAR", this))
    , m_cancelButton(new QPushButton("CANCEL", this))
{
    addActionWidgets();
}


void ActionManager::addActionWidgets(){
    m_goButton->setToolTip("Generate sample.");
    m_goButton->setStyleSheet("QPushButton { background-color: green; }");
    m_clearButton->setToolTip("Remove all samples.");
    m_clearButton->setStyleSheet("QPushButton { background-color: red; padding: 2px;}");
    m_cancelButton->setToolTip("Cancel current sampling.");
    m_cancelButton->setStyleSheet("QPushButton { background-color: red; padding: 2px;}");
    connect(
        m_goButton,
        &QPushButton::clicked,
        this,
        [this]() {
            m_goButton->setEnabled(false);
            m_clearButton->setEnabled(false);
            m_cancelButton->setEnabled(true);
            Parent()->onGoButtonClicked();
        });
    connect(
        m_clearButton,
        &QPushButton::clicked,
        this,
        [this]() {
            Parent()->onClearButtonClicked();
        });
    connect(
        m_cancelButton,
        &QPushButton::clicked,
        this,
        [this]() { Parent()->onCancelButtonClicked(); });
    auto layout = new QHBoxLayout(this);
    layout->addWidget(m_goButton);
    layout->addWidget(m_clearButton);
    layout->addWidget(m_cancelButton);
    setStyleSheet(GUI::groupBoxDescription());
}

void ActionManager::setReady(){
    m_goButton->setEnabled(true);
    m_clearButton->setEnabled(true);
    m_cancelButton->setEnabled(false);
}