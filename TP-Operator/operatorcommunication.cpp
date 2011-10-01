/**
    @file   OperatorCommunication.cpp
            Fichier contenant l'implémentation de la classe OperatorCommunication
    @author Frédérik Paradis
    @author Louis-Étienne Dorval
    @date   16 mai 2011
*/

#include "operatorcommunication.h"
#include "GeneralProtocol.h"
#include "BigInteger/BigIntegerLibrary.hh"

class Operator;

/**
    Méthode qui envoit le code 101 qui indique de commencer la recherche
    @param [in] _socket La socket qui sera utilisé
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
    Méthode qui envoit le code 102 qui indique de stopper la recherche
    @param [in] _socket La socket qui sera utilisé
*/
void OperatorCommunication::sendStop102(Socket* _socket)
{
    short code102 = 102;
    _socket->sending(&code102, sizeof(short));
}

/**
    Méthode qui envoit le code 103 qui indique la réception du code 203
    @param [in] _socket La socket qui sera utilisé
*/
void OperatorCommunication::sendConfirm103(Socket* _socket)
{
    short code103 = 103;
    _socket->sending(&code103, sizeof(short));
}

/**
    Méthode qui envoit le code 104 qui demande de mettre à jour toutes les informations de l'opérateur
    @param [in] _socket La socket qui sera utilisé
*/
void OperatorCommunication::sendUpdate104(Socket* _socket)
{
    short code104 = 104;
    _socket->sending(&code104, sizeof(short));
}

/**
    Méthode qui envoit le code 105 qui demande de mettre à jour le nombre de clients connectés
    @param [in] _socket La socket qui sera utilisé
*/
void OperatorCommunication::sendUpdate105(Socket* _socket)
{
    short code105 = 105;
    _socket->sending(&code105, sizeof(short));
}

/**
    Méthode qui envoit le code 106 qui indique que l'opérateur se déconnecte, il peut suivre la réception du code 207
    @param [in] _socket La socket qui sera utilisé
*/
void OperatorCommunication::sendDisconnect106(Socket* _socket)
{
    short code106 = 106;
    _socket->sending(&code106, sizeof(short));
}

/**
    Méthode qui recoit le code 201 qui indique que l'opérateur est accepté et informe du nombre de clients connectés
	@param [out] _isAccepted Sert à retourner le booléen d'acceptation.
	@param [out] _nbClient Sert à retourner le nombre de client.
    @param [in] _socket La socket qui sera utilisé
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
    Méthode qui recoit le code 203 qui indique l'avancement de la recherche depuis le dernier code 203
    @param [in] _socket La socket qui sera utilisé
    @return Retourne un EntierLong qui indique le nombre de possibilités depuis le dernier code 203
*/
EntierLong OperatorCommunication::receiveCODE203(Socket* _socket)
{
    EntierLong retour;
    _socket->receiving(&retour, sizeof(EntierLong));

    return retour;
}

/**
    Méthode qui recoit le code 204 qui informe l'opérateur sur toutes les informations qui à demander avec le code 104
    @param [in] _socket La socket qui sera utilisé
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
    Méthode qui recoit le code 205 qui indique le nombre de clients connectés
    @param [in] _socket La socket qui sera utilisé
    @return Le nombre de clients connectés
*/
short OperatorCommunication::receiveCODE205(Socket* _socket)
{
    short nbClients = GeneralProtocol::receiveNumberClient(_socket); //CODE 3
    return nbClients;
}

/**
    Méthode qui recoit le code 206 qui indique que la recherche à porté fruit
    @param [in] _socket La socket qui sera utilisé
    @return Le mot de passe cherché
*/
std::string OperatorCommunication::receiveCODE206(Socket* _socket)
{
    std::string finalHash = GeneralProtocol::receiveString(_socket);
    return finalHash;
}


