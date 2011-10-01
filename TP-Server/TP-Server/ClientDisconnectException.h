#pragma once
#include <exception>

/**
	Cette exception est lancée lorsqu'un client est déconnecté
	du serveur.
*/
class ClientDisconnectException : public std::exception
{
public:
};

