#ifndef CBDD_H
#define CBDD_H

#include <QObject>
#include "QSqlDatabase"
#include <QString>

class CBdd : public QObject
{
    Q_OBJECT
public:
    explicit CBdd(QObject *parent = nullptr);
    bool verifierParamsConnexion();
    QString get_idClient();
    QString get_mdpClient();
    QString login;
    QString mdp;

private:
    QSqlDatabase dbSC;
    bool connexionFaite;

signals:

public slots:
};

#endif // CBDD_H
