#include "GeneralProtocol.h"

GeneralProtocol::CodeProtocol GeneralProtocol::receiveData(Socket* socket)
{
	CodeProtocol retour;
	short resultat;
	socket->receiving(&resultat, sizeof(short));

	switch(resultat)
	{
	case 101:
		retour = CodeProtocol::CODE101;
		break;
	case 102:
		retour = CodeProtocol::CODE102;
		break;
	case 103:
		retour = CodeProtocol::CODE103;
		break;
	case 104:
		retour = CodeProtocol::CODE104;
		break;
	case 105:
		retour = CodeProtocol::CODE105;
		break;
	case 301:
		retour = CodeProtocol::CODE301;
		break;
	case 302:
		retour = CodeProtocol::CODE302;
		break;
	case 303:
		retour = CodeProtocol::CODE303;
		break;
	}

	return retour;
}

std::string GeneralProtocol::receiveExhaustiveSearchLetters(Socket* socket)
{
	short code1;
	socket->receiving(&code1, sizeof(short));

	//On reçoit le nombre de lettre
	short nbLetter;
	socket->receiving(&nbLetter, sizeof(short));
	
	//On reçoit la chaine de caractère.
	char* temp = new char[nbLetter + 1];
	socket->receiving(temp, nbLetter);
	temp[nbLetter] = 0;
	std::string retour = temp;
	delete[] temp;

	return retour;
}

Algorithm::TypeAlgo GeneralProtocol::receiveAlgoType(Socket* socket)
{
	short code2;
	socket->receiving(&code2, sizeof(short));

	//On reçoit le short qui représente le type de hash
	short type;
	socket->receiving(&type, sizeof(short));
	
	//On renvoie le TypeAlgo associé
	Algorithm::TypeAlgo retour;
	switch(type)
	{
	case 1:
		retour = Algorithm::TypeAlgo::MD5;
		break;
	case 2:
		retour = Algorithm::TypeAlgo::SHA1;
		break;
	}

	return retour;
}

MD5Search GeneralProtocol::receiveOperatorExhaustiveMD5SearchSettings(Socket* socket)
{
	MD5Search md5;
	socket->receiving(&md5, sizeof(MD5Search));

	return md5;
}

SHA1Search GeneralProtocol::receiveOperatorExhaustiveSHA1SearchSettings(Socket* socket)
{
	SHA1Search sha1;
	socket->receiving(&sha1, sizeof(SHA1Search));

	return sha1;
}

void GeneralProtocol::sendExhaustiveSearchLetters(std::string letters, Socket* socket)
{
	short code1 =  1;
	socket->sending(&code1, sizeof(short));

	//On envoie la string
	GeneralProtocol::sendString(letters, socket);
}

void GeneralProtocol::sendOperatorExhaustiveMD5SearchSettings(MD5Search md5, Socket* socket)
{
	short code2 = 2;
	socket->sending(&code2, sizeof(short));

	short algo = 1;
	socket->sending(&algo, sizeof(short));

	socket->sending(&md5, sizeof(MD5Search));
}

void GeneralProtocol::sendOperatorExhaustiveSHA1SearchSettings(SHA1Search sha1, Socket* socket)
{
	short code2 = 2;
	socket->sending(&code2, sizeof(short));

	short algo = 2;
	socket->sending(&algo, sizeof(short));

	socket->sending(&sha1, sizeof(SHA1Search));
}

void GeneralProtocol::sendNumberOfClient(short nb, Socket* socket)
{
	short code3 = 3;
	socket->sending(&code3, sizeof(short));

	socket->sending(&nb, sizeof(short));
}

void GeneralProtocol::sendString(const std::string& str, Socket* socket)
{
	short nbLettre = str.length();
	socket->sending(&nbLettre, sizeof(short));
	socket->sending((void*)str.c_str(), str.length());
}