#pragma once
#include <exception>

/**
	Cette exception est lanc�e lorsqu'un client est d�connect�
	du serveur.
*/
class ClientDisconnectException : public std::exception
{
public:
};

