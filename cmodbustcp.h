#ifndef CMODBUSTCP_H
#define CMODBUSTCP_H

#include <QObject>

class CModbusTcp : public QObject
{
    Q_OBJECT
public:
    explicit CModbusTcp(QObject *parent = nullptr);
    ~CModbusTcp();

signals:
    void sig_erreur(QString mess);
    void sig_info(QString mess);

public slots:
};

#endif // CMODBUSTCP_H
