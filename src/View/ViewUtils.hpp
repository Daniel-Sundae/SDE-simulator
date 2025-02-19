#pragma once
#include <QString>

namespace GUI
{
    inline auto GroupBoxDescription() -> QString
    {
        return R"(
            QGroupBox {
                border: 2px solid #CCCCCC;
                border-radius: 5px;
                font-size: 12px;
                margin-top: 1em;
                padding-top: 1px;
                padding: 10px;
            }
            QGroupBox::title {
                subcontrol-origin: margin;
                subcontrol-position: top center;
                padding: 0 3px;
                top: 0em; 
            }
        )";
    }
    inline auto SpinBoxDescription() -> QString
    {
        return R"(
            QDoubleSpinBox {
                min-height: 20px;
                min-width: 80px;
                max-width: 80px;
            }")";
    }
}

