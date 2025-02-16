#include "ProcessButton.hpp"
#include "ProcessData.hpp"
#include "ProcessButtonsManager.hpp"
#include "Types.hpp"

ProcessButton::ProcessButton(ProcessButtonsManager *parent, const ProcessType type)
    : QPushButton(parent)
    , m_processType(type)
{
    setText(QString::fromStdString(std::string(ProcessData::Acronym(type))));
    setToolTip("testdescr");
    
    connect(
        this,
        &QPushButton::clicked,
        this,
        &ProcessButton::PlotPath
    );
}

auto ProcessButton::PlotPath() const -> void
{
    static_cast<ProcessButtonsManager*>(parent())->OnButtonPressed(m_processType);
}
