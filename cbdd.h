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
    QString set_i2cAfficheur(QString adresseI2C, QString Message);
    QString mod_i2cAfficheur(QString adresseI2C, QString Message);
    QString del_i2cAfficheur(QString adresseI2C);
    QString set_i2cEclairage(QString adresseI2C, QString etatLampadaire, QString capteurPres1, QString capteurPres2, QString nombreDeLampadaireFonctionnels);
    QString mod_i2cEclairage(QString adresseI2C, QString etatLampadaire, QString capteurPres1, QString capteurPres2, QString nombreDeLampadaireFonctionnels);
    QString del_i2cEclairage(QString adresseI2C);
    QString set_i2cIntersection(QString adresseI2C, QString etatMode, QString boutonPieton1, QString boutonPieton2, QString etatVoie1, QString etatVoie2);
    QString mod_i2cIntersection(QString adresseI2C, QString etatMode, QString boutonPieton1, QString boutonPieton2, QString etatVoie1, QString etatVoie2);
    QString del_i2cIntersection(QString adresseI2C);
    QString set_i2cParking(QString adresseI2C, QString etatsEnDecimal);
    QString mod_i2cParking(QString adresseI2C, QString etatsEnDecimal);
    QString del_i2cParking(QString adresseI2C);

private:
    QSqlDatabase dbSC;
    bool connexionFaite;
    QString adrI2C;
    QString etatLampadaire;
    QString capteurPres1;
    QString capteurPres2;
    QString lampadairOK;
    QString etatMode;
    QString boutonPieton1;
    QString boutonPieton2;
    QString etatVoie1;
    QString etatVoie2;
    QString etats;


signals:

public slots:
};

#endif // CBDD_H
