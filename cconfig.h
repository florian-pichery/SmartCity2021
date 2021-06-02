#ifndef CCONFIG_H
#define CCONFIG_H

#include <QObject>
#include <QSettings>

class CConfig : public QSettings  
{
public:
    CConfig();
    QString _port;
    QString _addrPark;
    QString _addrEclair;
    QString _nbEclair;
    QString _addrInter;
};

#endif // CCONFIG_H
