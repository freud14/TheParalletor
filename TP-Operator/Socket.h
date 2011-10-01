/**
        @file	Socket.h
                        Fichier contenant la d�claration de la classe Socket
        @author Fr�d�rik Paradis
        @author Louis-�tienne Dorval
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

	//Fonctions partag�s par le serveur et le client
	bool sending(void* _message, int length);
	bool receiving(void* _message, int length);

	friend class SocketSelection;

private:
	//Constructeur priv�
	Socket();

	//Structure pour le serveur
	SOCKET sock;
	SOCKADDR_IN sin;
	socklen_t recsize;

	//Pour r�cup�rer les erreurs
    int sock_err;
};

#endif
