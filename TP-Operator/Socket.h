/**
        @file	Socket.h
                        Fichier contenant la déclaration de la classe Socket
        @author Frédérik Paradis
        @author Louis-Étienne Dorval
        @date   16 mai 2011
*/

#ifndef SOCKET_H_INCLUDED
#define SOCKET_H_INCLUDED

#include <winsock2.h>
#include <Windows.h>
#include <assert.h>

typedef int socklen_t;


/**
    Cette classe encapsule les sockets en C pour une utilisation plus simple
*/
class Socket
{
public:
	//Fonctions serveur seulement
	Socket(int _serverPort);
	bool listenServer();
	Socket* acceptServer();
	~Socket();

	//Fonctions client seulement
	Socket(char* _ipAddress, int _clientPort);
	bool connectClient();

	//Fonctions partagés par le serveur et le client
	bool sending(void* _message, int length);
	bool receiving(void* _message, int length);

	friend class SocketSelection;

private:
	//Constructeur privé
	Socket();

	//Structure pour le serveur
	SOCKET sock;
	SOCKADDR_IN sin;
	socklen_t recsize;

	//Pour récupérer les erreurs
    int sock_err;
};

#endif
