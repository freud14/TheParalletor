/**
    @file   main.cpp
            Fichier contenant le main() du programme
    @author Fr�d�rik Paradis
    @author Louis-�tienne Dorval
    @date   16 mai 2011
*/


#include <QtGui/QApplication>
#include "mainwindowoperator.h"

#include <iostream>

#include "SocketSelection.h"

/**
  M�thode qui d�marre l'application et donc la fen�tre MainWindowOperator
  @param [in] argc Nombre d'argument
  @param [in] argv Tableau d'arguments
  @return Le retour du main()
  */
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    WSADATA WSAData;

    int erreur = WSAStartup(MAKEWORD(2,2), &WSAData);


    MainWindowOperator mainWindowOperator;

    mainWindowOperator.show();

    return app.exec();
    WSACleanup();
}
