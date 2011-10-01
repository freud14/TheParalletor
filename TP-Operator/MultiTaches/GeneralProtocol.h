/**
	@file	GeneralProtocol.h
			Fichier contenant la déclaration de la classe GeneralProtocol.
	@author Frédérik Paradis
	@author Louis-Étienne Dorval
	@date   16 mai 2011
*/

#pragma once
#include <string>
#include "Socket.h"
#include "MD5.h"
#include "SHA1.h"

/**
	La classe GeneralProtocol est une classe utilitaire qui
	permet d'implémenter la partie générale du protocole
	créé pour ce programme. Il n'y a donc que des méthodes static
	qui en font parties.
*/
class GeneralProtocol
{
public:
	enum CodeProtocol { CODE101, CODE102, CODE103, CODE104, CODE105, CODE106, CODE201, CODE202, CODE203, CODE204, CODE205, CODE206, CODE301, CODE302, CODE303, CODE304};
	static CodeProtocol receiveData(Socket* socket);

	static std::string receiveExhaustiveSearchLetters(Socket* socket);
	static Algorithm::TypeAlgo receiveAlgoType(Socket* socket);
	static MD5Search receiveOperatorExhaustiveMD5SearchSettings(Socket* socket);
	static SHA1Search receiveOperatorExhaustiveSHA1SearchSettings(Socket* socket);

	static void sendExhaustiveSearchLetters(std::string letters, Socket* socket);
	static void sendOperatorExhaustiveMD5SearchSettings(MD5Search md5, Socket* socket);
	static void sendOperatorExhaustiveSHA1SearchSettings(SHA1Search sha1, Socket* socket);
	static void sendNumberOfClient(short nb, Socket* socket);

	static void sendString(const std::string& str, Socket* socket);
private:
	GeneralProtocol(void);
	~GeneralProtocol(void);
};

