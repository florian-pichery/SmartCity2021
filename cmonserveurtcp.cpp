#include "cmonserveurtcp.h"

CMonServeurTcp::CMonServeurTcp(QObject *parent) : QTcpServer(parent)
{

}


void CMonServeurTcp::incomingConnection(qintptr sd)
{
    //quand on reçoit une connexion
    emit sig_sdClient(sd);//Envoie du descripteur de la socket
}
