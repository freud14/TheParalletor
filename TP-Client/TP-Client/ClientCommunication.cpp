/**
	@file	ClientCommunication.cpp
			Fichier contenant l'impl�mentation de la classe ClientCommunication.
	@author Fr�d�rik Paradis
	@author Louis-�tienne Dorval
	@date   16 mai 2011
*/

#include "ClientCommunication.h"

/**
	Cette m�thode se charge de recevoir une t�che du serveur.
	@param [out] str La chaine de d�part de la recherche.
	@param [out] number Le nombre de possibilit� � tester.
	@param [in] socket Le socket de r�ception.
*/
void ClientCommunication::receiveTask(std::string& str, EntierLong& number, Socket* socket)
{
	//On re�oit le nombre de caract�re de la chaine de d�part
	short nbChar;
	socket->receiving(&nbChar, sizeof(short));

	//On re�oit la chaine de caract�re
	char* retour = new char[nbChar + 1];
	socket->receiving(retour, nbChar);
	retour[nbChar] = 0;
	str = retour;
	delete[] retour;

	//On re�oit le nombre de possibilit� � tester.
	EntierLong temp;
	socket->receiving(&temp, sizeof(EntierLong));
	number = temp;
}

/**
	Cette m�thode se charge de recevoir le hash selon l'algorithme voulu.
	@param [in] algo Le type de hash � recevoir
	@param [out] hash Le hash � recevoir. Veillez noter que la taille du tableau
	doit �tre ad�quate � la taille du type de hash
	@param [in] socket Le socket de r�ception
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
	Cette m�thode envoie le code pour la demande de
	connexion selon le protocole �tablie.
	@param [in] socket La socket d'envoie
*/
void ClientCommunication::sendConnectRequest(Socket* socket)
{
	short code301 = 301;
	socket->sending(&code301, sizeof(short));
}

/**
	Cette m�thode envoie le code pour une demande de
	t�che selon le protocole �tablie.
	@param [in] socket La socket d'envoie
*/
void ClientCommunication::sendTaskRequest(Socket* socket)
{
	short code302 = 302;
	socket->sending(&code302, sizeof(short));
}

/**
	Cette m�thode envoie le code pour pr�venir le serveur que
	le client a trouv� la solution.
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
