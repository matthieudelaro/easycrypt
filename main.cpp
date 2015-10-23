#include <QApplication>
#include "interfaceimages.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    InterfaceImages window;
    window.show();

    return app.exec();
}


