/**
	@file	Client.cpp
	Fichier contenant l'impl�mentation de la classe Client.
	@author Fr�d�rik Paradis
	@author Louis-�tienne Dorval
	@date   16 mai 2011
*/

#include "Client.h"

Client* Client::instance = NULL;

/**
	Cette m�thode se charge de g�rer la cr�ation du Singleton.
	@param [in] ip L'adresse IP du serveur
	@param [in] port Le port du serveur
*/
void Client::createInstance(char* ip, unsigned short port)
{
	assert(Client::instance == NULL);

	Client::instance = new Client(ip, port);
}

/**
	Cette m�thode donne acc�s au Singleton.
	@return Retourne le Singleton.
*/
Client* Client::getInstance()
{
	assert(Client::instance != NULL);

	return Client::instance;
}

/**
	Le constructeur initialise la connexion avec le serveur.
	@param [in] ip L'adresse IP du serveur
	@param [in] port Le port du serveur
*/
Client::Client(char* ip, unsigned short port)
{
	this->socket = new Socket(ip, port);
	this->stop = false;

	if(!this->socket->connectClient())
	{
		throw InvalidIPAdress();
	}

	this->dispatcher = NULL;
	this->algorithm = NULL;
}

/**
	Cette m�thode se charge de g�rer les communications avec
	le serveur.
*/
void Client::execute()
{
	//On cr�e la s�lection de socket pour la socket serveur
	SocketSelection* selection = new SocketSelection();
	selection->addSocket(this->socket);

	//On envoie la demande de connexion au serveur
	ClientCommunication::sendConnectRequest(this->socket);

	//Tant que le client veut rouler
	while(!this->stop)
	{
		int ms = 0;
		//On regarde si le serveur a envoy� des donn�es.
		std::vector<Socket*> toRead = selection->selection(&ms);
		if(toRead.size() != 0)
		{
			try
			{
				//Si oui, on les re�oit.
				this->readSocket();
			}
			catch(ServerDisconnectException& e)
			{
				//Si le serveur s'est d�connect�, on arr�te le client.
				this->stop = true;
			}
		}

		//Si une t�che est commenc�
		if(this->dispatcher != NULL)
		{
			//Si oui, on regarde si elle est fini.
			if(this->dispatcher->isFinish())
			{
				//Si elle est fini, on regarde si on a une r�ponse.
				std::string* temp = this->dispatcher->getResult();
				if(temp != NULL)
				{
					//Si oui, on envoie la r�ponse
					ClientCommunication::sendExhaustiveSearchResponse(*temp, this->socket);
					std::cout << "Reponse trouve : " << *temp << std::endl;
				}
				else
				{
					//Sinon, on demande une autre t�che.
					ClientCommunication::sendTaskRequest(this->socket);
				}
				delete this->dispatcher;
				this->dispatcher = NULL;
			}
		}

		Sleep(1);
	}

	if(this->dispatcher != NULL)
	{
		delete this->dispatcher;
	}
}

/**
	Cette m�tode se charge d'arr�ter le client et de le fermer.
*/
void Client::stopClient()
{
	this->stop = true;
}

/**
	Le destructeur se charge de fermer la connexion avec le serveur.
*/
Client::~Client(void)
{
	delete this->socket;
}

/**
	Cette m�thode se charge de recevoir et de r�pondre des
	informations au serveur.
*/
void Client::readSocket()
{
	//On re�oit le code serveur.
	GeneralProtocol::CodeProtocol code = GeneralProtocol::receiveData(this->socket);
	switch(code)
	{
	case GeneralProtocol::CODE401:
		NULL;
		std::cout << "Connexion au serveur reussie!" << std::endl;
		break;
	case GeneralProtocol::CODE402:
		{
			string tempLetters = "";
			string tempSaltBefore = "";
			string tempSaltAfter = "";
			GeneralProtocol::receiveExhaustiveSearchLettersAndSalts(tempLetters, tempSaltBefore, tempSaltAfter, this->socket);
			Algorithm::TypeAlgo algo = GeneralProtocol::receiveAlgoType(socket);
			unsigned char* hash;
			if(this->dispatcher == NULL)
			{
				if(this->algorithm != NULL)
				{
					delete this->algorithm;
					this->algorithm = NULL;
				}
			}
			if(algo == Algorithm::MD5)
			{
				hash = new unsigned char[16];
				ClientCommunication::receiveHash(algo, hash, this->socket);
				this->algorithm = new MD5(hash);
				delete[] hash;
			}
			else if(algo == Algorithm::SHA1)
			{
				hash = new unsigned char[20];
				ClientCommunication::receiveHash(algo, hash, this->socket);
				this->algorithm = new SHA1(hash);
				delete[] hash;
			}

			if(this->dispatcher == NULL)
			{
				if(this->algorithm != NULL)
				{
					Dispatch::setAlgorithm(this->algorithm);
					Dispatch::setLettersAndSalts(tempLetters, tempSaltBefore, tempSaltAfter);
				}
			}
			std::cout << "Reception des parametres de la recherche." << std::endl;
			break;
		}
	case GeneralProtocol::CODE403:
		{
			std::string str;
			EntierLong entier;
			ClientCommunication::receiveTask(str, entier, this->socket);

			if(dispatcher == NULL)
			{
				this->dispatcher = new Dispatcher(str, entier);
				this->dispatcher->start();
			}
			std::cout << "Reception d'une tache" << std::endl;
			break;
		}
	case GeneralProtocol::CODE404:
		{
			if(dispatcher != NULL)
			{
				this->dispatcher->stopDispatch();
				std::string* temp = dispatcher->getResult();
				if(temp != NULL)
				{
					ClientCommunication::sendExhaustiveSearchResponse(*temp, this->socket);
				}
				delete this->dispatcher;
				this->dispatcher = NULL;
			}
			std::cout << "Demande d'arret de la recherche exhaustive" << std::endl;
			break;
		}
	case GeneralProtocol::CODE405:
		{
			this->stop = true;
			std::cout << "Demande de deconnexion du client." << std::endl;
			break;
		}
	}
}