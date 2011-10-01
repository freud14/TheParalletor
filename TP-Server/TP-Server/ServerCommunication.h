/**
	@file	ServerCommunication.h
			Fichier contenant la déclaration de la classe ServerCommunication.
	@author Frédérik Paradis
	@author Louis-Étienne Dorval
	@date   16 mai 2011
*/

#pragma once
#include <string>
#include "Socket.h"
#include "Algorithm.h"
#include "MD5.h"
#include "SHA1.h"
#include "Dispatch.h"
#include "GeneralProtocol.h"

/**
	La classe ServerCommunication est une classe utilitaire qui
	permet d'implémenter du côté serveur la partie du protocole
	entre les clients et le serveur. Il n'y a donc que des 
	méthodes static	qui en font parties.
*/
class ServerCommunication
{
public:
	static std::string receive303Data(Socket* socket);
	static void send301Response(Socket* socket);
	static void sendExhaustiveSearchSettings(const std::string& letters, const std::string& saltBefore, const std::string& saltAfter,  Algorithm::TypeAlgo algo, unsigned char* hash, Socket* socket);
	static void sendTask(Dispatch* dispatch, Socket* socket);
	static void sendStopRequest(Socket* socket);
	static void sendDisconnectRequest(Socket* socket);
private:
	ServerCommunication();
	~ServerCommunication();
};

