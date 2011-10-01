/**
        @file	progressionWindowOperator.h
                        Fichier contenant la déclaration de la classe ProgressionWindowOperator
        @author Frédérik Paradis
        @author Louis-Étienne Dorval
        @date   16 mai 2011
*/

#ifndef PROGRESSIONWINDOWOPERATOR_H
#define PROGRESSIONWINDOWOPERATOR_H

#include <QtGui>
#include "Socket.h"
#include "operator.h"
#include "BigInteger/BigIntegerLibrary.hh"


/**
  Cette classe hérite de QWidget, elle affiche la progression de la recherche
  */
class ProgressionWindowOperator : public QWidget
{
    Q_OBJECT
public:
    static ProgressionWindowOperator* getInstance();
    void updateNbClients(short _nbClients);
    void updateQuota(short _quota);
    void updateProgressBar(short _pourcent);
    void updateTotalCalculed(std::string _toAdd);
    void stopIsFound(std::string _password);
    void stopNotFound();
    void updateProb(char* _prob);
    void setProbValue(std::string _prob);
    short getPourcentage(BigInteger& grand, BigInteger& petit);
    void closeEvent(QCloseEvent *event);
public slots:
    void cancelSearch();
    void afficherPasswordFound(char* _str);
    void afficherNotFound();
    void updateTotal(char* _total);
    void updateProgression(char* _progression);

signals:
    void passwordFound(char* _password);
    void notFound();
    void totalProb(char* _total);
    void progressionSignal(char* _progression);

private:
    QLabel* nbClients;
    QLabel* quota;
    QLabel* total;
    QLabel* progression;
    QProgressBar* progressBar;
    QPushButton* cancel;
    QString TXT_NBCLIENTS;
    QString TXT_QUOTA;
    QString TXT_TOTAL;
    QString TXT_PROGRESSION;
    QFormLayout* formLayout;
    QVBoxLayout* layoutPrincipal;

    static ProgressionWindowOperator* instance;
    ProgressionWindowOperator();
    BigInteger totalProgress;
    BigInteger totalBigInteger;
};

#endif // PROGRESSIONWINDOWOPERATOR_H
