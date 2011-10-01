/**
    @file   progressionwindowoperator.cpp
            Fichier contenant l'implémentation de la classe progressionwindowoperator
    @author Frédérik Paradis
    @author Louis-Étienne Dorval
    @date   16 mai 2011
*/

#include "progressionwindowoperator.h"
#include <iostream>
#include <sstream>
#include <string>


ProgressionWindowOperator* ProgressionWindowOperator::instance = NULL;

/**
  Constructeur de la classe ProgressionWindowOperator.
  Méthode qui place tous les objets
  */
ProgressionWindowOperator::ProgressionWindowOperator()
{
    this->TXT_NBCLIENTS = "Nombre de clients connectés";
    this->TXT_QUOTA = "Quota par client \r\n (en million)";
    this->TXT_TOTAL = "Nombre total de possibilités";
    this->TXT_PROGRESSION = "Progression";

    nbClients = new QLabel;
    nbClients->setFixedWidth(50);
    quota = new QLabel;
    quota->setFixedWidth(100);
    total  = new QLabel;
    total->setFixedWidth(350);
    progression  = new QLabel;
    progression->setFixedWidth(350);
    progression->setText("0");
    this->totalProgress = 0;
    progressBar = new QProgressBar;

    formLayout = new QFormLayout;
    formLayout->setVerticalSpacing(20);
    formLayout->addRow(TXT_NBCLIENTS, nbClients);
    formLayout->addRow(TXT_QUOTA, quota);
    formLayout->addRow(TXT_TOTAL, total);
    formLayout->addRow(TXT_PROGRESSION, progression);
    formLayout->addRow(progressBar);

    layoutPrincipal = new QVBoxLayout;
    layoutPrincipal->setSpacing(25);
    layoutPrincipal->addLayout(formLayout);

    cancel = new QPushButton("Annuler");
    cancel->setFixedWidth(75);
    layoutPrincipal->addWidget(cancel); // Ajout du bouton

    setLayout(layoutPrincipal);
    setWindowTitle("The Paralletor - Progression");

    connect(cancel, SIGNAL(clicked()), this, SLOT(cancelSearch()));
    connect(this, SIGNAL(passwordFound(char*)), this, SLOT(afficherPasswordFound(char*)));
    connect(this, SIGNAL(totalProb(char*)), this, SLOT(updateTotal(char*)));
    connect(this, SIGNAL(progressionSignal(char*)), this, SLOT(updateProgression(char*)));
    connect(this, SIGNAL(notFound()), this, SLOT(afficherNotFound()));
}

/**
  Redéfinition de l'évenement close, qui permet d'effectuer des actions lorsque le X de la fenêtre est sélectionné
  */
void ProgressionWindowOperator::closeEvent(QCloseEvent *event)
{
    Operator::getInstance()->askCancel();
    Operator::getInstance()->waitForThread();
    event->accept();
}

/**
  Méthode qui retourne l'instance de la classe -> Singleton
  @return L'instance de la classe
*/
ProgressionWindowOperator* ProgressionWindowOperator::getInstance()
{
    if (ProgressionWindowOperator::instance == NULL)
    {
        ProgressionWindowOperator::instance = new ProgressionWindowOperator();
    }
    return ProgressionWindowOperator::instance;
}

/**
  Méthode qui mets à jour le nombre de clients dans son QLabel
  @param [in] _nbClients Le nombre de clients
  */
void ProgressionWindowOperator::updateNbClients(short _nbClients)
{
    std::string retour;
    std::stringstream ss;
    ss << _nbClients;
    ss >> retour;
    this->nbClients->setText(retour.c_str());
}

/**
  Méthode qui mets à jour le quota par client dans son QLabel
  @param [in] _quota Le quota
  */
void ProgressionWindowOperator::updateQuota(short _quota)
{
    std::string retour;
    std::stringstream ss;
    ss << _quota;
    ss >> retour;
    this->quota->setText(retour.c_str());
}

/**
  Méthode qui mets à jour le nombre calculé depuis le début
  @param [in] _toAdd Le nombre depuis la dernière fois
  */
void ProgressionWindowOperator::updateTotalCalculed(std::string _toAdd)
{
    BigInteger toAdd;
    toAdd = stringToBigInteger(_toAdd.c_str());
    totalProgress += toAdd;
    std::string retour = bigIntegerToString(totalProgress);

    for (int i = retour.length(); i > 0 ; i -= 3)
    {
        retour.insert(i, " ");
    }
    emit progressionSignal((char*)retour.c_str());
}

/**
  Méthode qui mets à jour la progression
  @param [in] _progression La progression
  */
void ProgressionWindowOperator::updateProgression(char* _progression)
{
    this->progression->setText(_progression);
    this->progressBar->setValue(getPourcentage(totalBigInteger, totalProgress));
}

/**
  Méthode qui émet le signal pour l'update des probabilités
  @param [in] _prob Probabilités
  */
void ProgressionWindowOperator::updateProb(char* _prob)
{
    std::string value = _prob;
    setProbValue(value);
    emit updateTotal(_prob);
}

/**
  Méthode qui mets à jour la progressBar
  @param [in] _pourcent Le pourcentage de progression
  */
void ProgressionWindowOperator::updateProgressBar(short _pourcent)
{
    this->progressBar->setValue(_pourcent);
}

/**
  Méthode qui émet le signal pour avertir que le mot de passe est trouvé
  @param [in] _password Le mot de passe
  */
void ProgressionWindowOperator::stopIsFound(std::string _password)
{
    emit passwordFound((char*)_password.c_str());
    //QMessageBox::information(this, "Mot de passe trouvé!", _password.c_str());
}

/**
  Méthode qui émet le signal pour avertir que le mot de passe n'a pas été trouvé
  */
void ProgressionWindowOperator::stopNotFound()
{
    emit notFound();
    //QMessageBox::information(this, "Mot de passe trouvé!", _password.c_str());
}

/**
  SLOT qui affiche le mot de passe qui a été trouvé
  @param [in] _str Le mot de passe
  */
void ProgressionWindowOperator::afficherPasswordFound(char* _str)
{
    QMessageBox::information(this, "Mot de passe", _str);
    close();
}

/**
  SLOT qui affiche le total dans le QLabel
  @param [in] _total Le total
  */
void ProgressionWindowOperator::updateTotal(char* _total)
{
    this->total->setText(_total);
}

/**
  SLOT qui affiche que le mot de passe n'a pas été trouvé
  @param [in] _str Le mot de passe
  */
void ProgressionWindowOperator::afficherNotFound()
{
    QMessageBox::information(this, "Mot de passe non trouvé!", "Le mot de passe n'a pas été trouvé, veuillez entrer de nouveau paramètre");
    close();
}

/**
  SLOT qui annule la recherche
  */
void ProgressionWindowOperator::cancelSearch()
{
    Operator::getInstance()->askCancel();
    close();
}

/**
  Méthode pour calculer le pourcentage entre deux BigInteger
  @param [in] grand Le plus grand des deux chiffres
  @param [in] petit Le plus petit des deux chiffres
  @return Le pourcentage
  */
short ProgressionWindowOperator::getPourcentage(BigInteger& grand, BigInteger& petit)
{
    BigInteger resultat = (BigInteger(100) * petit) / grand;
    return resultat.toShort();
}

/**
  Méthode pour stocké les probabilités totales
  @param [in] _prob Probabilités
  */
void ProgressionWindowOperator::setProbValue(std::string _prob)
{
    QString total = _prob.c_str();
    total = total.remove(QChar(' '), Qt::CaseInsensitive);
    this->totalBigInteger = stringToBigInteger(total.toStdString());
}
