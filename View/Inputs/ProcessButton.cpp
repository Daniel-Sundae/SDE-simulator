#include "ProcessButton.hpp"
#include "CreateProcess.hpp"
#include "Types.hpp"

ProcessButton::ProcessButton(ProcessButtonsManager *parent, const ProcessType type)
    : QPushButton(parent), m_ProcessType(type)
{
    auto metaData = ProcessMetaData::Create(m_ProcessType);
    setText(metaData.acronym);
    setToolTip(metaData.description);
    
    connect(
        this,
        &QPushButton::clicked,
        this,
        &ProcessButton::RequestPlotPath
    );
}

auto ProcessButton::RequestPlotPath() -> void
{
    parent->ButtonPressed(m_processType)
}
