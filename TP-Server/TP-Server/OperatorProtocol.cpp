/**
	@file	OperatorProtocol.cpp
			Fichier contenant l'implémentation de la classe OperatorProtocol.
	@author Frédérik Paradis
	@author Louis-Étienne Dorval
	@date   16 mai 2011
*/

#include "OperatorProtocol.h"

/**
	Envoie le code d'acceptation de l'opérateur. Si l'opérateur est accepté,
	le nombre de client est également envoyé.
	@param [in] accept Indique si le client est accepté.
	@param [in] nbClient Indique le nombre de client. Ce paramètre n'est pas
	pris en compte si le client est refusé.
	@param [in] socket La socket d'envoie.
*/
void OperatorProtocol::sendAcceptation(bool accept, short nbClient, Socket* socket)
{
	short code201 = 201;
	socket->sending(&code201, sizeof(short));

	socket->sending(&accept, sizeof(bool));

	if(accept)
	{
		GeneralProtocol::sendNumberOfClient(nbClient, socket);
	}
}

/**
	Cette méthode envoie la réception du code 102.
	@param [in] socket La socket d'envoie.
*/
void OperatorProtocol::send102Response(Socket* socket)
{
	short code202 = 202;
	socket->sending(&code202, sizeof(short));
}

/**
	Cette méthode permet d'envoyer un nombre de 8 octets indiquant combien 
	de possibilité de plus ont été testé par les clients.
	@param [in] number Le nombre de possibilités testées de plus
	@param [in] socket La socket d'envoie.
*/
void OperatorProtocol::sendAddingNumberOfPossibilityTested(EntierLong number, Socket* socket)
{
	short code203 = 203;
	socket->sending(&code203, sizeof(short));

	socket->sending(&number, sizeof(EntierLong));
}

/**
	Cette méthode se charge d'envoyer les paramètres de la recherche exhaustive
	MD5 à l'opérateur.
	@param [in] letters Les lettres à tester dans la recherche exhaustive
	@param [in] md5 Le contexte de recherche
	@param [in] nbClient Le quota(en millions) à envoyer à chacun des clients
	@param [in] number Le nombre total de possibilité testé
	@param [in] socket Le socket d'envoie.
*/
void OperatorProtocol::sendExhaustiveMD5SearchState(const std::string& letters, const std::string& saltBefore, const std::string& saltAfter, const MD5Search& md5, short nbClient, const WholeNumber& number, Socket* socket)
{
	short code204 = 204;
	socket->sending(&code204, sizeof(short));

	//Code #1
	GeneralProtocol::sendExhaustiveSearchLettersAndSalts(letters, saltBefore, saltAfter, socket);

	//Code #2
	GeneralProtocol::sendOperatorExhaustiveMD5SearchSettings(md5, socket);

	//Code #3
	GeneralProtocol::sendNumberOfClient(nbClient, socket);

	//On envoie la string
	string temp = number.toString();
	GeneralProtocol::sendString(temp, socket);
}

/**
	Cette méthode se charge d'envoyer les paramètres de la recherche exhaustive
	SHA-1 à l'opérateur.
	@param [in] letters Les lettres à tester dans la recherche exhaustive
	@param [in] sha1 Le contexte de recherche
	@param [in] nbClient Le quota(en millions) à envoyer à chacun des clients
	@param [in] number Le nombre total de possibilité testé
	@param [in] socket Le socket d'envoie.
*/
void OperatorProtocol::sendExhaustiveSHA1SearchState(const std::string& letters, const std::string& saltBefore, const std::string& saltAfter, const SHA1Search& sha1, short nbClient, const WholeNumber& number, Socket* socket)
{
	short code204 = 204;
	socket->sending(&code204, sizeof(short));

	//Code #1
	GeneralProtocol::sendExhaustiveSearchLettersAndSalts(letters, saltBefore, saltAfter, socket);

	//Code #2
	GeneralProtocol::sendOperatorExhaustiveSHA1SearchSettings(sha1, socket);

	//Code #3
	GeneralProtocol::sendNumberOfClient(nbClient, socket);

	//On envoie la string
	string temp = number.toString();
	GeneralProtocol::sendString(temp, socket);
}

/**
	Cette méthode se charge d'envoyer le nombre de client à l'opérateur.
	@param [in] nbClient Le nombre de client
	@param [in] socket Le socket d'envoie
*/
void OperatorProtocol::sendNumberOfClient(short nbClient, Socket* socket)
{
	short code205 = 205;
	socket->sending(&code205, sizeof(short));

	//Code #3
	GeneralProtocol::sendNumberOfClient(nbClient, socket);
}

/**
	Cette méthode se charge d'envoyer la réponse de la recherche exhaustive
	à l'opérateur.
	@param [in] str La réponse de la recherche exhaustive
	@param [in] socket Le socket d'envoie
*/
void OperatorProtocol::sendExhaustiveSearchAnswer(const std::string& str, Socket* socket)
{
	short code206 = 206;
	socket->sending(&code206, sizeof(short));

	//On envoie la string
	GeneralProtocol::sendString(str, socket);
}

/**
	Cette méthode se charge de dire à l'opérateur que la recherche n'a rien donné.
	@param [in] socket Le socket d'envoie
*/
void OperatorProtocol::sendResultNotFound(Socket* socket)
{
	short code208 = 208;
	socket->sending(&code208, sizeof(short));	
}