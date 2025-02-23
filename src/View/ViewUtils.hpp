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

    inline auto CreateIntSpinBox(QWidget* parent, int min, int max, int default_value, int step) -> QSpinBox*
    {
        auto* spinBox = new QSpinBox(parent);
        spinBox->setRange(min, max);
        spinBox->setValue(default_value);
        spinBox->setSingleStep(step);
        return spinBox;
    };

    inline auto CreateDoubleSpinBox(QWidget* parent, double min, double max, double default_value, double step) -> QDoubleSpinBox*
    {
        auto* spinBox = new QDoubleSpinBox(parent);
        spinBox->setRange(min, max);
        spinBox->setValue(default_value);
        spinBox->setSingleStep(step);
        return spinBox;
    };

    inline auto DarkChartTheme(QChart* chart) -> void
    {
        // Background colors
        chart->setBackgroundBrush(QColor(25, 25, 25));
        chart->setBackgroundPen(Qt::NoPen);
        chart->setPlotAreaBackgroundBrush(QColor(25, 25, 25));
        chart->setPlotAreaBackgroundVisible(true);

        // Title
        chart->setTitleBrush(QBrush(Qt::white));

        // Legend
        auto* legend = chart->legend();
        legend->setLabelColor(Qt::white);
        legend->setBackgroundVisible(true);
        legend->setColor(Qt::white);
        legend->setBrush(QColor(53, 53, 53));

        // Configure all axes
        const QList<QAbstractAxis*> axes = chart->axes();
        for (QAbstractAxis* axis : axes) {
            axis->setLabelsColor(Qt::white);
            axis->setTitleBrush(QBrush(Qt::white));
            axis->setGridLineColor(QColor(75, 75, 75));
            axis->setMinorGridLineColor(QColor(65, 65, 65));
            axis->setLinePen(QPen(Qt::white));
        }

        // Series colors
        const QPen seriesPen(QColor(42, 130, 218), 2);

        // Apply to all series
        const QList<QAbstractSeries*> seriesList = chart->series();
        for (QAbstractSeries* series : seriesList) {
            if (auto* lineSeries = qobject_cast<QLineSeries*>(series)) {
                lineSeries->setPen(seriesPen);
            }
            // Handle other series types if needed
            else if (auto* scatterSeries = qobject_cast<QScatterSeries*>(series)) {
                scatterSeries->setPen(seriesPen);
                scatterSeries->setBrush(QBrush(QColor(42, 130, 218)));
            }
            else if (auto* splineSeries = qobject_cast<QSplineSeries*>(series)) {
                splineSeries->setPen(seriesPen);
            }
            // Add more series types as needed
        }
    }

}

