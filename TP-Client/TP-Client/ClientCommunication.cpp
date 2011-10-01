/**
	@file	ClientCommunication.cpp
			Fichier contenant l'implémentation de la classe ClientCommunication.
	@author Frédérik Paradis
	@author Louis-Étienne Dorval
	@date   16 mai 2011
*/

#include "ClientCommunication.h"

/**
	Cette méthode se charge de recevoir une tâche du serveur.
	@param [out] str La chaine de départ de la recherche.
	@param [out] number Le nombre de possibilité à tester.
	@param [in] socket Le socket de réception.
*/
void ClientCommunication::receiveTask(std::string& str, EntierLong& number, Socket* socket)
{
	//On reçoit le nombre de caractère de la chaine de départ
	short nbChar;
	socket->receiving(&nbChar, sizeof(short));

	//On reçoit la chaine de caractère
	char* retour = new char[nbChar + 1];
	socket->receiving(retour, nbChar);
	retour[nbChar] = 0;
	str = retour;
	delete[] retour;

	//On reçoit le nombre de possibilité à tester.
	EntierLong temp;
	socket->receiving(&temp, sizeof(EntierLong));
	number = temp;
}

/**
	Cette méthode se charge de recevoir le hash selon l'algorithme voulu.
	@param [in] algo Le type de hash à recevoir
	@param [out] hash Le hash à recevoir. Veillez noter que la taille du tableau
	doit être adéquate à la taille du type de hash
	@param [in] socket Le socket de réception
*/
void ClientCommunication::receiveHash(Algorithm::TypeAlgo algo, unsigned char* hash, Socket* socket)
{
	switch(algo)
	{
	case Algorithm::MD5:
		socket->receiving(hash, 16);
		break;
	case Algorithm::SHA1:
		socket->receiving(hash, 20);
		break;
	}
}

/**
	Cette méthode envoie le code pour la demande de
	connexion selon le protocole établie.
	@param [in] socket La socket d'envoie
*/
void ClientCommunication::sendConnectRequest(Socket* socket)
{
	short code301 = 301;
	socket->sending(&code301, sizeof(short));
}

/**
	Cette méthode envoie le code pour une demande de
	tâche selon le protocole établie.
	@param [in] socket La socket d'envoie
*/
void ClientCommunication::sendTaskRequest(Socket* socket)
{
	short code302 = 302;
	socket->sending(&code302, sizeof(short));
}

/**
	Cette méthode envoie le code pour prévenir le serveur que
	le client a trouvé la solution.
	@param [in] str La chaine corrrespondant au hash.
	@param [in] socket La socket d'envoie
*/
void ClientCommunication::sendExhaustiveSearchResponse(const std::string& str, Socket* socket)
{
	short code303 = 303;
	socket->sending(&code303, sizeof(short));

	short len = str.length();
	socket->sending(&len, sizeof(short));

	socket->sending((void*)str.c_str(), str.length());
}
