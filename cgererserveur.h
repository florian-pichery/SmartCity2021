#ifndef CTCPSERVEUR_H
#define CTCPSERVEUR_H

#include <QObject>
//#include <QTcpServer>
#include <QList>
#include <QDebug>
#include <QThread>
#include <stdio.h>

#include "cgererclient.h"
#include "cmonserveurtcp.h"

class CGererServeur : public QObject
{
    Q_OBJECT
public:
    explicit CGererServeur( quint16 noPort = 2222, QObject *parent = nullptr);
    ~CGererServeur();

private:
    //objects
    CMonServeurTcp *_serv;

    //Variables
        //listes
        QList<CGererClient *> _listeClient;
        QList<QThread *> _listeThread;

    quint16 m_noPort;
    char chaine[30];

signals:
    void sig_goGestionClient();

    void sig_erreur(QString mess);
    void sig_info(QString mess);


public slots:
    void on_newConnection(qintptr sd);
    void on_disconnected();

    void on_erreur(QString mess);
    void on_info(QString mess);

};

#endif // CTCPSERVEUR_H
