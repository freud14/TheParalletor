/**
	@file	ServerCommunication.cpp
			Fichier contenant l'impl�mentation de la classe ServerCommunication.
	@author Fr�d�rik Paradis
	@author Louis-�tienne Dorval
	@date   16 mai 2011
*/

#include "ServerCommunication.h"

/**
	Cette m�thode se charge de recevoir la r�ponse de la recherche
	exhaustive. C'est ce que le code 303 est suppos� indiqu�.
	@param [in] socket Le socket de r�ception
	@return Retourne la chaine de caract�re repr�sentant la r�ponse.
*/
std::string ServerCommunication::receive303Data(Socket* socket)
{
	//On re�oit la taille puis la chaine de caract�re.
	short nbCaractere;
	socket->receiving(&nbCaractere, sizeof(short));
	char* resultat = new char[nbCaractere + 1];
	socket->receiving(resultat, nbCaractere);
	resultat[nbCaractere] = 0;
	std::string retour = resultat;
	delete[] resultat;
	return retour;
}

/**
	Cette m�thode envoie le code indiquant la r�ception	du code 301.
	@param [in] socket La socket d'envoie.
*/
void ServerCommunication::send301Response(Socket* socket)
{
	short response = 401;
	socket->sending(&response, sizeof(short));
}

/**
	Cette m�thode se charge d'envoyer les param�tre de la recherche
	exhaustive au client.
	@param [in] letters Les lettres pour la recherche exhaustive
	@param [in] algo Le type de hash � envoyer
	@param [in] hash Le hash � envoyer
	@param [in] socket Le socket d'envoie
*/
void ServerCommunication::sendExhaustiveSearchSettings(const std::string& letters, const std::string& saltBefore, const std::string& saltAfter,  Algorithm::TypeAlgo algo, unsigned char* hash, Socket* socket)
{
	short response = 402;
	socket->sending(&response, sizeof(short));

	//Premier envoi: Code #1
	GeneralProtocol::sendExhaustiveSearchLettersAndSalts(letters, saltBefore, saltAfter, socket);

	//Deuxi�me envoi
	short type;
	short nbCaractere;
	if(algo == Algorithm::MD5)
	{
		type = 1;
		nbCaractere = 16;
	}
	else if(algo == Algorithm::SHA1)
	{
		type = 2;
		nbCaractere = 20;
	}
	socket->sending(&type, sizeof(short));

	//Troisi�me envoi
	socket->sending(hash, nbCaractere);
}

/**
	Cette m�thode se charge d'envoyer la t�che au client.
	@param [in] dispatch La t�che � envoyer
	@param [in] socket Le socket d'envoie
*/
void ServerCommunication::sendTask(Dispatch* dispatch, Socket* socket)
{
	short response = 403;
	socket->sending(&response, sizeof(short));

	//On envoie la string
	GeneralProtocol::sendString(dispatch->getDepart(), socket);

	//On envoie le nombre de possibilit� � tester
	EntierLong toSend = dispatch->getNbEssais();
	socket->sending((void*)&toSend, sizeof(EntierLong));
}

/**
	Cette m�thode envoie le code d'arr�t de la recherche exhaustive
	au client.
	@param [in] socket Le socket d'envoie
*/
void ServerCommunication::sendStopRequest(Socket* socket)
{
	short response = 404;
	socket->sending(&response, sizeof(short));
}

/**
	Cette m�thode envoie le code de d�connexion	au client.
	@param [in] socket Le socket d'envoie
*/
void ServerCommunication::sendDisconnectRequest(Socket* socket)
{
	short response = 405;
	socket->sending(&response, sizeof(short));
}
