/**
        @file	SocketSelection.h
                        Fichier contenant la d�claration de la classe SocketSelection
        @author Fr�d�rik Paradis
        @author Louis-�tienne Dorval
        @date   16 mai 2011
*/

#ifndef SOCKETSELECTION_H_INCLUDED
#define SOCKETSELECTION_H_INCLUDED

#include "Socket.h"

#include <vector>
#include <time.h>

using namespace std;

/**
  Cette classe encapsule des m�thodes en C pour faire la s�lection de socket
*/
class SocketSelection
{
public:
	SocketSelection();
	~SocketSelection();
	bool addSocket(Socket* _socket);
	bool removeSocket(Socket* _socket);
	vector<Socket*> selection(int* _msTimeOut);
private:
	std::vector<Socket*> vecSocket;
	fd_set readfs;
};
#endif
