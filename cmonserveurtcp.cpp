#include "cmonserveurtcp.h"

CMonServeurTcp::CMonServeurTcp(QObject *parent) : QTcpServer(parent)
{

}


void CMonServeurTcp::incomingConnection(qintptr sd)
{
    emit sig_sdClient(sd);
}
