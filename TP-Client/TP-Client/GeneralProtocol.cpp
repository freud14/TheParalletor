/**
	@file	GeneralProtocol.cpp
			Fichier contenant l'impl�mentation de la classe GeneralProtocol.
	@author Fr�d�rik Paradis
	@author Louis-�tienne Dorval
	@date   16 mai 2011
*/

#include "GeneralProtocol.h"

/**
	Cette m�thode se charge de recevoir le code du serveur.
	@param [in] socket La socket de r�zzception
	@return Retourne le code que le serveur a envoy�.
*/
GeneralProtocol::CodeProtocol GeneralProtocol::receiveData(Socket* socket)
{
	CodeProtocol retour;
	short resultat;
	if(socket->receiving(&resultat, sizeof(short)))
	{
		switch(resultat)
		{
		case 401:
			retour = GeneralProtocol::CODE401;
			break;
		case 402:
			retour = GeneralProtocol::CODE402;
			break;
		case 403:
			retour = GeneralProtocol::CODE403;
			break;
		case 404:
			retour = GeneralProtocol::CODE404;
			break;
		case 405:
			retour = GeneralProtocol::CODE405;
			break;
		}
	}
	else
	{
		throw ServerDisconnectException();
	}
	return retour;
}

/**
	Cette m�thode se charge de recevoir le code #1 c'est-�-dire les lettres �
	tester dans la recherche exhaustive.
	@param [in] socket La socket de r�ception
	@return Retourne les lettres voulu dans l'algorithme de recherche exhaustive.
*/
void GeneralProtocol::receiveExhaustiveSearchLettersAndSalts(std::string& letters, std::string& saltBefore, std::string& saltAfter, Socket* socket)
{
	short code1;
	socket->receiving(&code1, sizeof(short));

	letters = GeneralProtocol::receiveString(socket);
	saltBefore = GeneralProtocol::receiveSalt(socket);
	saltAfter = GeneralProtocol::receiveSalt(socket);
}

/**
	Cette m�thode se charge de recevoir le type d'algorithme envoy� par
	le serveur. Rappelons que le serveur doit envoyer 1 pour avoir l'algorithme
	MD5 et 2 pour l'algorithme SHA-1.
	@param [in] socket La socket de r�ception
	@return Retourne le type d'algorithme.
*/
Algorithm::TypeAlgo GeneralProtocol::receiveAlgoType(Socket* socket)
{
	//On re�oit le short qui repr�sente le type de hash
	short type;
	socket->receiving(&type, sizeof(short));
	
	//On renvoie le TypeAlgo associ�
	Algorithm::TypeAlgo retour;
	switch(type)
	{
	case 1:
		retour = Algorithm::MD5;
		break;
	case 2:
		retour = Algorithm::SHA1;
		break;
	}

	return retour;
}

/**
	Cette m�thode se charge de recevoir les param�tres d'une
	recherche exhaustive sur un hash MD5.
	@param [in] socket La socket de r�ception
	@return Retourne les param�tres de la recherche exhaustive sur un hash MD5.
*/
MD5Search GeneralProtocol::receiveOperatorExhaustiveMD5SearchSettings(Socket* socket)
{
	MD5Search md5;
	socket->receiving(&md5, sizeof(MD5Search));

	return md5;
}

/**
	Cette m�thode se charge de recevoir les param�tres d'une
	recherche exhaustive sur un hash SHA-1.
	@param [in] socket La socket de r�ception
	@return Retourne les param�tres de la recherche exhaustive sur un hash SHA-1.
*/
SHA1Search GeneralProtocol::receiveOperatorExhaustiveSHA1SearchSettings(Socket* socket)
{
	SHA1Search sha1;
	socket->receiving(&sha1, sizeof(SHA1Search));

	return sha1;
}

/**
	Cette m�thode se charge d'envoyer les lettres pour la recherche
	exhaustive.
	@param [in] letters Les lettres pour la recherche exhaustive
	@param [in] socket La socket d'envoi
*/
void GeneralProtocol::sendExhaustiveSearchLettersAndSalts(std::string letters, std::string saltBefore, std::string saltAfter, Socket* socket)
{
	short code1 =  1;
	socket->sending(&code1, sizeof(short));

	//On envoie la string
	GeneralProtocol::sendString(letters, socket);
	GeneralProtocol::sendSalt(saltBefore, socket);
	GeneralProtocol::sendSalt(saltAfter, socket);
}

/**
	Cette m�thode se charge d'envoyer les param�tres de la recherches exhaustives
	pour un hash MD5.
	@param [in] md5 Les param�tres de la recherche
	@param [in] socket La socket d'envoi
*/
void GeneralProtocol::sendOperatorExhaustiveMD5SearchSettings(MD5Search md5, Socket* socket)
{
	short code2 = 2;
	socket->sending(&code2, sizeof(short));

	short algo = 1;
	socket->sending(&algo, sizeof(short));

	socket->sending(&md5, sizeof(MD5Search));
}

/**
	Cette m�thode se charge d'envoyer les param�tres de la recherches exhaustives
	pour un hash SHA-1.
	@param [in] sha1 Les param�tres de la recherche
	@param [in] socket La socket d'envoi
*/
void GeneralProtocol::sendOperatorExhaustiveSHA1SearchSettings(SHA1Search sha1, Socket* socket)
{
	short code2 = 2;
	socket->sending(&code2, sizeof(short));

	short algo = 2;
	socket->sending(&algo, sizeof(short));

	socket->sending(&sha1, sizeof(SHA1Search));
}

/**
	Cette m�thode se charge d'envoyer le nombre de client
	connect� sur le serveur. Pour rappel, cette m�thode envoie
	le code #1 avant d'envoyer le nombre.
	@param [in] nb Le nombre de client connect�.
	@param [in] socket La socket d'envoie.
*/
void GeneralProtocol::sendNumberOfClient(short nb, Socket* socket)
{
	short code3 = 3;
	socket->sending(&code3, sizeof(short));

	socket->sending(&nb, sizeof(short));
}

/**
	Cette m�thode se charge d'envoyer une chaine de caract�re.
	@param [in] str La chaine de caract�re � envoyer
	@param [in] socket La socket d'envoie.
*/
void GeneralProtocol::sendString(const std::string& str, Socket* socket)
{
	short nbLettre = str.length();
	socket->sending(&nbLettre, sizeof(short));
	socket->sending((void*)str.c_str(), str.length());
}

std::string GeneralProtocol::receiveString(Socket* socket)
{
	//On re�oit le nombre de lettre
	short nbLetter;
	socket->receiving(&nbLetter, sizeof(short));
	
	//On re�oit la chaine de caract�re.
	char* temp = new char[nbLetter + 1];
	socket->receiving(temp, nbLetter);
	temp[nbLetter] = 0;
	std::string retour = temp;
	delete[] temp;

	return retour;
}

std::string GeneralProtocol::receiveSalt(Socket* socket)
{
	//On re�oit le nombre de lettre
	short nbLetter;
	socket->receiving(&nbLetter, sizeof(short));
	
	std::string retour = "";
	if(nbLetter != 0)
	{
		//On re�oit la chaine de caract�re.
		char* temp = new char[nbLetter + 1];
		socket->receiving(temp, nbLetter);
		temp[nbLetter] = 0;
		retour = temp;
		delete[] temp;
	}

	return retour;
}

void GeneralProtocol::sendSalt(const std::string& str, Socket* socket)
{
	short nbLettre = str.length();
	socket->sending(&nbLettre, sizeof(short));
	if(nbLettre != 0)
	{
		socket->sending((void*)str.c_str(), str.length());
	}
}