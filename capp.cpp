#include "capp.h"

CApp::CApp(QObject *parent) : QObject(parent)
{
    _zdc = new CZdc();
    _gthm = new QThread();
    _maquette = new CGestionMaquette(_zdc);
    _maquette->moveToThread(_gthm);
    connect(this, &CApp::sig_go, _maquette, &CGestionMaquette::on_go);
    connect(_gthm, &QThread::finished, _maquette, &QObject::deleteLater);
    emit sig_go();
}

CApp::~CApp()
{
    _gthm->quit();
    _gthm->wait();
    delete _zdc;
}


