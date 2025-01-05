
#include "MainWindow.hpp"
#include <QtWidgets/qapplication.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow r{};
    return app.exec();
}


