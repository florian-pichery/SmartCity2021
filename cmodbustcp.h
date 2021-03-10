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
    bool verifier();
    int on_trameClient(QByteArray trameClient);

private:

    QString _data;
    QByteArray _tc;  // trame du client
    uint16_t calculCrc16();

signals:
    void sig_erreur(QString mess);
    void sig_info(QString mess);


};

#endif // CMODBUSTCP_H
