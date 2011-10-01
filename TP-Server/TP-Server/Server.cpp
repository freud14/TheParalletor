/**
	@file	Server.cpp
	Fichier contenant l'implémentation de la classe Server.
	@author Frédérik Paradis
	@author Louis-Étienne Dorval
	@date   16 mai 2011
*/

#include "Server.h"

volatile Server* Server::instance = NULL;

/**
	Cette méthode se charge de gérer la création du Singleton.
	@param [in] portOperator Le port pour les opérateurs
	@param [in] portClient Le port pour les clients.
*/
void Server::createInstance(unsigned short portOperator, unsigned short portClient)
{
	assert(Server::instance == NULL);

	Server::instance = new Server(portOperator, portClient);
}

/**
	Cette méthode donne accès au Singleton.
	@return Retourne le Singleton.
*/
Server* Server::getInstance()
{
	assert(Server::instance != NULL);

	return (Server*)Server::instance;
}

/**
	Le constructeur initialise l'objet.
	@param [in] portOperator Le port pour les opérateurs
	@param [in] portClient Le port pour les clients.
*/
Server::Server(unsigned short portOperator, unsigned short portClient)
{
	assert(portOperator != portClient);

	this->portOperator = portOperator;
	this->portClient = portClient;
	this->dispatcher = NULL;
	this->algo = NULL;
	this->letters = NULL;
	this->saltBefore = NULL;
	this->saltAfter = NULL;
	this->stop = false;
	this->nbClient = 0;
	this->answer = new std::string;

	this->nbEssai = new WholeNumber((unsigned long long)0);
}

/**
	Cette méthode se charge de libérer les ressources prises par l'objet.
*/
Server::~Server()
{
	if(this->dispatcher != NULL)
	{
		delete this->dispatcher;
	}
	delete this->answer;
	delete this->nbEssai;
	delete this->clients;
}

/**
	Cette méthode se charge de démarrer le thread pour le
	gestionnaire de client.
*/
void Server::startClientManager()
{
	//On démarre le client manager
	this->clients = new ClientManager(portClient);
	this->clients->start();
}

/**
	Cette méthode se charge des gérer les connexions avec les opérateurs.
*/
void Server::execute()
{
	//On boucle pour avoir de quoi à envoyer ou recevoir de l'opérateur
	
	//On crée notre socket serveur et on l'écoute
	Socket* serverSocket = new Socket(this->portOperator); 
	serverSocket->listenServer(); 
	
	//On crée la sélection de socket pour accepter les opérateurs
	SocketSelection* acceptOperator = new SocketSelection();
	acceptOperator->addSocket(serverSocket);

	//On crée la sélection pour recevoir des données des opérateurs
	SocketSelection* selection = new SocketSelection();
	std::vector<Socket*> allOperator;

	//Tant qu'on veut que le serveur roule...
	while(!this->stop)
	{
		int ms = 0;
		vector<Socket*> socketToAccept = acceptOperator->selection(&ms);
		vector<Socket*> socketToRead = selection->selection(&ms);

		//On regarde s'il y a des opérateurs à accepter
		if(socketToAccept.size() != 0)
		{
			Socket* opSock = serverSocket->acceptServer();
			selection->addSocket(opSock);
			allOperator.push_back(opSock);
			std::cout << "Un operateur vient de se connecter." << std::endl;
		}

		//On regarde s'il y a des messages à recevoir des opérateurs
		if(socketToRead.size() != 0)
		{
			//S'il y a des messages à recevoir, on les reçois.
			for(vector<Socket*>::iterator it = socketToRead.begin(); it != socketToRead.end(); ++it)
			{
				try
				{
					//On reçoit les données
					receiveOperatorMessage(*it, selection, allOperator);
				}
				catch(ClientDisconnectException& e)
				{
					// Si un opérateur se déconnecte, on le supprime de la sélection de socket, on ferme
					// la connexion et on le supprime de la liste des opérateurs.
					selection->removeSocket(*it);
					delete *it;
					vector<Socket*>::iterator toDel;
					for(vector<Socket*>::iterator it2 = allOperator.begin(); it2 != allOperator.end(); ++it2)
					{
						if(*it2 == *it)
						{
							toDel = it2;
							break;
						}
					}
					allOperator.erase(toDel);
				}
			}
		}

		// S'il y a des messages à envoyer aux opérateurs, on les envoie.
		if(this->fOperatorMessage.size() != 0)
		{
			this->sendOperatorMessage(allOperator);
		}

		Sleep(1);
	}

	//On fait le ménage...
	delete serverSocket;
	delete acceptOperator;
	delete selection;

	// On envoie le code de déconnexion aux opérateurs.
	short code207 = 207;
	for(vector<Socket*>::iterator it = allOperator.begin(); it != allOperator.end(); ++it)
	{
		(*it)->sending(&code207, sizeof(short));
		delete *it;
	}
}

/**
	Cette méthode se charge d'envoyer les messages des clients vers les
	opérateurs.
	@param [in] allOperator La liste des opérateurs.
*/
void Server::sendOperatorMessage(vector<Socket*>& allOperator)
{
	//On regarde dans la file le code à envoyer.
	GeneralProtocol::CodeProtocol code = this->fOperatorMessage.back();
	this->fOperatorMessage.pop_back();
	//et on envoie le message.
	switch(code)
	{
	case GeneralProtocol::CODE203:
		{
			EntierLong temp = this->lastEntierLong.back();
			this->lastEntierLong.pop_back();
			for(vector<Socket*>::iterator it = allOperator.begin(); it != allOperator.end(); ++it)
			{
				OperatorProtocol::sendAddingNumberOfPossibilityTested(temp, *it);
			}
			break;
		}
	case GeneralProtocol::CODE205:
		{
			short code205 = 205;
			for(vector<Socket*>::iterator it = allOperator.begin(); it != allOperator.end(); ++it)
			{
				(*it)->sending(&code205, sizeof(short));
				GeneralProtocol::sendNumberOfClient(this->nbClient, *it);
			}
			break;
		}
	case GeneralProtocol::CODE206:
		{
			short code206 = 206;
			for(vector<Socket*>::iterator it = allOperator.begin(); it != allOperator.end(); ++it)
			{
				(*it)->sending(&code206, sizeof(short));
				GeneralProtocol::sendString(*((std::string*)this->answer), *it);
			}
			break;
		}
	case GeneralProtocol::CODE208:
		{
			for(vector<Socket*>::iterator it = allOperator.begin(); it != allOperator.end(); ++it)
			{
				OperatorProtocol::sendResultNotFound(*it);
			}
			break;
		}
	}

}

/**
	Cette méthode se charge de recevoir un message d'un opérateur et
	de le traiter.
	@param [in] socket Le socket de l'opérateur
	@param [in] selection La sélection de socket dont fait partie le socket
	@param [in] allOperator La liste de tous les opérateurs
*/
void Server::receiveOperatorMessage(Socket* socket, SocketSelection* selection, vector<Socket*>& allOperator)
{
	//On reçoit l'informations de l'opérateur et on lui répond.
	GeneralProtocol::CodeProtocol code = GeneralProtocol::receiveData(socket);
	switch(code)
	{
	case GeneralProtocol::CODE101:
		this->receiveCODE101(socket);
		std::cout << "Demande de recherche exhaustive recu." << std::endl;
		break;
	case GeneralProtocol::CODE102:
		OperatorProtocol::send102Response(socket);
		this->stopDispatch();
		std::cout << "Demande d'arrete de la recherche exhaustive recu." << std::endl;
		break;
	case GeneralProtocol::CODE103:
		NULL;
		break;
	case GeneralProtocol::CODE104:
		std::cout << "Demande d'information sur la recherche exhaustive recu." << std::endl;
		this->sendAnswerToCODE104(socket);
		break;
	case GeneralProtocol::CODE105:
		std::cout << "Demande du nombre de client recu." << std::endl;
		OperatorProtocol::sendNumberOfClient(this->nbClient, socket);
		break;
	case GeneralProtocol::CODE106:
		std::cout << "Un operateur vient de se déconnecter." << std::endl;
		//On suprime la socket du serveur.
		selection->removeSocket(socket);
		vector<Socket*>::iterator del;
		for(vector<Socket*>::iterator op = allOperator.begin(); op != allOperator.end(); ++op)
		{
			if(socket == *op)
			{
				del = op;
			}
		}
		allOperator.erase(del);
		delete socket;
		break;
	}
}

/**
	Cette méthode se charge de recevoir les données du code 101 et
	de lui répondre.
	@param [in] socket Le socket de réception et d'envoie
*/
void Server::receiveCODE101(Socket* socket)
{
	//On regarde si une tâche n'est pas déjà commencé.
	if(this->dispatcher == NULL)
	{
		//On réinitialise l'algo
		if(this->algo != NULL)
		{
			delete this->algo;
			this->algo = NULL;
		}

		//Code #1
		if(this->letters != NULL)
		{
			delete this->letters;
			this->letters = NULL;
		}
		this->letters = new std::string;
		if(this->saltBefore != NULL)
		{
			delete this->saltBefore;
			this->saltBefore = NULL;
		}
		this->saltBefore = new std::string;
		if(this->saltAfter != NULL)
		{
			delete this->saltAfter;
			this->saltAfter = NULL;
		}
		this->saltAfter = new std::string;
		GeneralProtocol::receiveExhaustiveSearchLettersAndSalts(*((std::string*)this->letters), *((std::string*)this->saltBefore), *((std::string*)this->saltAfter), socket);

		//Code #2
		short code2;
		socket->receiving(&code2, sizeof(short));

		this->type = GeneralProtocol::receiveAlgoType(socket);
		switch(this->type)
		{
		case Algorithm::MD5:
			{
				//On reçoit les paramètre pour une recherche en MD5
				MD5Search md5 = GeneralProtocol::receiveOperatorExhaustiveMD5SearchSettings(socket);
				this->dispatcher = new Dispatcher(md5.min, md5.max, md5.nbClient * 1000000, (*(std::string*)this->letters), (*(std::string*)this->saltBefore), (*(std::string*)this->saltAfter));
				this->algo = new MD5(md5.hash);
				break;
			}
		case Algorithm::SHA1:
			{
				//On reçoit les paramètre pour une recherche en SHA-1
				SHA1Search sha1 = GeneralProtocol::receiveOperatorExhaustiveSHA1SearchSettings(socket);
				this->dispatcher = new Dispatcher(sha1.min, sha1.max, sha1.nbClient * 1000000, (*(std::string*)this->letters), (*(std::string*)this->saltBefore), (*(std::string*)this->saltAfter));
				this->algo = new SHA1(sha1.hash);
				break;
			}
		}

		//On envoie l'acceptation du client.
		OperatorProtocol::sendAcceptation(true, this->nbClient, socket);

		this->clients->beginDispatching();
	}
	else //On reçoit quand même tout mais pour le jeter à la "poubelle"
	{
		//Code #1
		std::string strTemp;
		GeneralProtocol::receiveExhaustiveSearchLettersAndSalts(strTemp, strTemp, strTemp, socket);

		//Code #2
		Algorithm::TypeAlgo temp = GeneralProtocol::receiveAlgoType(socket);
		switch(temp)
		{
		case Algorithm::MD5:
			//On reçoit les paramètre pour une recherche en MD5 et on les "jette".
			GeneralProtocol::receiveOperatorExhaustiveMD5SearchSettings(socket);
			break;
		case Algorithm::SHA1:
			//On reçoit les paramètre pour une recherche en SHA-1 et on les "jette".
			GeneralProtocol::receiveOperatorExhaustiveSHA1SearchSettings(socket);
			break;
		}

		//On envoie le refus du client.
		OperatorProtocol::sendAcceptation(false, 0, socket);
	}
}

/**
	Cette méthode se charge d'envoyer les données en réponse au
	code 104.
	@param [in] socket Le socket de réception et d'envoie
*/
void Server::sendAnswerToCODE104(Socket* socket)
{
	short code204 = 204;
	socket->sending(&code204, sizeof(short));

	//On reçoit les lettres faisant parties de la recherche exhaustive
	GeneralProtocol::sendExhaustiveSearchLettersAndSalts((*(std::string*)this->letters), (*(std::string*)this->saltBefore), (*(std::string*)this->saltAfter), socket);
	if(this->type == Algorithm::MD5)
	{
		//On envoie les données de la recherche actuelle
		MD5Search md5;
		unsigned char* temp = ((Algorithm*)this->algo)->getHash();
		for(int i = 0; i < 16; ++i)
		{
			md5.hash[i] = temp[i];
		}
		md5.min = ((Dispatcher*)this->dispatcher)->getMinCharacter();
		md5.min = ((Dispatcher*)this->dispatcher)->getMaxCharacter();
		md5.nbClient = ((Dispatcher*)this->dispatcher)->getNbEssaisParDispatch() / 1000000;
		GeneralProtocol::sendOperatorExhaustiveMD5SearchSettings(md5, socket);
	}
	else if(this->type == Algorithm::SHA1)
	{
		//On envoie les données de la recherche actuelle
		SHA1Search sha1;
		unsigned char* temp = ((Algorithm*)algo)->getHash();
		for(int i = 0; i < 20; ++i)
		{
			sha1.hash[i] = temp[i];
		}
		sha1.min = ((Dispatcher*)this->dispatcher)->getMinCharacter();
		sha1.min = ((Dispatcher*)this->dispatcher)->getMaxCharacter();
		sha1.nbClient = ((Dispatcher*)this->dispatcher)->getNbEssaisParDispatch() / 1000000;
		GeneralProtocol::sendOperatorExhaustiveSHA1SearchSettings(sha1, socket);
	}
	
	//On envoie le nombre de client.
	GeneralProtocol::sendNumberOfClient(this->nbClient, socket);
}

/**
	Cette méthode se charge de donner un nouveau Dispatch.
	@return Retourne un nouveau Dispatch.
*/
Dispatch Server::getDispatch()
{
	assert(this->dispatcher != NULL);

	return ((Dispatcher*)this->dispatcher)->getDispatch();
}

/**
	Cette méthode indique au serveur qu'un client a trouvé la réponse
	à la recherche exhaustive.
	@param [in] mot La réponse de la recherche.
	@return Retourne vrai si la réponse est bel et bien cohérente; sinon faux.
*/
bool Server::stopDispatch(std::string mot)
{
	//On effectue la vérification de la réponse
	bool retour = ((Algorithm*)this->algo)->compare(mot);
	if(retour) //Si la réponse est bonne...
	{
		std::cout << "Reponse trouve: " << mot << std::endl;
		//On arrête tous les clients
		this->stopDispatch();
		(*(std::string*)this->answer) = mot;
		this->fOperatorMessage.push_front(GeneralProtocol::CODE206);
	}

	return retour;
}

/**
	Cette méthode indique au client d'arrêter la recherche exhaustive.
*/
void Server::stopDispatch()
{
	//On arrête tous les clients
	this->clients->stopDispatch();
	if(this->dispatcher != NULL)
	{
		delete this->dispatcher;
		this->dispatcher = NULL;
	}
}

/**
	Cette méthode se charge d'arrêter le serveur.
*/
void Server::stopServer()
{
	this->stopDispatch();
	this->stop = true;
}

/**
	Cette méthode retourne vrai si une tâche est donnée; sinon faux.
	@return Retourne vrai si une tâche est donnée; sinon faux.
*/
bool Server::isBegin()
{
	return this->dispatcher != NULL;
}

/**
	Cette méthode se charge d'ajouter un nombre de possibilité testé
	au nombre total de possibilité testé. Elle en avertit également les
	opérateurs.
	@param [in] number Le nombre de possibilité
*/
void Server::addPossibilityTested(EntierLong number)
{
	//On augemente le nombre de possibilité total testée
	WholeNumber temp = *((WholeNumber*)this->nbEssai);
	delete this->nbEssai;
	this->nbEssai = new WholeNumber(temp + number);

	//On avertis le thread du serveur pour en avertir les clients.
	this->lastEntierLong.push_front(number);
	this->fOperatorMessage.push_front(GeneralProtocol::CODE203);
}

/**
	Cette méthode se charge d'indiquer au serveur qu'un client
	vient de se connecter.
*/
void Server::addClient()
{
	this->nbClient++;
	//On avertis le thread du serveur pour en avertir les clients.
	this->fOperatorMessage.push_front(GeneralProtocol::CODE205);
}

/**
	Cette méthode se charge d'indiquer au serveur qu'un client
	vient de se déconnecter.
*/
void Server::removeClient()
{
	this->nbClient--;
	//On avertis le thread du serveur pour en avertir les clients.
	this->fOperatorMessage.push_front(GeneralProtocol::CODE205);
}

/**
	Cette méthode se charge de dire au serveur que la recherche n'a
	rien donné.
*/
void Server::resultNotFound()
{
	this->stopDispatch();
	this->fOperatorMessage.push_front(GeneralProtocol::CODE208);
}

/**
	Cette méthode retourne le type de hash utilisé par le serveur.
	@return Retourne le type de hash utilisé par le serveur.
*/
Algorithm::TypeAlgo Server::getTypeAlgo()
{
	return this->type;
}

/**
	Cette méthode retourne les lettres utilisées dans la recherche exhaustive.
	@return Retourne les lettres utilisées dans la recherche exhaustive.
*/
std::string Server::getLetters()
{
	return (*(string*)this->letters);
}

std::string Server::getSaltBefore()
{
	return (*(string*)this->saltBefore);
}

std::string Server::getSaltAfter()
{
	return (*(string*)this->saltAfter);
}
/**
	Cette méthode retourne l'Algorithm utilisé par le serveur.
	@return Retourne l'Algorithm utilisé par le serveur.
*/
Algorithm* Server::getAlgo()
{
	return (Algorithm*)this->algo;
}

/**
	Cette méthode retourne le nombre maximal de caractère dans la
	recherche exhaustive.
	@return Retourne le nombre maximal de caractère dans la	recherche 
	exhaustive.
*/
int Server::getSearchMaxCharacter()
{
	assert(this->dispatcher != NULL);

	return ((Dispatcher*)dispatcher)->getMaxCharacter();
}