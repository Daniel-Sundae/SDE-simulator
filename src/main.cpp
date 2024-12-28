
#include "MainWindow.hpp"
// #include "Utils.hpp"
// #include "SDE.hpp"
// #include <iostream>
// #include <memory>
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{

    QApplication app(argc, argv);
    MainWindow r{};
    // // Use smart pointers for better memory management
    // auto series = std::make_unique<QLineSeries>();
    
    // // Create and sample the Brownian Motion
    // SDE BM = Utils::BrownianMotion();
    // std::vector<double> BMpath = BM.SamplePath(1000, 0.05, 0);

    // // Fill the series with data
    // for (size_t i = 0; i < BMpath.size(); ++i) {
    //     series->append(static_cast<qreal>(i), static_cast<qreal>(BMpath[i]));
    // }

    // // Create chart and take ownership of series
    // auto chart = std::make_unique<QChart>();
    // chart->addSeries(series.release());  // Chart takes ownership
    // chart->setTitle("Brownian Motion Sample Path");
    // chart->createDefaultAxes();

    // // Create chart view
    // auto chartView = std::make_unique<QChartView>(chart.release());  // ChartView takes ownership
    // chartView->setRenderHint(QPainter::Antialiasing);
    // chartView->resize(800, 600);
    // chartView->show();

    return app.exec();
}


