#include "cbdd.h"
#include "QSqlDatabase"
#include <QDebug>
#include <QSqlError>
#include <QString>
#include <iostream>
#include <string>
#include <QSqlQuery>
#include <QSettings>
#include <QStringListModel>
#include <QStringList>
#include <QAbstractItemView>

CBdd::CBdd(QObject *parent) : QObject(parent)
{

}

bool CBdd::verifierParamsConnexion()
{
    // Instanciation de la variable membre dbMeteo
    dbSC = QSqlDatabase::addDatabase("QMYSQL");

    // Définition des paramètres de connexion à la base de données
    dbSC.setHostName("192.168.43.199"); // @ip serveur MySQL
    dbSC.setDatabaseName("bdd_sc"); // Nom de la base
    dbSC.setUserName("Durant"); // Nom utilisateur
    dbSC.setPassword("Durant"); // Mot de passe

    if(dbSC.open()) {
        qDebug() << "Ok - ouverture de la base de donnée";
        connexionFaite = 1;
    } else {
        qDebug() << "Echec d'ouverture de la base de donnée";
        qDebug() << dbSC.lastError();
        connexionFaite = 0;
    }
    return connexionFaite;
}

QString CBdd::get_idClient()
{
    //bool connexionFaite = verifierParamsConnexion();

    // Exécution d'une requête
    QSqlQuery requete;
    QStringList liste;
    if(requete.exec("SELECT * FROM clients")) {
        qDebug() << "Début de la requête";

        // Boucle qui permet de parcourir les enregistrements renvoyés par la requête
        while(requete.next()) {
            // On accède ici aux différents champs par leurs noms, il est également possible
            // d'y accéder par leur index : requete.value(0)
            login = requete.value("login").toString();
            qDebug() << "login : " << login;
        }
    }else {
        qDebug() << "Echec de la requête";
        // La méthode lastError permet d'afficher un message
        // plus explicite sur les causes de l'erreur
        qDebug() << requete.lastError();
    }
    dbSC.close(); // Fermeture de la base de données
    return login;
}

QString CBdd::get_mdpClient()
{

    //bool connexionFaite = verifierParamsConnexion();

    // Exécution d'une requête
    QSqlQuery requete;
    QStringList liste;
    if(requete.exec("SELECT * FROM clients")) {
        qDebug() << "Début de la requête";

        // Boucle qui permet de parcourir les enregistrements renvoyés par la requête
        while(requete.next()) {
            // On accède ici aux différents champs par leurs noms, il est également possible
            // d'y accéder par leur index : requete.value(0)
            mdp = requete.value("mdp").toString();
            qDebug() << "mdp : " << mdp;
        }
    }else {
        qDebug() << "Echec de la requête";
        // La méthode lastError permet d'afficher un message
        // plus explicite sur les causes de l'erreur
        qDebug() << requete.lastError();
    }
 dbSC.close(); // Fermeture de la base de données
 return mdp;
 return "mdp";
}
