#include <QApplication>             // for QApplication
#include <QtCharts>
#include "Utils.hpp"
#include "SDE.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 1. Create your SDE object and sample the BM path
    SDE BM = Utils::BrownianMotion();
    std::vector<double> BMpath = BM.SamplePath(1000, 0.05, 0);

    // 2. Create a QLineSeries and fill it with data from BMpath
    QLineSeries *series = new QLineSeries;
    for (int i = 0; i < static_cast<int>(BMpath.size()); ++i) {
        series->append(i, BMpath[i]);
    }

    // 3. Create a chart and add the series
    QChart *chart = new QChart;
    chart->addSeries(series);
    chart->setTitle("Brownian Motion Sample Path");
    chart->createDefaultAxes();

    // 4. Create a QChartView and set render hints (for smoother lines)
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // 5. Show the chart in a window
    chartView->resize(800, 600);
    chartView->show();

    return app.exec();
}


