#pragma once
#include "DataLink.hpp"
#include "ButtonPresenter.hpp"
#include "ProcessButton.hpp"

class ButtonPresenterLink : public DataFlowLink{
public:
    explicit ButtonPresenterLink(std::unique_ptr<ButtonPresenter> buttonPresenter,
                                std::unique_ptr<ProcessButton> processButton)
        : m_buttonPresenter(std::move(buttonPresenter))
        , m_processButton(std::move(processButton)){};

    auto ButtonPresenter() -> const ButtonPresenter&
    {return *m_buttonPresenter;}
    auto ProcessButton() -> const ProcessButton&
    {return *m_processButton;}

private:
    std::unique_ptr<ButtonPresenter> m_buttonPresenter;
    std::unique_ptr<ProcessButton> m_processButton;    
};