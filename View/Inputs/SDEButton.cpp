#include "SDEButton.hpp"
#include "CreateSDE.hpp"
#include "Types.hpp"

SDEButton::SDEButton(SDEButtonsManager *parent, const SDEType type)
    : QPushButton(parent), m_sdeType(type)
{
    auto metaData = SDEMetaData::Create(m_sdeType);
    setText(metaData.acronym);
    setToolTip(metaData.description);
    
    connect(
        this,
        &QPushButton::clicked,
        this,
        &SDEButton::StartSamplePath
    );
}

auto SDEButton::StartSamplePath() -> void
{
    emit RequestParameters();
}

auto SDEButton::OnParametersReceived(const SDEDefinition& def) -> void
{
    Path path;
    switch(m_sdeType){
        case SDEType::BM:
            path = SDE<SDEType::BM>::Create(def)->Sample(1000, 0.1);
            break;
        case SDEType::GBM:
            path = SDE<SDEType::GBM>::Create(def)->Sample(1000, 0.1);
            break;
        default:
            throw std::runtime_error("Unexpected SDE type");
    }
    emit RequestUpdatePathChart(path, m_sdeType);
    emit SetActive(this);
}

