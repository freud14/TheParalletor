#pragma once
#include <exception>

/**
	Cette exception est lanc�e lorsque le serveur se d�connecte.
*/
class ServerDisconnectException : public std::exception
{
public:
};

