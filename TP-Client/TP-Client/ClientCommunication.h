/**
	@file	ClientCommunication.h
			Fichier contenant la déclaration de la classe ClientCommunication.
	@author Frédérik Paradis
	@author Louis-Étienne Dorval
	@date   16 mai 2011
*/

#pragma once
#include <string>
#include "Socket.h"
#include "Dispatch.h"

/**
	La classe ClientCommunication est une classe utilitaire qui
	permet d'implémenter du côté serveur la partie du protocole
	entre les clients et le serveur. Il n'y a donc que des 
	méthodes static	qui en font parties.
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

