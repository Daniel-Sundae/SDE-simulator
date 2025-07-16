#pragma once
#include <QtCore/QString>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QPushButton>
#include <QtCharts>

namespace GUI
{
    inline QString groupBoxDescription(){
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

    inline QString comboBoxDescription(){
        return R"(QComboBox {
                min-height: 20px;
                min-width: 80px;
                max-width: 80px;
            })";
    }

    inline QString spinBoxDescription(){
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

    inline QString checkBoxDescription(){
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

    inline void setDriftStyle(QLineSeries* driftSeries){
        const QColor highlightColor = QColor(Qt::GlobalColor::red);
        QPen seriesPen(highlightColor, 1);
        seriesPen.setStyle(Qt::PenStyle::DashLine);
        driftSeries->setPen(seriesPen);
    }

    inline void setPDFStyle(QLineSeries* driftSeries){
        const QColor highlightColor = QColor(255, 121, 0); // Orange
        QPen seriesPen(highlightColor, 1);
        driftSeries->setPen(seriesPen);
    }

    inline void setPathStyle(QLineSeries* series){
        const QColor highlightColor = QColor(42, 130, 218); // Blue
        const QPen seriesPen(highlightColor, 1);
        series->setPen(seriesPen);
    }

    inline void setChartStyle(QChart* chart){
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
