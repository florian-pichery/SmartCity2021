#ifndef CMYTCPSERVER_H
#define CMYTCPSERVER_H


#include <QObject>
#include <QTcpServer>

class CMonServeurTcp : public QTcpServer
{
       Q_OBJECT
public:
    explicit CMonServeurTcp(QObject *parent = Q_NULLPTR);

protected:
    void incomingConnection(qintptr sd) override; // réimplémentation

signals:
    void sig_sdClient(qintptr sd);
};

#endif // CMYTCPSERVER_H
