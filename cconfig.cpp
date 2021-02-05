#include "cconfig.h"


CConfig::CConfig()
{
    //creer objet avec paramètre chemin d'access du fichier et format de lecture
    QSettings settings("SmartCity.ini", QSettings::IniFormat);

    //prend valeur ici, si non trouvée prend valeur du .ini
    _ip = settings.value("Adresses/adresseSgbd", "localhost").toString();
    _login = settings.value("BDD/LOGIN", "sc2021").toString();
    _mdp = settings.value("BDD/MDP", "sc2021").toString();
    _nomBDD = settings.value("BDD/NOM_BASE", "sc2021").toString();
    _addrPark = settings.value("Parking/ADDR_I2C_PARK", "0x50").toString();
    _addrEclair = settings.value("Eclairage/ADDR_I2C_ECLAIR", "0x20").toString();
    _nbEclair = settings.value("Eclairage/NOMBRE_CARTES", "1").toString();
    _addrInter = settings.value("Intersection/ADDR_I2C_INTER", "0x10").toString();
}
