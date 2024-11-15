#include <QApplication>
#include <QMainWindow>
#include <QtCharts>
#include <QLineSeries>
#include "Wienerprocess.hpp"

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);
    QMainWindow window;

    // Create the series and add data points
    auto series = new QLineSeries();
    for (int i = 0; i < 500; i++) {
        series->append(i, qSin(M_PI / 50 * i) * 100);
    }

    // Create and setup the chart
    auto chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Simple Sine Wave");
    chart->createDefaultAxes();
    
    // Create the chart view and set it as the central widget
    auto chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    window.setCentralWidget(chartView);

    // Show the window
    window.resize(800, 600);
    window.show();

    return app.exec();
}