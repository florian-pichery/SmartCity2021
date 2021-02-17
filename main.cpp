#include "cihm.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    cihm w;
    w.show();
    return a.exec();
}
