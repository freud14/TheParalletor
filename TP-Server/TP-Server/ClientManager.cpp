/**
	@file	ClientManager.cpp
			Fichier contenant l'impl�mentation de la classe ClientManager.
	@author Fr�d�rik Paradis
	@author Louis-�tienne Dorval
	@date   16 mai 2011
*/

#include "ClientManager.h"
#include "Server.h"

/**
	Le constructeur initialise la connexion cliente.
	@param [in] port Le port o� l'on doit accepter les clients
*/
ClientManager::ClientManager(unsigned short port)
{
	this->serverSocket = new Socket(port);
	this->begin = false;
	this->stopDispatching = false;
	this->nbClientFini = 0;
}

/**
	Le destructeur envoie le signal au thread qui s'occupe
	des clients de s'arr�ter et attend qu'il s'arr�te. Il
	lib�re �galement les autres ressources prises par l'objet.
*/
ClientManager::~ClientManager()
{
	this->waitForThread();
	delete this->serverSocket;
}

/**
	Cette m�thode se charge de g�rer les clients du serveur.
	Habituellement, on l'ex�cute dans un thread � part.
	@return Retourne 0 si tout est correct.
*/
DWORD ClientManager::execute()
{
	this->threadTerminated = false;

	//On �coute notre socket serveur
	this->serverSocket->listenServer();

	//On cr�e la s�lection de socket pour accepter les clients
	SocketSelection* acceptClient = new SocketSelection();
	acceptClient->addSocket(this->serverSocket);

	//On cr�e la s�lection pour recevoir des donn�es des clients
	SocketSelection* selection = new SocketSelection();
	std::vector<Socket*> allClient;

	//Tant qu'on veut que le serveur roule...
	while(!this->threadTerminated)
	{
		Server* serveur = Server::getInstance();
		int ms = 0;
		vector<Socket*> socketToAccept = acceptClient->selection(&ms);
		vector<Socket*> socketToRead = selection->selection(&ms);

		//On regarde s'il y a des clients � accepter
		if(socketToAccept.size() != 0)
		{
			Socket* cliSock = this->serverSocket->acceptServer();
			selection->addSocket(cliSock);
			allClient.push_back(cliSock);
			serveur->addClient();
		}
		
		//On regarde s'il y a des messages � recevoir des clients
		if(socketToRead.size() != 0)
		{
			//S'il y a des messages � recevoir, on les re�ois.
			for(vector<Socket*>::iterator it = socketToRead.begin(); it != socketToRead.end(); ++it)
			{
				try
				{
					//On re�oit les donn�es
					receiveClientMessage(*it, selection, allClient);
				}
				catch(ClientDisconnectException& e)
				{
					// Si un client se d�connecte, on le supprime de la s�lection de socket, on ferme
					// la connexion et on le supprime de la liste des clients.
					selection->removeSocket(*it);
					delete *it;
					vector<Socket*>::iterator toDel;
					for(vector<Socket*>::iterator it2 = allClient.begin(); it2 != allClient.end(); ++it2)
					{
						if(*it2 == *it)
						{
							toDel = it2;
							break;
						}
					}
					allClient.erase(toDel);
					serveur->removeClient();
				}
			}
		}

		//On regarde si la division de la t�ches doit commencer.
		if(this->begin)
		{
			this->begin = false;
			for(std::vector<Socket*>::iterator it = allClient.begin(); it != allClient.end(); ++it)
			{
				ServerCommunication::sendExhaustiveSearchSettings(serveur->getLetters(), serveur->getSaltBefore(), serveur->getSaltAfter(), serveur->getTypeAlgo(), serveur->getAlgo()->getHash(), *it);
				Dispatch dis = serveur->getDispatch();
				ServerCommunication::sendTask(&dis, *it);
			}
		}
		
		//On regarde si on a l'ordre d'arr�ter la recherche.
		if(this->stopDispatching)
		{
			//Si oui, on envoie la demande � tous les clients.
			this->stopDispatching = false;
			for(std::vector<Socket*>::iterator it = allClient.begin(); it != allClient.end(); ++it)
			{
				ServerCommunication::sendStopRequest(*it);
			}
			
		}

		Sleep(1);
	}

	delete acceptClient;
	delete selection;

	//On envoie le code d'arr�t au client.
	for(std::vector<Socket*>::iterator it = allClient.begin(); it != allClient.end(); ++it)
	{
		ServerCommunication::sendDisconnectRequest(*it);
		delete (*it);
	}

	return 0;
}

/**
	Cette m�thode se charge de g�rer la r�ception et les r�ponses
	des messages des clients.
	@param [in] socket La socket de r�ception et d'envoie
	@param [in] selection La s�lection de socket qui peut servir pour
	pouvoir supprimer le socket de la s�lection si besoin est.
	@param [in] allClient La liste de tous les clients.
*/
void ClientManager::receiveClientMessage(Socket* socket, SocketSelection* selection, vector<Socket*>& allClient)
{
	Server* serveur = Server::getInstance();
	//On re�oit le code du client
	GeneralProtocol::CodeProtocol code = GeneralProtocol::receiveData(socket);
	switch(code)
	{
	case GeneralProtocol::CODE301:
		{
			ServerCommunication::send301Response(socket);
			if(serveur->isBegin()) //Si la t�che est commenc�, on envoie les informations.
			{
				ServerCommunication::sendExhaustiveSearchSettings(serveur->getLetters(), serveur->getSaltBefore(), serveur->getSaltAfter(), serveur->getTypeAlgo(), serveur->getAlgo()->getHash(), socket);
				Dispatch dis = serveur->getDispatch();
				ServerCommunication::sendTask(&dis, socket);
			}
			std::cout << "Un client vient de se connecter." << std::endl;
			break;
		}
	case GeneralProtocol::CODE302:
		{
			if(serveur->isBegin()) //Si la t�che est commenc�, on envoie les informations.
			{
				Dispatch dis = serveur->getDispatch();
				serveur->addPossibilityTested(dis.getNbEssais());
				if(dis.getDepart().length() <= serveur->getSearchMaxCharacter())
				{
					ServerCommunication::sendTask(&dis, socket);
				}
				else
				{
					this->nbClientFini++;
					if(nbClientFini == allClient.size())
					{
						serveur->resultNotFound();
						this->nbClientFini = 0;
					}
				}
			}
			std::cout << "Demande de tache d'un client." << std::endl;
			break;
		}
	case GeneralProtocol::CODE303:
		std::cout << "Un client a trouve la reponse a la recherche exhaustive." << std::endl;
		if(serveur->isBegin()) //Si la t�che est commenc�, on envoie les informations.
		{
			if(!serveur->stopDispatch(ServerCommunication::receive303Data(socket))) //On v�rifie si l'information re�ue est bonne.
			{
				selection->removeSocket(socket);
				vector<Socket*>::iterator del;
				for(vector<Socket*>::iterator cli = allClient.begin(); cli != allClient.end(); ++cli)
				{
					if(socket == *cli)
					{
						del = cli;
					}
				}
				allClient.erase(del);
				delete socket;
			}
			else
			{
				this->nbClientFini = 0;
			}
		}
		break;
	case GeneralProtocol::CODE304:
		selection->removeSocket(socket);
		vector<Socket*>::iterator del;
		for(vector<Socket*>::iterator cli = allClient.begin(); cli != allClient.end(); ++cli)
		{
			if(socket == *cli)
			{
				del = cli;
			}
		}
		allClient.erase(del);
		delete socket;
		std::cout << "Un client vient de se deconnecter." << std::endl;
		break;
	}
}

/**
	Cette m�thode se charge d'avertir le thread traitant les clients que
	les clients peuvent commencer � effectuer la recherche exhaustive
	avec les informations que le serveur a.
*/
void ClientManager::beginDispatching()
{
	this->begin = true;
	this->stopDispatching = false;
}

/**
	Cette m�thode se charge d'avertir le thread traitant les clients
	d'arr�ter d'effectuer la recherche exhaustive.
*/
void ClientManager::stopDispatch()
{
	this->begin = false;
	this->stopDispatching = true;
}