/**
    @file   SocketSelection.cpp
            Fichier contenant l'impl�mentation de la classe SocketSelection.
    @author Fr�d�rik Paradis
    @author Louis-�tienne Dorval
    @date   16 mai 2011
*/

#include "SocketSelection.h"

#include <iomanip>
#include <assert.h>
#include <vector>

using namespace std;

/**
    Constructeur par d�faut sans param�tre
*/
SocketSelection::SocketSelection()
{

}

/**
    Desctructeur de la classe
*/
SocketSelection::~SocketSelection()
{

}

/**
    Permet d'ajouter un socket � la SocketSelection
    @param [in] _socket Socket � ajouter
    @return Si la m�thode � r�ussi (TRUE) ou non (FALSE)
*/
bool SocketSelection::addSocket(Socket* _socket)
{
    bool retour = true;
    assert(_socket != NULL);

    for(vector<Socket*>::iterator it = vecSocket.begin(); it != vecSocket.end(); ++it)
    {
        if((*it) == _socket) //Si le socket est d�j� pr�sent
        {
            retour = false;
        }
    }

    if (retour != false)
    {
        this->vecSocket.push_back(_socket);
    }

    return retour;
}

/**
    Permet d'enlever un socket � la SocketSelection
    @param [in] _socket Socket � ajouter
    @return Si la m�thode � r�ussi (TRUE) ou non (FALSE)
*/
bool SocketSelection::removeSocket(Socket* _socket)
{
    bool retour = false;
    vector<Socket*>::iterator itErase;
    assert(_socket != NULL);

    for(vector<Socket*>::iterator it = vecSocket.begin(); it != vecSocket.end(); ++it)
    {
        if(*it == _socket)
        {
            retour = true;
            itErase = it;
        }
    }

    if (retour == true)
    {
        this->vecSocket.erase(itErase);
    }
    return retour;
}

/**
    Permet d'effectuer la s�lection de socket, donc de parcourir tous les sockets pour
    voir lesquels doivent �tre lues
    @param [in] _msTimeOut le d�lais durant lequel la fonction sera bloquante
    @return Un vector contenant toutes les sockets � lire
*/
vector<Socket*> SocketSelection::selection(int* _msTimeOut)
{

    vector<Socket*> retour;
    if (vecSocket.size() != 0)
    {
        FD_ZERO(&readfs);
        int n = vecSocket.front()->sock;
        int rv;
        struct timeval* tv;


        for(vector<Socket*>::iterator it = vecSocket.begin(); it != vecSocket.end(); ++it)
        {
            FD_SET((*it)->sock, &readfs);
            if (n < (*it)->sock)
            {
                n = (*it)->sock;
            }
        }

        n += 1; //Il faut que se soit le socket le plus grand plus 1

        if(_msTimeOut != NULL)
        {
            tv = new timeval;
            tv->tv_sec = *_msTimeOut/1000;
            tv->tv_usec = (*_msTimeOut%1000)*1000; //1 seconde = 1 000 000 microsecondes
        }
        else
        {
            tv = NULL;
        }

        rv = select(n, &readfs, NULL, NULL, tv);

        if (rv != -1 && rv != 0) //Aucune erreur et le timeout n'a pas expir�
        {
            for(vector<Socket*>::iterator it = vecSocket.begin(); it != vecSocket.end(); ++it)
            {
                if (FD_ISSET((*it)->sock, &readfs))
                {
                    retour.push_back(*it);
                }
            }
        }
        delete tv;
    }

    return retour;
}



