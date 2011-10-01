/**
        @file	OperatorCommunication.h
                        Fichier contenant la déclaration de la classe OperatorCommunication
        @author Frédérik Paradis
        @author Louis-Étienne Dorval
        @date   16 mai 2011
*/

#ifndef OPERATORCOMMUNICATION_H
#define OPERATORCOMMUNICATION_H

#include "Socket.h"
#include "operator.h"

/**
  Cette classe ne contient que des méthodes statiques qui gère le protocole de communication entre
  le serveur et l'opérateur
*/
class OperatorCommunication
{
public:
    static void sendInitiate101(Socket* _socket);
    static void sendStop102(Socket* _socket);
    static void sendConfirm103(Socket* _socket);
    static void sendUpdate104(Socket* _socket);
    static void sendUpdate105(Socket* _socket);
    static void sendDisconnect106(Socket* _socket);

    static void receiveCODE201(bool &_isAccepted, short &_nbClient, Socket* _socket);
    static EntierLong receiveCODE203(Socket* _socket);
    static void receiveCODE204(Socket* _socket);
    static short receiveCODE205(Socket* _socket);
    static std::string receiveCODE206(Socket* _socket);
private:
    OperatorCommunication();
    ~OperatorCommunication();
};

#endif // OPERATORCOMMUNICATION_H
