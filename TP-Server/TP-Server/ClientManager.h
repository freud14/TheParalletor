/**
	@file	ClientManager.h
			Fichier contenant la déclaration de la classe ClientManager.
	@author Frédérik Paradis
	@author Louis-Étienne Dorval
	@date   16 mai 2011
*/

#pragma once
#include <vector>
#include <Thread.h>
#include "Socket.h"
#include "SocketSelection.h"
#include "GeneralProtocol.h"
#include "ServerCommunication.h"

/**
	La classe ClientManager est la classe qui gère les connexions 
	de tous les clients. Elle hérite de la class Thread afin de
	pouvoir gérer les clients dans un tread différent.
*/
class ClientManager : public Thread
{
public:
	ClientManager(unsigned short port);
	virtual ~ClientManager(void);
	virtual DWORD execute();
	void beginDispatching();
	void stopDispatch();
private:
	void receiveClientMessage(Socket* socket, SocketSelection* selection, vector<Socket*>& allClient);
	Socket* serverSocket;
	volatile bool begin;
	volatile bool stopDispatching;
	volatile int nbClientFini;
};

