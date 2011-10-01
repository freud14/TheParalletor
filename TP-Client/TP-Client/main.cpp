#ifdef _WIN32
    #include <winsock2.h>
#endif
#include "Dispatch.h"
#include "Dispatcher.h"
#include "MD5.h"
#include <iostream>
#include <fstream>
#include <string>
#include "Socket.h"
#include "SHA1.h"
#include "Client.h"
#include "InvalidIPAdress.h"

//BOOL WINAPI ConsoleHandler(DWORD CEvent);

int main(int argc, char* argv[])
{
#ifdef _WIN32
	WSADATA WSAData;
    WSAStartup(MAKEWORD(2,2), &WSAData);
#endif

	fstream in("config.txt");
	if(in.is_open())
	{
		std::string res;
		in >> res;
	
		char* ip = new char[res.length() - 2];
		for(int i = 3; i < res.length(); i++)
		{
			ip[i - 3] = res[i];
		}
		ip[res.length() - 3] = 0;
		try
		{
			Client::createInstance(ip, 7331);
			//SetConsoleCtrlHandler( (PHANDLER_ROUTINE)ConsoleHandler, true);
			Client::getInstance()->execute();
		}
		catch(InvalidIPAdress& e)
		{
			std::cout << e.what() << std::endl;
			//system("pause");
		}
	
		delete[] ip;
	}
	else
	{
		std::cout << "Le fichier config.txt n'est pas present." << std::endl;
		//system("pause");
	}
	//system("pause");
#ifdef _WIN32
	WSACleanup();
#endif
	return 0;
}
/*
BOOL WINAPI ConsoleHandler(DWORD CEvent)
{
	switch(CEvent)
	{
	case CTRL_C_EVENT:
		Client::getInstance()->stopClient();
		break;
	case CTRL_BREAK_EVENT:
		Client::getInstance()->stopClient();
		break;
	case CTRL_CLOSE_EVENT:
		Client::getInstance()->stopClient();
		break;
	case CTRL_LOGOFF_EVENT:
		Client::getInstance()->stopClient();
		break;
	case CTRL_SHUTDOWN_EVENT:
		Client::getInstance()->stopClient();
		break;

	}
	return TRUE;
}*/

	/*int beg = ::GetTickCount();
	//SHA1* algo = new SHA1(Algorithm::hashToHex("84A516841BA77A5B4648DE2CD0DFCB30EA46DBB4")); // c
	//SHA1* algo = new SHA1(Algorithm::hashToHex("E0C9035898DD52FC65C41454CEC9C4D2611BFB37")); // aa
	//SHA1* algo = new SHA1(Algorithm::hashToHex("C8DDC7566D6FF419FC9384208ECFCD466A031EA6")); // sts
	SHA1* algo = new SHA1(Algorithm::hashToHex("C8DDC7566D6FF419FC9384208ECFCD466A031E")); //Pas bon
	Dispatch::setLetters("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
	Dispatch::setAlgorithm(algo);
	Dispatcher div("a", 10000000);
	div.doDispatch();
	//cout << *(div.doDispatch()) << endl;
	cout << ::GetTickCount() - beg << " ms" << endl;
	system("pause");*/
