#include "cihm.h"
#include "ui_cihm.h"

CIhm::CIhm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CIhm)
{
    ui->setupUi(this);
    _app = new CApp(this);

    ui->teEclair->setText("Puissance d'éclairage");
    ui->teInter->setText("Mode des voies\nCouleur de la voie 1\nCouleur de la voie 2");
    ui->tePark->setText("Action sur la barrière d'entrée\nAction sur la barrière de sortie");

    int cpt = _app->getCpt();

    ui->lcdNumber->display(cpt);

    /*Connect des signaux de CApp vers CIhm*/

    connect(_app, &CApp::sig_erreur, this, &CIhm::on_erreur);
    connect(_app, &CApp::sig_info, this, &CIhm::on_info);
    connect(_app, &CApp::sig_msgConsigne, this, &CIhm::on_sigMsgConsigne);
    connect(_app, &CApp::sig_msgInterOrdre1, this, &CIhm::on_sigMsginterOrdre1);
    connect(_app, &CApp::sig_msgInterOrdre2, this, &CIhm::on_sigMsginterOrdre2);
    connect(_app, &CApp::sig_msgMode, this, &CIhm::on_sigMsgMode);
    connect(_app, &CApp::sig_msgParkOrdreE, this, &CIhm::on_sigMsgParkOrdreE);
    connect(_app, &CApp::sig_msgParkOrdreS, this, &CIhm::on_sigMsgParkOrdreS);
}

CIhm::~CIhm()
{
    delete _app;
    delete ui;
}

void CIhm::on_leAddrEclair_returnPressed()
{
    if(ui->leAddrEclair->text().isEmpty()){//Renvoie la valeur de l'adresse par défaut à CZdc si lineEdit vide
        _app->setAddrEclair(20);
    }else {
        _app->setAddrEclair(static_cast<uint8_t>(ui->leAddrEclair->text().toUInt(nullptr, 10)));
    }
}
void CIhm::on_leAddrInter_returnPressed()
{
    if(ui->leAddrInter->text().isEmpty()){
        _app->setAddrInter(10);
    }else {
        _app->setAddrInter(static_cast<uint8_t>(ui->leAddrInter->text().toUInt(nullptr, 10)));
    }
}

void CIhm::on_leAddrPark_returnPressed()
{
    if(ui->leAddrPark->text().isEmpty()){
        _app->setAddrPark(53);
    }else {
        _app->setAddrPark(static_cast<uint8_t>(ui->leAddrPark->text().toUInt(nullptr, 10)));
    }
}

void CIhm::on_sigMsgConsigne(QString msg_consigne)
{
    const int lineToDelete = 0; // To delete the first line.
    QTextBlock b = ui->teEclair->document()->findBlockByLineNumber(lineToDelete);
    if (b.isValid()) {
        QTextCursor cursor(b);
        cursor.select(QTextCursor::BlockUnderCursor);
        cursor.removeSelectedText();
        cursor.insertText("Puissance d'éclairage :" + msg_consigne);
    }
}

void CIhm::on_sigMsginterOrdre1(QString msg_interOrdre1)
{
    const int lineToDelete = 1; // To delete the second line.
    QTextBlock b = ui->teInter->document()->findBlockByLineNumber(lineToDelete);
    if (b.isValid()) {
        QTextCursor cursor(b);
        cursor.select(QTextCursor::BlockUnderCursor);
        cursor.removeSelectedText();
        cursor.insertText("\nCouleur de la voie 1 :" + msg_interOrdre1);
    }
}

void CIhm::on_sigMsginterOrdre2(QString msg_interOrdre2)
{
    const int lineToDelete = 2; // To delete the third line.
    QTextBlock b = ui->teInter->document()->findBlockByLineNumber(lineToDelete);
    if (b.isValid()) {
        QTextCursor cursor(b);
        cursor.select(QTextCursor::BlockUnderCursor);
        cursor.removeSelectedText();
        cursor.insertText("\nCouleur de la voie 2 :" + msg_interOrdre2);
    }
}

void CIhm::on_sigMsgMode(QString msg_mode)
{
    const int lineToDelete = 0; // To delete the first line.
    QTextBlock b = ui->teInter->document()->findBlockByLineNumber(lineToDelete);
    if (b.isValid()) {
        QTextCursor cursor(b);
        cursor.select(QTextCursor::BlockUnderCursor);
        cursor.removeSelectedText();
        cursor.insertText("Mode des voies :" + msg_mode);
    }
}

void CIhm::on_sigMsgParkOrdreE(QString msg_parkOrdre)
{
    const int lineToDelete = 0; // To delete the first line.
    QTextBlock b = ui->tePark->document()->findBlockByLineNumber(lineToDelete);
    if (b.isValid()) {
        QTextCursor cursor(b);
        cursor.select(QTextCursor::BlockUnderCursor);
        cursor.removeSelectedText();
        cursor.insertText("Action sur la barrière d'entrée :" + msg_parkOrdre);
    }
}

void CIhm::on_sigMsgParkOrdreS(QString msg_parkOrdre)
{
    const int lineToDelete = 1; // To delete the first line.
    QTextBlock b = ui->tePark->document()->findBlockByLineNumber(lineToDelete);
    if (b.isValid()) {
        QTextCursor cursor(b);
        cursor.select(QTextCursor::BlockUnderCursor);
        cursor.removeSelectedText();
        cursor.insertText("\nAction sur la barrière de sortie :" + msg_parkOrdre);
    }
}

//Fin slots de signaux

/*SLOTS IHM*/

//Partie éclairage
void CIhm::on_cb_0_clicked(bool checked)
{
    if(checked == true){

        _app->setConsigne(0); //appel de la fonction dans CApp

        ui->cb_50->setEnabled(false);//Quand cb_0 coché, cb_50 pas cochable.
        ui->cb_100->setEnabled(false);//La même chose pour cb_100.
    }else{
        ui->cb_50->setEnabled(true);//Quand décoché, rend cochable les autres
        ui->cb_100->setEnabled(true);
        const int lineToDelete = 0; // Pour supprimer la première ligne
        QTextBlock b = ui->teEclair->document()->findBlockByLineNumber(lineToDelete);
        if (b.isValid()) {
            QTextCursor cursor(b);
            cursor.select(QTextCursor::BlockUnderCursor);
            cursor.removeSelectedText();
            cursor.insertText("Puissance d'éclairage");
        }
    }
}

void CIhm::on_cb_50_clicked(bool checked)
{
    if(checked == true){

        _app->setConsigne(1);

        ui->cb_0->setEnabled(false);
        ui->cb_100->setEnabled(false);
    }else{
        ui->cb_0->setEnabled(true);
        ui->cb_100->setEnabled(true);
        const int lineToDelete = 0; // Pour supprimer la première ligne
        QTextBlock b = ui->teEclair->document()->findBlockByLineNumber(lineToDelete);
        if (b.isValid()) {
            QTextCursor cursor(b);
            cursor.select(QTextCursor::BlockUnderCursor);
            cursor.removeSelectedText();
            cursor.insertText("Puissance d'éclairage");
        }
    }
}


void CIhm::on_cb_100_clicked(bool checked)
{
    if(checked == true){

        _app->setConsigne(2);

        ui->cb_0->setEnabled(false);
        ui->cb_50->setEnabled(false);
    }else{
        ui->cb_0->setEnabled(true);
        ui->cb_50->setEnabled(true);
        const int lineToDelete = 0; // Pour supprimer la première ligne
        QTextBlock b = ui->teEclair->document()->findBlockByLineNumber(lineToDelete);
        if (b.isValid()) {
            QTextCursor cursor(b);
            cursor.select(QTextCursor::BlockUnderCursor);
            cursor.removeSelectedText();
            cursor.insertText("Puissance d'éclairage");
        }
    }
}
//Fin partie Éclairage

//Partie intersection
void CIhm::on_rbCligno_clicked(bool checked)
{
    if(checked == true){
        _app->setMode(0);//définir le mode sur 00 : Orange Clignotant
    }
}

void CIhm::on_rbAuto_clicked(bool checked)
{
    if(checked == true){
        _app->setMode(1);//définir le mode sur 01 : Auto
    }
}
void CIhm::on_rbManu_clicked(bool checked)
{
    if(checked == true){
        _app->setMode(2);
    }
}

void CIhm::on_comboBoxVoie1_currentIndexChanged(int index)
{
    switch(index){
    case 0 : _app->setInterOrdre1(0+ACK); break;
    case 1 : _app->setInterOrdre1(1+ACK); break;
    case 2 : _app->setInterOrdre1(2+ACK); break;
    case 3 : _app->setInterOrdre1(3+ACK); break;
    }
}

void CIhm::on_comboBoxVoie2_currentIndexChanged(int index)
{
    switch(index){
    case 0 : _app->setInterOrdre2(0+ACK); break;
    case 1 : _app->setInterOrdre2(1+ACK); break;
    case 2 : _app->setInterOrdre2(2+ACK); break;
    case 3 : _app->setInterOrdre2(3+ACK); break;
    }
}

//Fin partie Intersection

//Partie Parking

void CIhm::on_cbMontEnt_clicked(bool checked)
{
    if(checked == true){
        _app->setParkOrdre(1);

        ui->cbDescEnt->setEnabled(false);
    }else {
        ui->cbDescEnt->setEnabled(true);
        const int lineToDelete = 0; // Pour supprimer la première ligne
        QTextBlock b = ui->tePark->document()->findBlockByLineNumber(lineToDelete);
        if (b.isValid()) {
            QTextCursor cursor(b);
            cursor.select(QTextCursor::BlockUnderCursor);
            cursor.removeSelectedText();
            cursor.insertText("Action sur la barrière d'entrée");
        }
    }
}

void CIhm::on_cbDescEnt_clicked(bool checked)
{
    if(checked == true){
        _app->setParkOrdre(2);

        ui->cbMontEnt->setEnabled(false);
    }else {
        ui->cbMontEnt->setEnabled(true);
        const int lineToDelete = 0; // Pour supprimer la première ligne
        QTextBlock b = ui->tePark->document()->findBlockByLineNumber(lineToDelete);
        if (b.isValid()) {
            QTextCursor cursor(b);
            cursor.select(QTextCursor::BlockUnderCursor);
            cursor.removeSelectedText();
            cursor.insertText("Action sur la barrière d'entrée");
        }
    }
}

void CIhm::on_cbMontSor_clicked(bool checked)
{
    if(checked == true){
        _app->setParkOrdre(4);

        ui->cbDescSor->setEnabled(false);
    }else {
        ui->cbDescSor->setEnabled(true);
        const int lineToDelete = 1; // Pour supprimer la première ligne
        QTextBlock b = ui->tePark->document()->findBlockByLineNumber(lineToDelete);
        if (b.isValid()) {
            QTextCursor cursor(b);
            cursor.select(QTextCursor::BlockUnderCursor);
            cursor.removeSelectedText();
            cursor.insertText("\nAction sur la barrière de sortie");
        }
    }
}

void CIhm::on_cbDescSor_clicked(bool checked)
{
    if(checked == true){
        _app->setParkOrdre(8);

        ui->cbMontSor->setEnabled(false);
    }else {
        ui->cbMontSor->setEnabled(true);
        const int lineToDelete = 1; // Pour supprimer la première ligne
        QTextBlock b = ui->tePark->document()->findBlockByLineNumber(lineToDelete);
        if (b.isValid()) {
            QTextCursor cursor(b);
            cursor.select(QTextCursor::BlockUnderCursor);
            cursor.removeSelectedText();
            cursor.insertText("\nAction sur la barrière de sortie");
        }
    }
}

void CIhm::on_leSup_returnPressed()
{
    if(ui->leSup->text().isEmpty()){
        _app->setLigneSup("Ligne superieure");
    }else {
        _app->setLigneSup(ui->leSup->text());
    }
}

void CIhm::on_leInf_returnPressed()
{
    if(ui->leInf->text().isEmpty()){
        _app->setLigneInf("Ligne inferieure");
    }else {
        _app->setLigneInf(ui->leInf->text());
    }
}

void CIhm::on_pbPlus_clicked()
{
    _app->setCptPlus();
    int cpt = _app->getCpt();
    ui->lcdNumber->display(cpt);
}

void CIhm::on_pbMoins_clicked()
{
    _app->setCptMoins();
    int cpt = _app->getCpt();
    ui->lcdNumber->display(cpt);
}

void CIhm::on_erreur(QString mess)
{
    _compteur++;
    ui->teErreurs->append("["+QString::number(_compteur)+"] "+mess);
    qDebug()<<mess;
}

void CIhm::on_info(QString mess)
{
    _compteur++;
    ui->teSuivi->append("["+QString::number(_compteur)+"] "+mess);
    qDebug()<<mess;
}
