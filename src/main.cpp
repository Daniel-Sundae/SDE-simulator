
#include "AppInitializer.hpp"
#include <QtWidgets/qapplication.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    AppInitializer a(app);
    return app.exec();
}


