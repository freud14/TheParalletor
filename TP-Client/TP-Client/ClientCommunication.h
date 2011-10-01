/**
	@file	ClientCommunication.h
			Fichier contenant la d�claration de la classe ClientCommunication.
	@author Fr�d�rik Paradis
	@author Louis-�tienne Dorval
	@date   16 mai 2011
*/

#pragma once
#include <string>
#include "Socket.h"
#include "Dispatch.h"

/**
	La classe ClientCommunication est une classe utilitaire qui
	permet d'impl�menter du c�t� serveur la partie du protocole
	entre les clients et le serveur. Il n'y a donc que des 
	m�thodes static	qui en font parties.
*/
class ClientCommunication
{
public:
	static void receiveTask(std::string& str, EntierLong& number, Socket* socket);
	static void receiveHash(Algorithm::TypeAlgo algo, unsigned char* hash, Socket* socket);

	static void sendConnectRequest(Socket* socket);
	static void sendTaskRequest(Socket* socket);
	static void sendExhaustiveSearchResponse(const std::string& str, Socket* socket);
private:
	ClientCommunication(void);
	~ClientCommunication(void);
};

