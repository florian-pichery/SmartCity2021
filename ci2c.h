#ifndef CI2C_H
#define CI2C_H

#include <QObject>
#include <QMutex>
#include <QMutexLocker>
#include <QDebug>
#include <stdio.h>
#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port
#include <strings.h>

class CI2c : public QObject
{
    Q_OBJECT

public:
    // creation destruction de l'objet
    static CI2c *getInstance(QObject *parent = nullptr, char no = '1');
    static void freeInstance();
    int lire(unsigned char addr, unsigned char *buffer, int lg);
    int ecrire(unsigned char addr, unsigned char *buffer, int lg);

private:
    explicit CI2c(QObject *parent = nullptr, char noBus = '1');
    int init();
    QObject *m_parent;
    QMutex m_mutexI2c;
    int getNbLink();
    int m_addr;   // Adresse du composant I2C
    char m_noBus;   // No d'accès au fichier /dev
    int m_fileI2c;  // descripteur du fichier i2C
    int m_nbLink;
    static CI2c *m_singleton;

signals:
    void sigErreur(QString msg);
};

#endif // CI2C_H
