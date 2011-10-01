/**
	@file	Server.h
			Fichier contenant la déclaration de la classe Server.
	@author Frédérik Paradis
	@author Louis-Étienne Dorval
	@date   16 mai 2011
*/

#pragma once
#include <iostream>
#include <string>
#include <cassert>
#include <DequeSafe.h>
#include "Dispatcher.h"
#include "Algorithm.h"
#include "MD5.h"
#include "SHA1.h"
#include "ClientManager.h"
#include "Socket.h"
#include "SocketSelection.h"
#include "WholeNumber.h"
#include "GeneralProtocol.h"
#include "OperatorProtocol.h"

/**
	Cette classe se charge de gérer les opérateurs.
*/
class Server
{
public:
	static void createInstance(unsigned short portOperator, unsigned short portClient);
	static Server* getInstance();

	void startClientManager();
	void execute();

	Dispatch getDispatch();

	bool stopDispatch(std::string mot);
	void stopDispatch();
	void stopServer();

	bool isBegin();

	void addPossibilityTested(EntierLong number);
	void addClient();
	void removeClient();
	void resultNotFound();

	Algorithm::TypeAlgo getTypeAlgo();
	std::string getLetters();
	std::string getSaltBefore();
	std::string getSaltAfter();
	Algorithm* getAlgo();
	int getSearchMaxCharacter();
private:
	Server(unsigned short portOperator, unsigned short portClient);
	virtual ~Server();

	void sendOperatorMessage(vector<Socket*>& allOperator);
	void receiveOperatorMessage(Socket* socket, SocketSelection* selection, vector<Socket*>& allOperator);

	void receiveCODE101(Socket* socket);
	void sendAnswerToCODE104(Socket* socket);

	static volatile Server* instance;
	
	volatile short portOperator;
	volatile short portClient;
	volatile EntierLong nbEssaisParDispatch;
	volatile std::string* letters;
	volatile std::string* saltBefore;
	volatile std::string* saltAfter;
	volatile Algorithm::TypeAlgo type;
	volatile Algorithm* algo;
	volatile Dispatcher* dispatcher;
	volatile bool stop;
	volatile short nbClient;
	volatile WholeNumber* nbEssai;
	volatile std::string* answer;

	DequeSafe<EntierLong> lastEntierLong;
	DequeSafe<GeneralProtocol::CodeProtocol> fOperatorMessage;
	
	ClientManager* clients;
};

