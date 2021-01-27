#include "ci2c.h"

CI2c::CI2c(QObject *parent, char noBus) :
    QObject(parent)
{
    m_parent = parent;
    m_noBus = noBus;
    m_nbLink=0;
} // constructeur

CI2c * CI2c::m_singleton = nullptr;

int CI2c::lire(unsigned char addr,  unsigned char *buffer, int lg)
{
    if(ioctl(m_fileI2c, I2C_SLAVE, addr)!=0) {  // Règle le driver I2C sur l'adresse.
        QString mess="CI2c::lire Erreur ioctl acces au bus I2C";
//        qDebug() << mess;
        emit sigErreur(mess);
        return -1;
    } // if ioctl
    bzero(buffer, lg);
    QMutexLocker lock(&this->m_mutexI2c);  // verrouillage du mutex. Il est libéré en sortie de méthode

    int nb=read(m_fileI2c, buffer, lg);
//    qDebug() << "CI2c:lire: " << buffer[0] << " " << buffer[1] << buffer[2] << " " << buffer[3] << buffer[4] << " " << buffer[5];
    return nb;
} // lire

int CI2c::ecrire(unsigned char addr, unsigned char *buffer, int lg)
{
    if(ioctl(m_fileI2c, I2C_SLAVE, addr)!=0) {  // Règle le driver I2C sur l'adresse.
        QString mess="CI2c::ecrire Erreur ioctl acces au bus I2C";
//        qDebug() << mess;
        emit sigErreur(mess);
        return -1;
    } // if ioctl

    QMutexLocker lock(&this->m_mutexI2c);  // verrouillage du mutex. Il est libéré en sortie de méthode

    int nb=write(m_fileI2c, buffer, lg);
//    qDebug() << "CI2c:ecrire: nb=" << nb << " : " << buffer[0] << " " << buffer[1] << buffer[2];
    return nb;
} // ecrire

int CI2c::init()
{
    char filename[20];
    sprintf(filename, "/dev/i2c-%c",m_noBus);
    if((m_fileI2c=open(filename, O_RDWR))==-1) {  // ouvre le fichier virtuel d'accès à l'I2C
        QString mess="CI2c::init Erreur ouverture acces au bus I2C";
//        qDebug() << mess;
        emit sigErreur(mess);
        return -1;
    } // if open
    return m_fileI2c;
} // init

int CI2c::getNbLink()
{
    return m_nbLink;
} // getNbLink

CI2c *CI2c::getInstance(QObject *parent, char no)
{
    if (m_singleton == nullptr)
    {
        qDebug("L'objet CI2c est créé");
        m_singleton =  new CI2c(parent, no);
        m_singleton->init();
        m_singleton->m_nbLink=1;
    }
    else
    {
        m_singleton->m_nbLink++;
        qDebug("singleton est déjà existant");
    }
    return m_singleton;
} // getInstance

void CI2c::freeInstance()
{
    if (m_singleton != nullptr)
      {
            m_singleton->m_nbLink--;
            if (m_singleton->m_nbLink==0) {
                close(m_singleton->m_fileI2c);
                delete m_singleton;
                m_singleton = nullptr;
            } // if mNbLink
      } // if null
} // freeInstance
