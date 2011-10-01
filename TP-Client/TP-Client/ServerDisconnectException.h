#pragma once
#include <exception>

/**
	Cette exception est lancée lorsque le serveur se déconnecte.
*/
class ServerDisconnectException : public std::exception
{
public:
};

