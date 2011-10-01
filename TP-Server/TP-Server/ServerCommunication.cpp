/**
	@file	ServerCommunication.cpp
			Fichier contenant l'implémentation de la classe ServerCommunication.
	@author Frédérik Paradis
	@author Louis-Étienne Dorval
	@date   16 mai 2011
*/

#include "ServerCommunication.h"

/**
	Cette méthode se charge de recevoir la réponse de la recherche
	exhaustive. C'est ce que le code 303 est supposé indiqué.
	@param [in] socket Le socket de réception
	@return Retourne la chaine de caractère représentant la réponse.
*/
std::string ServerCommunication::receive303Data(Socket* socket)
{
	//On reçoit la taille puis la chaine de caractère.
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
	Cette méthode envoie le code indiquant la réception	du code 301.
	@param [in] socket La socket d'envoie.
*/
void ServerCommunication::send301Response(Socket* socket)
{
	short response = 401;
	socket->sending(&response, sizeof(short));
}

/**
	Cette méthode se charge d'envoyer les paramètre de la recherche
	exhaustive au client.
	@param [in] letters Les lettres pour la recherche exhaustive
	@param [in] algo Le type de hash à envoyer
	@param [in] hash Le hash à envoyer
	@param [in] socket Le socket d'envoie
*/
void ServerCommunication::sendExhaustiveSearchSettings(const std::string& letters, const std::string& saltBefore, const std::string& saltAfter,  Algorithm::TypeAlgo algo, unsigned char* hash, Socket* socket)
{
	short response = 402;
	socket->sending(&response, sizeof(short));

	//Premier envoi: Code #1
	GeneralProtocol::sendExhaustiveSearchLettersAndSalts(letters, saltBefore, saltAfter, socket);

	//Deuxième envoi
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

	//Troisième envoi
	socket->sending(hash, nbCaractere);
}

/**
	Cette méthode se charge d'envoyer la tâche au client.
	@param [in] dispatch La tâche à envoyer
	@param [in] socket Le socket d'envoie
*/
void ServerCommunication::sendTask(Dispatch* dispatch, Socket* socket)
{
	short response = 403;
	socket->sending(&response, sizeof(short));

	//On envoie la string
	GeneralProtocol::sendString(dispatch->getDepart(), socket);

	//On envoie le nombre de possibilité à tester
	EntierLong toSend = dispatch->getNbEssais();
	socket->sending((void*)&toSend, sizeof(EntierLong));
}

/**
	Cette méthode envoie le code d'arrêt de la recherche exhaustive
	au client.
	@param [in] socket Le socket d'envoie
*/
void ServerCommunication::sendStopRequest(Socket* socket)
{
	short response = 404;
	socket->sending(&response, sizeof(short));
}

/**
	Cette méthode envoie le code de déconnexion	au client.
	@param [in] socket Le socket d'envoie
*/
void ServerCommunication::sendDisconnectRequest(Socket* socket)
{
	short response = 405;
	socket->sending(&response, sizeof(short));
}
