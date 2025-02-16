
#include "AppInitializer.hpp"
#include <QtWidgets/qapplication.h>

int main(int argc, char *argv[])
{
    qDebug() << "Application starting...";
    QApplication app(argc, argv);
    AppInitializer a{};
    return app.exec();
}


