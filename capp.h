#ifndef CAPP_H
#define CAPP_H

#include <QObject>
#include <QThread>

#include "cgestionmaquette.h"
#include "czdc.h"
#include "cconfig.h"

class CApp : public QObject
{
    Q_OBJECT
public:
    explicit CApp(QObject *parent = nullptr);
    ~CApp();

private:
    CZdc *_zdc;
    CGestionMaquette *_maquette;
    CConfig *_config;
    QThread *_gthm;

signals:
    void sig_go();

public slots:
};

#endif // CAPP_H
