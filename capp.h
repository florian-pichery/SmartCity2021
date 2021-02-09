#ifndef CAPP_H
#define CAPP_H

#include <QObject>
#include <QThread>
#include "cgererserveur.h"
//#include "cgestionmaquette.h"
//#include "czdc.h"
//Cbdd
//CConfig


#define PORT 2222//confs

class CApp : public QObject
{
    Q_OBJECT
public:
    explicit CApp(QObject *parent = nullptr);
    ~CApp();
private:

    //CGestionMaquette *_maquette;
    //CZdc *_zdc;
    //CBdd *_bdd;
    //CConfig *_config;
    CGererServeur *_serv;

signals:

    void sig_erreur(QString mess);
    void sig_info(QString mess);


private slots:

    void on_erreur(QString mess);
    void on_info(QString mess);

};

#endif // CAPP_H
