/**
	@file	Client.h
			Fichier contenant la déclaration de la classe Client.
	@author Frédérik Paradis
	@author Louis-Étienne Dorval
	@date   16 mai 2011
*/

#pragma once
#ifdef linux
    #define Sleep(s) usleep(s * 1000)
#endif
#include <cassert>
#include <iostream>
#include <vector>
#include "Socket.h"
#include "InvalidIPAdress.h"
#include "SocketSelection.h"
#include "Dispatcher.h"
#include "ClientCommunication.h"
#include "GeneralProtocol.h"

/**
	La classe Client se charge de gérer les communications avec
	le serveur. Elle est la classe principale du programme et c’est 
	elle qui sert à démarrer les opérations du client.
*/
class Client
{
public:
	static void createInstance(char* ip, unsigned short port);
	static Client* getInstance();
	void execute();
	void stopClient();
private:
	Client(char* ip, unsigned short port);
	~Client();
	void readSocket();
	static Client* instance;
	Socket* socket;
	volatile bool stop;

	Algorithm* algorithm;
	Dispatcher* dispatcher;
};

