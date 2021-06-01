#include "cihm.h"
#include "ui_cihm.h"

CIhm::CIhm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CIhm)
{
    ui->setupUi(this);
    _app = new CApp(this);

    ui->teEclair->setText("Puissance d'éclairage\nJour/nuit\nPrésence de personne\n");
    ui->teInter->setText("Mode des voies\nCouleur de la voie 1\nCouleur de la voie 2");
    ui->tePark->setText("Action sur la barrière d'entrée\nAction sur la barrière de sortie");

    /*Connect des signaux de CApp vers CIhmI2c*/

    connect(_app, &CApp::sig_erreur, this, &CIhm::on_erreur);
    connect(_app, &CApp::sig_info, this, &CIhm::on_info);
    connect(_app, &CApp::sig_msgConsigne, this, &CIhm::on_sigMsgConsigne);
    connect(_app, &CApp::sig_msgCellule, this, &CIhm::on_sigMsgCellule);
    connect(_app, &CApp::sig_msgPresence, this, &CIhm::on_sigMsgPresence);
    connect(_app, &CApp::sig_msgInterOrdre1, this, &CIhm::on_sigMsginterOrdre1);
    connect(_app, &CApp::sig_msgInterOrdre2, this, &CIhm::on_sigMsginterOrdre2);
    connect(_app, &CApp::sig_msgMode, this, &CIhm::on_sigMsgMode);
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

void CIhm::on_sigMsgCellule(QString msg_cellule)
{
    const int lineToDelete = 1; // To delete the second line.
    QTextBlock b = ui->teEclair->document()->findBlockByLineNumber(lineToDelete);
    if (b.isValid()) {
        QTextCursor cursor(b);
        cursor.select(QTextCursor::BlockUnderCursor);
        cursor.removeSelectedText();
        cursor.insertText("\nJour/nuit :" + msg_cellule);
    }
}

void CIhm::on_sigMsgPresence(QString msg_presence)
{
    const int lineToDelete = 2; // To delete the second line.
    QTextBlock b = ui->teEclair->document()->findBlockByLineNumber(lineToDelete);
    if (b.isValid()) {
        QTextCursor cursor(b);
        cursor.select(QTextCursor::BlockUnderCursor);
        cursor.removeSelectedText();
        cursor.insertText("\nPrésence de personne :" + msg_presence);
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
        cursor.insertText("\nMode des voies :" + msg_mode);
    }
}

//Fin slots de signaux

/*SLOTS IHM*/

//Partie éclairage
void CIhm::on_cb_0_clicked(bool checked)
{
    if(checked == true){

        //_app->setConsigne(0); //appel de la fonction dans CApp

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

        //_app->setConsigne(1);

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

        //_app->setConsigne(2);

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

void CIhm::on_cb_jour_clicked(bool checked)
/*Ici est un cas particulier, c'est le jour donc pas de lumière -> pas de possibilité de pouvoir mettre de la lumière
Le 0% est forcé par défaut et ne peut être enlevé qu'à la condition que ça ne soit plus le jour*/
{
    if(checked == true){
        //_app->setCellule(true);

       ui->cb_0->setChecked(true);
       ui->cb_0->setEnabled(false);
       //_app->setConsigne(0);

       ui->cb_50->setEnabled(false);
       ui->cb_100->setEnabled(false);
       ui->cb_nuit->setEnabled(false);
       ui->cb_presence->setEnabled(false);
       ui->cb_absence->setEnabled(false);
    }else{
       ui->cb_0->setChecked(false);
       int lineToDelete = 0; // Pour supprimer la première ligne
       QTextBlock b = ui->teEclair->document()->findBlockByLineNumber(lineToDelete);
       if (b.isValid()) {
           QTextCursor cursor(b);
           cursor.select(QTextCursor::BlockUnderCursor);
           cursor.removeSelectedText();
           cursor.insertText("Puissance d'éclairage");
       }
       ui->cb_0->setEnabled(true);
       ui->cb_50->setEnabled(true);
       ui->cb_100->setEnabled(true);
       ui->cb_nuit->setEnabled(true);
       ui->cb_presence->setEnabled(true);
       ui->cb_absence->setEnabled(true);
       lineToDelete = 1; // Pour supprimer la deuxième ligne
       b = ui->teEclair->document()->findBlockByLineNumber(lineToDelete);
       if (b.isValid()) {
           QTextCursor cursor(b);
           cursor.select(QTextCursor::BlockUnderCursor);
           cursor.removeSelectedText();
           cursor.insertText("\nJour/nuit");
      }
   }
}

void CIhm::on_cb_nuit_clicked(bool checked)
{
    if(checked == true){
        //_app->setCellule(false);

       ui->cb_jour->setEnabled(false);
    }else{
       ui->cb_jour->setEnabled(true);
       const int lineToDelete = 1; // Pour supprimer la deuxième ligne
       QTextBlock b = ui->teEclair->document()->findBlockByLineNumber(lineToDelete);
       if (b.isValid()) {
           QTextCursor cursor(b);
           cursor.select(QTextCursor::BlockUnderCursor);
           cursor.removeSelectedText();
           cursor.insertText("\nJour/nuit");
      }
   }
}

void CIhm::on_cb_presence_clicked(bool checked)
{
    if(checked == true){
        //_app->setPresence(true);

       ui->cb_absence->setEnabled(false);
    }else{
       ui->cb_absence->setEnabled(true);
       const int lineToDelete = 2; // Pour supprimer la troisième ligne
       QTextBlock b = ui->teEclair->document()->findBlockByLineNumber(lineToDelete);
       if (b.isValid()) {
           QTextCursor cursor(b);
           cursor.select(QTextCursor::BlockUnderCursor);
           cursor.removeSelectedText();
           cursor.insertText("\nPrésence de personne");
      }
   }
}

void CIhm::on_cb_absence_clicked(bool checked)
{
    if(checked == true){
        //_app->setPresence(false);

       ui->cb_presence->setEnabled(false);
    }else{
       ui->cb_presence->setEnabled(true);
       const int lineToDelete = 2; // Pour supprimer la troisième ligne
       QTextBlock b = ui->teEclair->document()->findBlockByLineNumber(lineToDelete);
       if (b.isValid()) {
           QTextCursor cursor(b);
           cursor.select(QTextCursor::BlockUnderCursor);
           cursor.removeSelectedText();
           cursor.insertText("\nPrésence de personne");
        }
    }
}

//Fin partie Éclairage

//Partie intersection

void CIhm::on_cb_red_clicked(bool checked)
{
    if(checked == true){
        _app->setInterOrdre1(0);

       ui->cb_orange->setEnabled(false);
       ui->cb_green->setEnabled(false);
    }else{
       ui->cb_orange->setEnabled(true);
       ui->cb_green->setEnabled(true);
       const int lineToDelete = 1; // Pour supprimer la première ligne
       QTextBlock b = ui->teInter->document()->findBlockByLineNumber(lineToDelete);
       if (b.isValid()) {
           QTextCursor cursor(b);
           cursor.select(QTextCursor::BlockUnderCursor);
           cursor.removeSelectedText();
           cursor.insertText("\nCouleur de la voie 1");
        }
    }
}

void CIhm::on_cb_red2_clicked(bool checked)//Continuer la deuxième voie
{
    if(checked == true){
        _app->setInterOrdre2(0);

       ui->cb_orange2->setEnabled(false);
       ui->cb_green2->setEnabled(false);
    }else{
       ui->cb_orange2->setEnabled(true);
       ui->cb_green2->setEnabled(true);
       const int lineToDelete = 3; // Pour supprimer la dernière ligne
       QTextBlock b = ui->teInter->document()->findBlockByLineNumber(lineToDelete);
       if (b.isValid()) {
           QTextCursor cursor(b);
           cursor.select(QTextCursor::BlockUnderCursor);
           cursor.removeSelectedText();
           cursor.insertText("\nCouleur de la voie 2");
       }
    }
}


void CIhm::on_cb_orange_clicked(bool checked)
{
    if(checked == true){
        _app->setInterOrdre1(1);

       ui->cb_red->setEnabled(false);
       ui->cb_green->setEnabled(false);
    }else{
       ui->cb_red->setEnabled(true);
       ui->cb_green->setEnabled(true);
       const int lineToDelete = 1; // Pour supprimer la première ligne
       QTextBlock b = ui->teInter->document()->findBlockByLineNumber(lineToDelete);
       if (b.isValid()) {
           QTextCursor cursor(b);
           cursor.select(QTextCursor::BlockUnderCursor);
           cursor.removeSelectedText();
           cursor.insertText("\nCouleur de la voie 1");
        }
    }
}

void CIhm::on_cb_orange2_clicked(bool checked)
{
    if(checked == true){
        _app->setInterOrdre2(1);

       ui->cb_red2->setEnabled(false);
       ui->cb_green2->setEnabled(false);
    }else{
       ui->cb_red2->setEnabled(true);
       ui->cb_green2->setEnabled(true);
       const int lineToDelete = 3; // Pour supprimer la dernière ligne
       QTextBlock b = ui->teInter->document()->findBlockByLineNumber(lineToDelete);
       if (b.isValid()) {
           QTextCursor cursor(b);
           cursor.select(QTextCursor::BlockUnderCursor);
           cursor.removeSelectedText();
           cursor.insertText("\nCouleur de la voie 2");
       }
    }
}

void CIhm::on_cb_green_clicked(bool checked)
{
    if(checked == true){
        _app->setInterOrdre1(2);

       ui->cb_red->setEnabled(false);
       ui->cb_orange->setEnabled(false);
    }else{
       ui->cb_red->setEnabled(true);
       ui->cb_orange->setEnabled(true);
       const int lineToDelete = 1; // Pour supprimer la deuxième ligne
       QTextBlock b = ui->teInter->document()->findBlockByLineNumber(lineToDelete);
       if (b.isValid()) {
           QTextCursor cursor(b);
           cursor.select(QTextCursor::BlockUnderCursor);
           cursor.removeSelectedText();
           cursor.insertText("\nCouleur de la voie 1");
        }
    }
}

void CIhm::on_cb_green2_clicked(bool checked)
{
    if(checked == true){
        _app->setInterOrdre2(2);

       ui->cb_red2->setEnabled(false);
       ui->cb_orange2->setEnabled(false);
    }else{
       ui->cb_red2->setEnabled(true);
       ui->cb_orange2->setEnabled(true);
       const int lineToDelete = 0; // Pour supprimer la première ligne
       QTextBlock b = ui->teInter->document()->findBlockByLineNumber(lineToDelete);
       if (b.isValid()) {
           QTextCursor cursor(b);
           cursor.select(QTextCursor::BlockUnderCursor);
           cursor.removeSelectedText();
           cursor.insertText("\nCouleur de la voie 2");
       }
    }
}


void CIhm::on_cb_auto_clicked(bool checked)
{
    if(checked == true){
        _app->setMode(1);//définir le mode sur 01 : Auto

        //Empêche de cliquer sur les autres boutons (étant en automatique)
        ui->cb_manuel->setEnabled(false);
        ui->cb_cligno->setEnabled(false);
        ui->cb_red->setEnabled(false);
        ui->cb_red2->setEnabled(false);
        ui->cb_orange->setEnabled(false);
        ui->cb_orange2->setEnabled(false);
        ui->cb_green->setEnabled(false);
        ui->cb_green2->setEnabled(false);
    }else {
        ui->cb_manuel->setEnabled(true);
        ui->cb_cligno->setEnabled(true);
        ui->cb_red->setEnabled(true);
        ui->cb_red2->setEnabled(true);
        ui->cb_orange->setEnabled(true);
        ui->cb_orange2->setEnabled(true);
        ui->cb_green->setEnabled(true);
        ui->cb_green2->setEnabled(true);
        const int lineToDelete = 0; // Pour supprimer la première ligne
        QTextBlock b = ui->teInter->document()->findBlockByLineNumber(lineToDelete);
        if (b.isValid()) {
            QTextCursor cursor(b);
            cursor.select(QTextCursor::BlockUnderCursor);
            cursor.removeSelectedText();
            cursor.insertText("Mode des voies");
        }
    }
}

void CIhm::on_cb_cligno_clicked(bool checked)
{
    if(checked == true){
        _app->setMode(0);//définir le mode sur 00 : Orange Clignotant

        ui->cb_auto->setEnabled(false);
        ui->cb_manuel->setEnabled(false);
    }else {
        ui->cb_manuel->setEnabled(true);
        ui->cb_auto->setEnabled(true);
        const int lineToDelete = 0; // Pour supprimer la première ligne
        QTextBlock b = ui->teInter->document()->findBlockByLineNumber(lineToDelete);
        if (b.isValid()) {
            QTextCursor cursor(b);
            cursor.select(QTextCursor::BlockUnderCursor);
            cursor.removeSelectedText();
            cursor.insertText("\nCouleur de la voie 2");
        }
    }
}

void CIhm::on_cb_manuel_clicked(bool checked)
{

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


