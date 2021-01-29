#include "cihm.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CIhm w;
    w.show();

    return a.exec();
}
