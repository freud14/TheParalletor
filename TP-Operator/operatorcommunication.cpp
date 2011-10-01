/**
    @file   OperatorCommunication.cpp
            Fichier contenant l'impl�mentation de la classe OperatorCommunication
    @author Fr�d�rik Paradis
    @author Louis-�tienne Dorval
    @date   16 mai 2011
*/

#include "operatorcommunication.h"
#include "GeneralProtocol.h"
#include "BigInteger/BigIntegerLibrary.hh"

class Operator;

/**
    M�thode qui envoit le code 101 qui indique de commencer la recherche
    @param [in] _socket La socket qui sera utilis�
*/
void OperatorCommunication::sendInitiate101(Socket* _socket)
{
    short code101 = 101;
    _socket->sending(&code101, sizeof(short));

    GeneralProtocol::sendExhaustiveSearchLetters(Operator::getInstance()->range.toStdString(), _socket);
    GeneralProtocol::sendSalt(Operator::getInstance()->saltBefore.toStdString(), Operator::getInstance()->saltAfter.toStdString(), _socket);
    switch(Operator::getInstance()->hashType)
    {
    case Algorithm::MD5:
        GeneralProtocol::sendOperatorExhaustiveMD5SearchSettings(Operator::getInstance()->md5Setting, _socket);
        break;
    case Algorithm::SHA1:
        GeneralProtocol::sendOperatorExhaustiveSHA1SearchSettings(Operator::getInstance()->sha1Setting, _socket);
        break;
    }
}

/**
    M�thode qui envoit le code 102 qui indique de stopper la recherche
    @param [in] _socket La socket qui sera utilis�
*/
void OperatorCommunication::sendStop102(Socket* _socket)
{
    short code102 = 102;
    _socket->sending(&code102, sizeof(short));
}

/**
    M�thode qui envoit le code 103 qui indique la r�ception du code 203
    @param [in] _socket La socket qui sera utilis�
*/
void OperatorCommunication::sendConfirm103(Socket* _socket)
{
    short code103 = 103;
    _socket->sending(&code103, sizeof(short));
}

/**
    M�thode qui envoit le code 104 qui demande de mettre � jour toutes les informations de l'op�rateur
    @param [in] _socket La socket qui sera utilis�
*/
void OperatorCommunication::sendUpdate104(Socket* _socket)
{
    short code104 = 104;
    _socket->sending(&code104, sizeof(short));
}

/**
    M�thode qui envoit le code 105 qui demande de mettre � jour le nombre de clients connect�s
    @param [in] _socket La socket qui sera utilis�
*/
void OperatorCommunication::sendUpdate105(Socket* _socket)
{
    short code105 = 105;
    _socket->sending(&code105, sizeof(short));
}

/**
    M�thode qui envoit le code 106 qui indique que l'op�rateur se d�connecte, il peut suivre la r�ception du code 207
    @param [in] _socket La socket qui sera utilis�
*/
void OperatorCommunication::sendDisconnect106(Socket* _socket)
{
    short code106 = 106;
    _socket->sending(&code106, sizeof(short));
}

/**
    M�thode qui recoit le code 201 qui indique que l'op�rateur est accept� et informe du nombre de clients connect�s
	@param [out] _isAccepted Sert � retourner le bool�en d'acceptation.
	@param [out] _nbClient Sert � retourner le nombre de client.
    @param [in] _socket La socket qui sera utilis�
*/
void OperatorCommunication::receiveCODE201(bool &_isAccepted, short &_nbClient, Socket* _socket)
{
    _socket->receiving(&_isAccepted, sizeof(bool));
    if(_isAccepted == true)
    {
        _nbClient = GeneralProtocol::receiveNumberClient(_socket);
    }
}

/**
    M�thode qui recoit le code 203 qui indique l'avancement de la recherche depuis le dernier code 203
    @param [in] _socket La socket qui sera utilis�
    @return Retourne un EntierLong qui indique le nombre de possibilit�s depuis le dernier code 203
*/
EntierLong OperatorCommunication::receiveCODE203(Socket* _socket)
{
    EntierLong retour;
    _socket->receiving(&retour, sizeof(EntierLong));

    return retour;
}

/**
    M�thode qui recoit le code 204 qui informe l'op�rateur sur toutes les informations qui � demander avec le code 104
    @param [in] _socket La socket qui sera utilis�
*/
void OperatorCommunication::receiveCODE204(Socket* _socket)
{
    std::string range = GeneralProtocol::receiveExhaustiveSearchLetters(_socket); //CODE 1
    MD5Search md5Set;
    SHA1Search sha1Set;
    switch(GeneralProtocol::receiveAlgoType(_socket)) //CODE 2
    {
    case Algorithm::MD5:
        md5Set = GeneralProtocol::receiveOperatorExhaustiveMD5SearchSettings(_socket);
        break;
    case Algorithm::SHA1:
        sha1Set = GeneralProtocol::receiveOperatorExhaustiveSHA1SearchSettings(_socket);
        break;
    }
    short nbClients = GeneralProtocol::receiveNumberClient(_socket); //CODE 3
    std::string numberTested = GeneralProtocol::receiveString(_socket);
    BigInteger bigTested = stringToBigInteger(numberTested);
    //==============================================================
    //            UPDATE ALL INFORMATION Progression Window
    //==============================================================
}
/**
    M�thode qui recoit le code 205 qui indique le nombre de clients connect�s
    @param [in] _socket La socket qui sera utilis�
    @return Le nombre de clients connect�s
*/
short OperatorCommunication::receiveCODE205(Socket* _socket)
{
    short nbClients = GeneralProtocol::receiveNumberClient(_socket); //CODE 3
    return nbClients;
}

/**
    M�thode qui recoit le code 206 qui indique que la recherche � port� fruit
    @param [in] _socket La socket qui sera utilis�
    @return Le mot de passe cherch�
*/
std::string OperatorCommunication::receiveCODE206(Socket* _socket)
{
    std::string finalHash = GeneralProtocol::receiveString(_socket);
    return finalHash;
}


