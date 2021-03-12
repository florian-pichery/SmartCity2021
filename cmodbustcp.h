#ifndef CMODBUSTCP_H
#define CMODBUSTCP_H

#include <QObject>
#include <QDebug>
#include <QChar>
#include <QByteArray>

class CModbusTcp : public QObject
{
    Q_OBJECT

public:
    explicit CModbusTcp(QObject *parent = nullptr);
    ~CModbusTcp();

    int decoder();
    bool verifierCRC16();
    int on_trameClient(QByteArray trameClient);
    void deleteTc();

private:
    QString _data;
    QByteArray _tc;  // trame du client
    uint16_t calculCrc16();

signals:
    void sig_erreur(QString mess);
    void sig_info(QString mess);


};

#endif // CMODBUSTCP_H
