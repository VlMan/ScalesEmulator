#include "scalesserver.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ScalesServer w;
    w.show();
    return a.exec();
}
