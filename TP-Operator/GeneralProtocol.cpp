/**
    @file   GeneralProtocol.cpp
            Fichier contenant l'implémentation de la classe GeneralProtocol.
    @author Frédérik Paradis
    @author Louis-Étienne Dorval
    @date   16 mai 2011
*/

#include "GeneralProtocol.h"

/**
    Cette méthode se charge de recevoir le code du client.
    @param [in] socket La socket de réception
    @return Retourne le code que le serveur a envoyé.
*/
GeneralProtocol::CodeProtocol GeneralProtocol::receiveData(Socket* socket)
{
    CodeProtocol retour;
    short resultat;
    socket->receiving(&resultat, sizeof(short));

    switch(resultat)
    {
    case 1:
        retour = GeneralProtocol::CODE1;
        break;
    case 2:
        retour = GeneralProtocol::CODE2;
        break;
    case 3:
        retour = GeneralProtocol::CODE3;
        break;
    case 101:
        retour = GeneralProtocol::CODE101;
        break;
    case 102:
        retour = GeneralProtocol::CODE102;
        break;
    case 103:
        retour = GeneralProtocol::CODE103;
        break;
    case 104:
        retour = GeneralProtocol::CODE104;
        break;
    case 105:
        retour = GeneralProtocol::CODE105;
        break;
    case 201:
        retour = GeneralProtocol::CODE201;
        break;
    case 202:
        retour = GeneralProtocol::CODE202;
        break;
    case 203:
        retour = GeneralProtocol::CODE203;
        break;
    case 204:
        retour = GeneralProtocol::CODE204;
        break;
    case 205:
        retour = GeneralProtocol::CODE205;
        break;
    case 206:
        retour = GeneralProtocol::CODE206;
        break;
    case 207:
        retour = GeneralProtocol::CODE207;
        break;
    case 208:
        retour = GeneralProtocol::CODE208;
        break;
    case 301:
        retour = GeneralProtocol::CODE301;
        break;
    case 302:
        retour = GeneralProtocol::CODE302;
        break;
    case 303:
        retour = GeneralProtocol::CODE303;
        break;
    }

    return retour;
}

/**
    Cette méthode se charge de recevoir le code #1 c'est-à-dire les lettres à
    tester dans la recherche exhaustive.
    @param [in] socket La socket de réception
    @return Retourne les lettres voulu dans l'algorithme de recherche exhaustive.
*/
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


/**
    Cette méthode se charge de recevoir le type d'algorithme envoyé par
    le serveur. Rappelons que le serveur doit envoyer 1 pour avoir l'algorithme
    MD5 et 2 pour l'algorithme SHA-1.
    @param [in] socket La socket de réception
    @return Retourne le type d'algorithme.
*/
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
        retour = Algorithm::MD5;
        break;
    case 2:
        retour = Algorithm::SHA1;
        break;
    }

    return retour;
}



/**
    Cette méthode se charge de recevoir les paramètres d'une
    recherche exhaustive sur un hash MD5.
    @param [in] socket La socket de réception
    @return Retourne les paramètres de la recherche exhaustive sur un hash MD5.
*/
MD5Search GeneralProtocol::receiveOperatorExhaustiveMD5SearchSettings(Socket* socket)
{
    MD5Search md5;
    socket->receiving(&md5, sizeof(MD5Search));

    return md5;
}

/**
    Cette méthode se charge de recevoir les paramètres d'une
    recherche exhaustive sur un hash SHA-1.
    @param [in] socket La socket de réception
    @return Retourne les paramètres de la recherche exhaustive sur un hash SHA-1.
*/
SHA1Search GeneralProtocol::receiveOperatorExhaustiveSHA1SearchSettings(Socket* socket)
{
    SHA1Search sha1;
    socket->receiving(&sha1, sizeof(SHA1Search));

    return sha1;
}

/**
    Cette méthode se charge d'envoyer les lettres pour la recherche
    exhaustive.
    @param [in] letters Les lettres pour la recherche exhaustive
    @param [in] socket La socket d'envoi
*/
void GeneralProtocol::sendExhaustiveSearchLetters(std::string letters, Socket* socket)
{
    short code1 =  1;
    socket->sending(&code1, sizeof(short));

    //On envoie la string
    GeneralProtocol::sendString(letters, socket);
}

/**
    Cette méthode se charge d'envoyer les paramètres de la recherches exhaustives
    pour un hash MD5.
    @param [in] md5 Les paramètres de la recherche
    @param [in] socket La socket d'envoi
*/
void GeneralProtocol::sendOperatorExhaustiveMD5SearchSettings(MD5Search md5, Socket* socket)
{
    short code2 = 2;
    socket->sending(&code2, sizeof(short));

    short algo = 1;
    socket->sending(&algo, sizeof(short));

    socket->sending(&md5, sizeof(MD5Search));
}

/**
    Cette méthode se charge d'envoyer les paramètres de la recherches exhaustives
    pour un hash SHA-1.
    @param [in] sha1 Les paramètres de la recherche
    @param [in] socket La socket d'envoi
*/
void GeneralProtocol::sendOperatorExhaustiveSHA1SearchSettings(SHA1Search sha1, Socket* socket)
{
    short code2 = 2;
    socket->sending(&code2, sizeof(short));

    short algo = 2;
    socket->sending(&algo, sizeof(short));

    socket->sending(&sha1, sizeof(SHA1Search));
}

/**
    Cette méthode se charge d'envoyer le nombre de client
    connecté sur le serveur. Pour rappel, cette méthode envoie
    le code #1 avant d'envoyer le nombre.
    @param [in] nb Le nombre de client connecté.
    @param [in] socket La socket d'envoie.
*/
void GeneralProtocol::sendNumberOfClient(short nb, Socket* socket)
{
    short code3 = 3;
    socket->sending(&code3, sizeof(short));

    socket->sending(&nb, sizeof(short));
}

/**
    Cette méthode se charge d'envoyer une chaine de caractère.
    @param [in] str La chaine de caractère à envoyer
    @param [in] socket La socket d'envoie.
*/
void GeneralProtocol::sendString(const std::string& str, Socket* socket)
{
    short nbLettre = str.length();
    socket->sending(&nbLettre, sizeof(short));
    socket->sending((void*)str.c_str(), str.length());
}


/**
    Cette méthode se charge de la réception du nombre de client
    @param [in] socket La socket qui transmet le message
    @return Retourne le nombre de clients recu
*/
short GeneralProtocol::receiveNumberClient(Socket* socket)
{
    short code;
    short numberClients;
    socket->receiving(&code, sizeof(short));
    if(code == 3) //Réception du code #3
    {
        socket->receiving(&numberClients, sizeof(short));
    }
    return numberClients;
}

/**
    Cette méthode se charge de la réception d'un message en string
    @param [in] socket La socket qui transmet le message
    @return Retourne le message recu
*/
std::string GeneralProtocol::receiveString(Socket* socket)
{
    short length;
    socket->receiving(&length, sizeof(short));
    char* tab = new char[length +1]; //Pour ajouter le caractère null
    socket->receiving(tab, length);
    tab[length] = 0;
    std::string retour = tab;
    delete[] tab;
    return retour;
}

void GeneralProtocol::sendSalt(std::string saltBefore, std::string saltAfter, Socket* socket)
{
    short beforeLength =  saltBefore.length();
    short afterLength =  saltAfter.length();

    socket->sending(&beforeLength, sizeof(short));
    if(beforeLength != 0)
    {
        socket->sending((void*)saltBefore.c_str(), saltBefore.length());
    }

    socket->sending(&afterLength, sizeof(short));
    if(afterLength != 0)
    {
        socket->sending((void*)saltAfter.c_str(), saltAfter.length());
    }
}
