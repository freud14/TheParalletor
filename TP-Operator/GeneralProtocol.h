/**
	@file	GeneralProtocol.h
			Fichier contenant la d�claration de la classe GeneralProtocol.
	@author Fr�d�rik Paradis
	@author Louis-�tienne Dorval
	@date   16 mai 2011
*/

#pragma once
#include <string>
#include "Socket.h"
#include "MD5.h"
#include "SHA1.h"

/**
	La classe GeneralProtocol est une classe utilitaire qui
	permet d'impl�menter la partie g�n�rale du protocole
	cr�� pour ce programme. Il n'y a donc que des m�thodes static
	qui en font parties.
*/
class GeneralProtocol
{
public:
	/**
		Cette enum�ration sert � indiquer le code re�u via le r�seau.
	*/
	enum CodeProtocol { CODE1, CODE2, CODE3, CODE101, CODE102, CODE103, CODE104, CODE105, CODE106, CODE201, CODE202, CODE203, CODE204, CODE205, CODE206, CODE207, CODE208, CODE301, CODE302, CODE303, CODE304};
	static CodeProtocol receiveData(Socket* socket);

	static std::string receiveExhaustiveSearchLetters(Socket* socket);
	static Algorithm::TypeAlgo receiveAlgoType(Socket* socket);
	static MD5Search receiveOperatorExhaustiveMD5SearchSettings(Socket* socket);
	static SHA1Search receiveOperatorExhaustiveSHA1SearchSettings(Socket* socket);

	static void sendExhaustiveSearchLetters(std::string letters, Socket* socket);
	static void sendOperatorExhaustiveMD5SearchSettings(MD5Search md5, Socket* socket);
	static void sendOperatorExhaustiveSHA1SearchSettings(SHA1Search sha1, Socket* socket);
	static void sendNumberOfClient(short nb, Socket* socket);
        static void sendSalt(std::string saltBefore, std::string saltAfter, Socket* socket);

	static void sendString(const std::string& str, Socket* socket);

        static short receiveNumberClient(Socket* socket);
        static std::string receiveString(Socket* socket);
private:
	GeneralProtocol(void);
	~GeneralProtocol(void);
};

