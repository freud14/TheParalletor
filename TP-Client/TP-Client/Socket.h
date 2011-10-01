/**
        @file	Socket.h
                        Fichier contenant la déclaration de la classe Socket
        @author Frédérik Paradis
        @author Louis-Étienne Dorval
        @date   16 mai 2011
*/

#ifndef SOCKET_H_INCLUDED
#define SOCKET_H_INCLUDED
#ifdef _WIN32
#include <Windows.h>
typedef int socklen_t;
#elif defined (linux)

    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>

    // Define, qui nous serviront par la suite
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    #define closesocket(s) close (s)

    // De même
    typedef int SOCKET;
    typedef struct sockaddr_in SOCKADDR_IN;
    typedef struct sockaddr SOCKADDR;
#endif
#include <assert.h>


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
