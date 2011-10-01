#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h> //#pragma comment(lib, "ws2_32.lib")
#include <Windows.h>
#include <process.h>
#include <iostream>
#include <string>
#include <tchar.h>
#include <signal.h>
#include "Server.h"

BOOL WINAPI ConsoleHandler(DWORD dwCtrlType);

int main(int argc, char* argv[])
{
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2,2), &WSAData);

	SetConsoleCtrlHandler( (PHANDLER_ROUTINE)ConsoleHandler, true);

	//std::string letters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	Server::createInstance(1337, 7331);
	Server::getInstance()->startClientManager();
	Server::getInstance()->execute();

	system("pause");

	WSACleanup();
	return 0;
}

BOOL WINAPI ConsoleHandler(DWORD CEvent)
{
	switch(CEvent)
	{
	case CTRL_C_EVENT:
		Server::getInstance()->stopServer();
		break;
	case CTRL_BREAK_EVENT:
		Server::getInstance()->stopServer();
		break;
	case CTRL_CLOSE_EVENT:
		Server::getInstance()->stopServer();
		break;
	case CTRL_LOGOFF_EVENT:
		Server::getInstance()->stopServer();
		break;
	case CTRL_SHUTDOWN_EVENT:
		Server::getInstance()->stopServer();
		break;

	}
	return TRUE;
}