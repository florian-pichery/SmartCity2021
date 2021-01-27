#include "cihmi2c.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CIhmI2c w;
    w.show();

    return a.exec();
}
