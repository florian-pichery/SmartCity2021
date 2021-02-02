#include "cconfig.h"


CConfig::CConfig()
{
    //creer objet avec paramètre chemin d'access du fichier et format de lecture
    QSettings settings("SmartCity.ini", QSettings::IniFormat);

    //prend valeur ici, si non trouvée prend valeur du .ini
    QString ip = settings.value("Adresses/adresseSgbd", "localhost").toString();
    QString login = settings.value("BDD/LOGIN", "sc2021").toString();
    QString mdp = settings.value("BDD/MDP", "sc2021").toString();
    QString NomBDD = settings.value("BDD/NOM_BASE", "sc2021").toString();
    QString Addr_park = settings.value("Parking/ADDR_I2C_PARK", "0x50").toString();
    QString Addr_eclair = settings.value("Parking/ADDR_I2C_ECLAIR", "0x20").toString();
    QString Addr_inter = settings.value("Parking/ADDR_I2C_INTER", "0x10").toString();
}
