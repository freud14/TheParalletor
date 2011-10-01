/**
        @file	mainWindowOperator.h
                        Fichier contenant la d�claration de la classe MainWindowOperator
        @author Fr�d�rik Paradis
        @author Louis-�tienne Dorval
        @date   16 mai 2011
*/

#ifndef MAINWINDOWOPERATOR_H
#define MAINWINDOWOPERATOR_H

#include <QtGui>

/**
    Cette classe h�rite de QWidget. C'est la fen�tre d'accueil du logiciel
 */
class MainWindowOperator : public QWidget
{   
    Q_OBJECT
public:
    MainWindowOperator();
private:
    bool checkIP();
    bool checkPort();
    bool checkHash();
    bool checkMinChar();
    bool checkMaxChar();
    bool checkQuota();

    QLineEdit* ip;
    QLineEdit* port;
    QLineEdit* range;
    QComboBox* hashtype;
    QLineEdit* hash;
    QLineEdit* minchar;
    QLineEdit* maxchar;
    QLineEdit* quota;
    QLineEdit* saltBefore;
    QLineEdit* saltAfter;
    QFormLayout* formLayout;
    QVBoxLayout* layoutPrincipal;
    QPushButton* boutonStart;

    QString TXT_IP;
    QString TXT_PORT;
    QString TXT_RANGE;
    QString TXT_HASHTYPE;
    QString TXT_HASH;
    QString TXT_MINCHAR;
    QString TXT_MAXCHAR;
    QString TXT_QUOTA;
    QString TXT_SALT_BEFORE;
    QString TXT_SALT_AFTER;
public slots:
    void startOperator();

signals:
};

#endif // MAINWINDOWOPERATOR_H
