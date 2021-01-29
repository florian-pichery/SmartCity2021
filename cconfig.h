#ifndef CCONFIG_H
#define CCONFIG_H

#include <QObject>
#include <QSettings>

class CConfig : public QSettings  
{
public:
    CConfig();
};

#endif // CCONFIG_H
