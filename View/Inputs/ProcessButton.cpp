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
        &ProcessButton::PlotPath
    );
}

auto ProcessButton::PlotPath() -> void
{
    assert(bpLink->ProcessButton() == this);
    bpLink->ButtonPresenter()->ButtonPressed(type);
}


// auto ProcessButton::OnParametersReceived(const ProcesProcessfinition& def) -> void
// {
//     Path path;
//     switch(m_ProcessType){
//         case ProcessType::BM:
//             path = Process<ProcessType::BM>::Create(def)->Sample(1000, 0.1);
//             break;
//         case ProcessType::GBM:
//             path = Process<ProcessType::GBM>::Create(def)->Sample(1000, 0.1);
//             break;
//         default:
//             throw std::runtime_error("Unexpected Process type");
//     }
//     emit RequestUpdatePathChart(path, m_ProcessType);
//     emit SetActive(this);
// }

