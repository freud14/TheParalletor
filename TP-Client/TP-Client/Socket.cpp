/**
    @file   Socket.cpp
            Fichier contenant l'implémentation de la classe Socket.
    @author Frédérik Paradis
    @author Louis-Étienne Dorval
    @date   16 mai 2011
*/

#include "Socket.h"
#include <iostream>

using namespace std;

/**
    Constructeur d'un socket serveur
    @param [in] _serverPort Le numéro de port du socket serveur
*/
Socket::Socket(int _serverPort){
    //Taille de la structure
    recsize = sizeof(sin);

    sock = socket(AF_INET, SOCK_STREAM, 0); //Création du socket

    if(sock != INVALID_SOCKET)
    {
        sin.sin_addr.s_addr = htonl(INADDR_ANY);
        sin.sin_family = AF_INET;
        sin.sin_port = htons(_serverPort);
        sock_err = bind(sock, (SOCKADDR*)&sin, recsize);
    }
    else //Si la création a échoué
    {

    }
}

/**
    Constructeur d'un socket client
    @param [in] _ipAddress L'adresse IP de connection
    @param [in] _clientPort Le numéro de port du socket client
*/
Socket::Socket(char* _ipAddress, int _clientPort){
    //Taille de la structure
    recsize = sizeof(sin);

    sock = socket(AF_INET,SOCK_STREAM,0);//Création du socket

    if(sock != INVALID_SOCKET) //Si la création a échoué
    {
        sin.sin_addr.s_addr	= inet_addr(_ipAddress);
        sin.sin_family		= AF_INET;
        sin.sin_port		= htons(_clientPort);
    }
}

/**
    Constructeur privée sans paramètre
*/
Socket::Socket(){

}

/**
   Destructeur privée sans paramètre
*/
Socket::~Socket(){
    closesocket(sock);
}

/**
    Méthode qui permet au serveur d'écouter
    @return Si la méthode à réussi (TRUE) ou non (FALSE)
*/
bool Socket::listenServer(){
    assert(this->sin.sin_addr.s_addr == htonl(INADDR_ANY)); //Si le socket n'est pas un serveur

    bool retour;

    if(sock_err != SOCKET_ERROR)
    {
        listen(sock, 0);
        retour = true;
    }
    else //Si le bind() dans le constructeur a retourné une erreur
    {
        retour = false;
    }
    return retour;
}

/**
    Méthode qui permet au serveur d'accepter la connection d'un client
    @return Un pointeur vers la socket si la connection à réussi, sinon NULL
*/
Socket* Socket::acceptServer(){
    assert(this->sin.sin_addr.s_addr == htonl(INADDR_ANY)); //Si le socket n'est pas un serveur


    Socket* socketReturn = NULL; //Pointeur pour le retour de la fonction

    if(sock_err != SOCKET_ERROR)
    {
        //Structure pour le client qui s'y connectera
        SOCKET csock;
        SOCKADDR_IN csin;
        socklen_t crecsize = sizeof(csin);

        csock = accept(sock, (SOCKADDR *)&csin, &crecsize);

        socketReturn = new Socket();
        socketReturn->sin = csin;
        socketReturn->recsize = crecsize;
        socketReturn->sock = csock;
    }
    return socketReturn;
}

/**
    Cette méthode se charge d'établir la connexion avec le serveur. Seul un socket client peut s'en servir.
    @return Si la méthode à réussi (TRUE) ou non (FALSE)
*/
bool Socket::connectClient(){
    assert(this->sin.sin_addr.s_addr != htonl(INADDR_ANY)); //Si le socket n'est pas un serveur

    bool retour; //Variable de retour

    if(connect(sock, (SOCKADDR *)&sin, recsize) != SOCKET_ERROR)
    {
        retour = true;
    }
    else
    { //Si le connect() à retourner une erreur
        retour = false;
    }
    return retour;
}

/**
    Cette méthode se charge de transmettre un message. Un socket client ou serveur peut s'en servir.
    @param [in] _message Message à transmettre
    @param [in] length Longueur de la transmission
    @return Si la méthode à réussi (TRUE) ou non (FALSE)
*/
bool Socket::sending(void* _message, int length){
    bool retour;
    if(send(this->sock, (char*)_message, length, 0) != SOCKET_ERROR)
    {
        retour = true;
    }
    else //Si le send() retourne une erreur
    {
        retour = false;
    }
    return retour;
}

/**
    Cette méthode se charge de recevoir un message. Un socket client ou serveur peut s'en servir.
    @param [in] _message Message à recevoir
    @param [in] length Longueur de la réception
    @return Si la méthode à réussi (TRUE) ou non (FALSE)
*/
bool Socket::receiving(void* _message, int length){
    bool retour;
    int rv = recv(this->sock, (char*)_message, length, 0);
    if (rv != SOCKET_ERROR && rv != 0)
    {
        retour = true;
    }
    else //Si le recv()retourne une erreur
    {
	retour = false;
    }
    return retour;
}
