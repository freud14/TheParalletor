/**
	@file	OperatorProtocol.h
			Fichier contenant la déclaration de la classe OperatorProtocol.
	@author Frédérik Paradis
	@author Louis-Étienne Dorval
	@date   16 mai 2011
*/

#pragma once
#include <string>
#include "Socket.h"
#include "Dispatch.h"
#include "WholeNumber.h"
#include "MD5.h"
#include "SHA1.h"
#include "GeneralProtocol.h"

/**
	La classe OperatorProtocol est une classe utilitaire qui
	permet d'implémenter du côté serveur la partie du protocole
	entre les opérateurs et le serveur. Il n'y a donc que des 
	méthodes static	qui en font parties.
*/
class OperatorProtocol
{
public:
	static void sendAcceptation(bool accept, short nbClient, Socket* socket);
	static void send102Response(Socket* socket);
	static void sendAddingNumberOfPossibilityTested(EntierLong number, Socket* socket);
	static void sendExhaustiveMD5SearchState(const std::string& letters, const std::string& saltBefore, const std::string& saltAfter, const MD5Search& md5, short nbClient, const WholeNumber& number, Socket* socket);
	static void sendExhaustiveSHA1SearchState(const std::string& letters, const std::string& saltBefore, const std::string& saltAfter, const SHA1Search& sha1, short nbClient, const WholeNumber& number, Socket* socket);
	static void sendNumberOfClient(short nbClient, Socket* socket);
	static void sendExhaustiveSearchAnswer(const std::string& str, Socket* socket);
	static void sendResultNotFound(Socket* socket);
private:
	OperatorProtocol(void);
	~OperatorProtocol(void);
};

