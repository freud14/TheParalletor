/**
    @file   mainWindowOperator.cpp
            Fichier contenant l'impl�mentation de la classe mainWindowOperator
    @author Fr�d�rik Paradis
    @author Louis-�tienne Dorval
    @date   16 mai 2011
*/


#include "mainwindowoperator.h"
#include "operator.h"
#include "ProgressionWindowOperator.h"
#include "invalidsocket.h"
#include <QRegExp>

/**
   Constructeur de la classe mainWindowOperator.
   Elle ajoute tous les �l�ments de la page et ajuste ses divers param�tres
  */
MainWindowOperator::MainWindowOperator()
{
    this->TXT_IP = "Adresse IP du serveur:";
    this->TXT_PORT = "Port du serveur:";
    this->TXT_RANGE = "�tendu des caract�res:";
    this->TXT_HASHTYPE   = "Type de hash:";
    this->TXT_HASH = "Hash:";
    this->TXT_MINCHAR = "Caract�re minimum:";
    this->TXT_MAXCHAR = "Caract�re maximum:";
    this->TXT_QUOTA = "Quota par client: \r\n (en million)";
    this->TXT_SALT_BEFORE = "Salt: \r\n (avant)";
    this->TXT_SALT_AFTER = "Salt: \r\n (apr�s)";

    ip = new QLineEdit;
    ip->setFixedWidth(275);
    ip->setText("127.0.0.1");

    port = new QLineEdit;
    port->setFixedWidth(50);
    port->setText("1337");

    range = new QLineEdit;
    range->setFixedWidth(400);
    range->setText("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");

    hashtype = new QComboBox;
    hashtype->addItem("MD5");
    hashtype->addItem("SHA1");
    hashtype->setFixedWidth(100);

    hash = new QLineEdit;
    hash->setText("");
    hash->setFixedWidth(275);

    minchar = new QLineEdit;
    minchar->setFixedWidth(50);
    minchar->setText("1");

    maxchar = new QLineEdit;
    maxchar->setFixedWidth(50);
    maxchar->setText("8");

    quota = new QLineEdit;
    quota->setFixedWidth(50);
    quota->setText("10");

    saltBefore = new QLineEdit;
    saltBefore->setFixedWidth(100);

    saltAfter = new QLineEdit;
    saltAfter->setFixedWidth(100);

    formLayout = new QFormLayout;
    formLayout->setVerticalSpacing(20);
    formLayout->addRow(TXT_IP, ip);
    formLayout->addRow(TXT_PORT, port);
    formLayout->addRow(TXT_RANGE, range);
    formLayout->addRow(TXT_HASHTYPE, hashtype);
    formLayout->addRow(TXT_HASH, hash);
    formLayout->addRow(TXT_MINCHAR, minchar);
    formLayout->addRow(TXT_MAXCHAR, maxchar);
    formLayout->addRow(TXT_QUOTA, quota);
    formLayout->addRow(TXT_SALT_BEFORE, saltBefore);
    formLayout->addRow(TXT_SALT_AFTER, saltAfter);

    layoutPrincipal = new QVBoxLayout;
    layoutPrincipal->setSpacing(25);
    layoutPrincipal->addLayout(formLayout);


    boutonStart = new QPushButton("D�marrer");
    boutonStart->setFixedWidth(75);
    layoutPrincipal->addWidget(boutonStart); // Ajout du bouton


    setLayout(layoutPrincipal);
    setWindowTitle("The Paralletor - Recherche exhaustive");

    connect(boutonStart, SIGNAL(clicked()), this, SLOT(startOperator()));
}

/**
   M�thode qui d�marre la classe Op�rateur, donc qui d�marre la recherche exhaustive
   et qui ouvre ainsi la fen�tre ProgressionWindowOperator
*/
void MainWindowOperator::startOperator()
{
    if (checkIP() && checkPort() && checkHash() && checkMinChar() && checkMaxChar() && checkQuota()) //Si les donn�es entr�es sont valides
    {
        try
        {
            Operator::createInstance(ip->text(), port->text().toShort(), range->text(),
                                     (short)hashtype->currentIndex(), hash->text(),
                                     minchar->text().toShort(), maxchar->text().toShort(), quota->text().toShort(),
                                     saltBefore->text(), saltAfter->text());
            this->hide();
            ProgressionWindowOperator::getInstance()->show();
            Operator::getInstance()->start();
        }
        catch(InvalidSocket& e)
        {
            QMessageBox::information(this, "Erreur", e.what());
        }
    }
}

/**
    M�thode qui v�rifie l'adresse IP avec une expression r�guli�re
    @return Retourne si l'adresse IP est valide (TRUE) ou non (FALSE)
*/
bool MainWindowOperator::checkIP()
{
    bool retour = true;
    QString ip = this->ip->text();
    QRegExp rx("(\[0-9]{1,3})\.(\[0-9]{1,3})\.(\[0-9]{1,3})\.(\[0-9]{1,3})"); //Adresse IP ou non
    QRegExpValidator v(rx, 0);
    int pos = 0;
    QValidator::State result = v.validate(ip, pos);
    if (result == QValidator::Invalid)
    {
        retour = false;
        QMessageBox::information(this, "Adresse IP non valide", "Veuillez v�rifier l'adresse IP");
    }
    return retour;
}

/**
    M�thode qui v�rifie le port avec une expression r�guli�re
    @return Retourne si le port est valide (TRUE) ou non (FALSE)
*/
bool MainWindowOperator::checkPort()
{
    bool retour = true;
    QString port = this->port->text();
    QRegExp rx("(\[0-9]{1,5})");  //Seulement des chiffres avec une longueur de 5
    QRegExpValidator v(rx, 0);
    int pos = 0;
    QValidator::State result = v.validate(port, pos);
    if (result == QValidator::Invalid)
    {
        retour = false;
        QMessageBox::information(this, "Port non valide", "Veuillez v�rifier le port");
    }
    return retour;
}

/**
    M�thode qui v�rifie le hash avec une expression r�guli�re
    @return Retourne si le hash est valide (TRUE) ou non (FALSE)
*/
bool MainWindowOperator::checkHash()
{
    bool retour = true;
    QString hash = this->hash->text();
    QRegExp rx;
    if(this->hashtype->currentIndex() == 0)
    {
        rx.setPattern("(^[0-9a-f]{32})"); //Seulement des chiffres ou des lettres entre A-F avec une longueur de 32
    }
    else if (this->hashtype->currentIndex() == 1)
    {
       rx.setPattern("(^[0-9a-f]{40})");  //Seulement des chiffres ou des lettres entre A-F avec une longueur de 40
    }
    QRegExpValidator v(rx, 0);
    int pos = 0;
    QValidator::State result = v.validate(hash, pos);
    if (result == QValidator::Invalid)
    {
        retour = false;
        QMessageBox::information(this, "Hash non valide", "Veuillez v�rifier le hash");
    }
    return retour;
}

/**
    M�thode qui v�rifie le nombre de charact�re minimum avec une expression r�guli�re
    @return Retourne si le nombre de charact�re minimum est valide (TRUE) ou non (FALSE)
*/
bool MainWindowOperator::checkMinChar()
{
    bool retour = true;
    QString minChar = this->minchar->text();
    QRegExp rx("(\[0-9]{1,2})");  //Seulement des chiffres avec une longueur maximum de 2
    QRegExpValidator v(rx, 0);
    int pos = 0;
    QValidator::State result = v.validate(minChar, pos);
    if (result == QValidator::Invalid)
    {
        retour = false;
        QMessageBox::information(this, "Nombre de charact�re minimum non valide", "Veuillez v�rifier le nombre de charact�re minimum");
    }
    return retour;
}

/**
    M�thode qui v�rifie le nombre de charact�re maximum avec une expression r�guli�re
    @return Retourne si le nombre de charact�re maximum est valide (TRUE) ou non (FALSE)
*/
bool MainWindowOperator::checkMaxChar()
{
    bool retour = true;
    QString maxChar = this->maxchar->text();
    QRegExp rx("(\[0-9]{1,2})");  //Seulement des chiffres avec une longueur maximum de 2
    QRegExpValidator v(rx, 0);
    int pos = 0;
    QValidator::State result = v.validate(maxChar, pos);
    if (result == QValidator::Invalid)
    {
        retour = false;
        QMessageBox::information(this, "Nombre de charact�re maximum non valide", "Veuillez v�rifier le nombre de charact�re maximum");
    }
    return retour;
}

/**
    M�thode qui v�rifie le quota avec une expression r�guli�re
    @return Retourne si le quota est valide (TRUE) ou non (FALSE)
*/
bool MainWindowOperator::checkQuota()
{
    bool retour = true;
    QString quota = this->quota->text();
    QRegExp rx("(\[0-9]{1,9})"); //Seulement des chiffres avec une longueur maximum de 9
    QRegExpValidator v(rx, 0);
    int pos = 0;
    QValidator::State result = v.validate(quota, pos);
    if (result == QValidator::Invalid)
    {
        retour = false;
        QMessageBox::information(this, "Quota non valide", "Veuillez v�rifier le quota");
    }
    return retour;
}
