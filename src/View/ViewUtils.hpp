#pragma once
#include <QtCore/qstring.h>
#include <QtWidgets/qspinbox.h>
#include <QtWidgets/qpushbutton.h>
#include <QtCharts>

namespace GUI
{
    inline auto GroupBoxDescription() -> QString
    {
        return R"(QGroupBox {
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
            })";
    }

    inline auto ComboBoxDescription() -> QString
    {
        return R"(QComboBox {
                min-height: 20px;
                min-width: 80px;
                max-width: 80px;
            })";
    }

    inline auto SpinBoxDescription() -> QString
    {
        return R"(QDoubleSpinBox {
                min-height: 20px;
                min-width: 80px;
                max-width: 80px;
            }
            QSpinBox {
                min-height: 20px;
                min-width: 80px;
                max-width: 80px;
            })";
    }

    inline auto CheckBoxDescription() -> QString
    {
        return R"(QCheckBox {
            min-height: 20px;
            spacing: 5px;
        }
        QCheckBox::indicator:unchecked {
            border: 2px solid #CCCCCC;
            background-color: white;
        }
        )";
    }

    inline auto SetDriftStyle(QLineSeries* driftSeries) -> void
    {
        const QColor highlightColor = QColor(Qt::GlobalColor::red);
        QPen seriesPen(highlightColor, 1);
        seriesPen.setStyle(Qt::PenStyle::DashLine);
        driftSeries->setPen(seriesPen);
    }

    inline auto SetPDFStyle(QLineSeries* driftSeries) -> void
    {
        const QColor highlightColor = QColor(255, 121, 0); // Orange
        QPen seriesPen(highlightColor, 1);
        driftSeries->setPen(seriesPen);
    }

    inline auto SetPathStyle(QLineSeries* series) -> void
    {
        const QColor highlightColor = QColor(42, 130, 218); // Blue
        const QPen seriesPen(highlightColor, 1);
        series->setPen(seriesPen);
    }

    inline auto SetChartStyle(QChart* chart) -> void
    {
        chart->setBackgroundBrush(QColor(25, 25, 25));
        chart->setBackgroundPen(Qt::NoPen);
        chart->setTitleBrush(QBrush(Qt::white));

        const QList<QAbstractAxis*> axes = chart->axes();
        for (QAbstractAxis* axis : axes) {
            axis->setLabelsColor(Qt::white);
            axis->setTitleBrush(QBrush(Qt::white));
            axis->setLinePen(QPen(Qt::white));
        }
    }
}

