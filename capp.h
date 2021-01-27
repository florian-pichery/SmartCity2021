#ifndef CAPP_H
#define CAPP_H

#include <QObject>
#include <QThread>
#include "cservertcp.h"
#include "cgestionclient.h"
#include "cgestionmaquette.h"
#include "czdc.h"
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

    //objets Server//
    QThread *_gthc;//gestion Thread Client
    CServerTcp *_serv;
    CGestionClient *_client;
    CGestionMaquette *_maquette;
    CZdc *_zdc;

signals:

    void sig_erreur(QString mess);
    void sig_info(QString mess);

    //serverTCP
    void sig_goServer();

private slots:

    void on_erreur(QString mess);
    void on_info(QString mess);

    //serverTCP
    void on_newClient(QTcpSocket *client);
    /*void onEvenementServeur(QString eve);
    void onDataRecu(QString adrIpClient, QString data);
    void onErreurServeur(QAbstractSocket::SocketError err);
    void onAdrClient(QString adrClient);
    void onListeMaJClients(QList<QTcpSocket *> liste);
    */
};

#endif // CAPP_H
