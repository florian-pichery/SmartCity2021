#ifndef CCONFIG_H
#define CCONFIG_H

#include <QObject>
#include <QSettings>

class CConfig : public QSettings  
{
public:
    CConfig();
    QString _ip;
    QString _login;
    QString _mdp;
    QString _nomBDD;
    QString _addrPark;
    QString _addrEclair;
    QString _nbEclair;
    QString _addrInter;
};

#endif // CCONFIG_H
