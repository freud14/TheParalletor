/**
    @file   Operator.cpp
            Fichier contenant l'implémentation de la classe Operator
    @author Frédérik Paradis
    @author Louis-Étienne Dorval
    @date   16 mai 2011
*/

#include "operator.h"
#include <QMessageBox>
#include "invalidsocket.h"
#include "SocketSelection.h"
#include "GeneralProtocol.h"
#include "operatorcommunication.h"
#include <sstream>

volatile Operator* Operator::instance = NULL;


/**
  Permet de créer une nouvelle instance de la classe, méthode utilisé comme un singleton
  @param [in] _ip Adresse IP de connection
  @param [in] _port Port de connection
  @param [in] _range Les caractères de la recherche exhaustive
  @param [in] _hashType Le type de hash recherché
  @param [in] _hash Le hash recherché
  @param [in] _minChar Le nombre minimum de caractère
  @param [in] _maxChar Le nombre maximum de caractère
  @param [in] _quota Le quota par client
  */
void Operator::createInstance(QString _ip, unsigned short _port, QString _range, short _hashType, QString _hash, short _minChar, short _maxChar, short _quota, QString _saltBefore, QString _saltAfter)
{
    Operator::instance = new Operator(_ip, _port, _range, _hashType, _hash, _minChar, _maxChar, _quota, _saltBefore, _saltAfter);
}

/**
  Constructeur privée de la classe
  @param [in] _ip Adresse IP de connection
  @param [in] _port Port de connection
  @param [in] _range Les caractères de la recherche exhaustive
  @param [in] _hashType Le type de hash recherché
  @param [in] _hash Le hash recherché
  @param [in] _minChar Le nombre minimum de caractère
  @param [in] _maxChar Le nombre maximum de caractère
  @param [in] _quota Le quota par client
  */
Operator::Operator(QString _ip, unsigned short _port, QString _range, short _hashType, QString _hash, short _minChar, short _maxChar, short _quota, QString _saltBefore, QString _saltAfter)
{
    this->stop = false;
    this->ip = _ip;
    this->port = _port;
    this->range = _range;
    this->quota = _quota;
    if (_hashType == 0)
    {
        this->hashType = Algorithm::MD5;
        this->hashHex = hashToHex(_hash.toStdString());
        for(int i = 0; i < 16; i++)
        {
            this->md5Setting.hash[i] = hashHex[i];
        }
        delete[](hashHex);
        this->md5Setting.max = _maxChar;
        this->md5Setting.min = _minChar;
        this->md5Setting.nbClient = _quota;
    }
    else if (_hashType == 1)
    {
        this->hashType = Algorithm::SHA1;
        this->hashHex = hashToHex(_hash.toStdString());
        for(int i = 0; i < 20; i++)
        {
            this->sha1Setting.hash[i] = hashHex[i];
        }
        delete[](hashHex);
        this->sha1Setting.max = _maxChar;
        this->sha1Setting.min = _minChar;
        this->sha1Setting.nbClient = _quota;
    }

    this->saltBefore = _saltBefore;
    this->saltAfter = _saltAfter;

    this->sock = new Socket((char*)this->ip.toStdString().c_str(), this->port);
    if (!this->sock->connectClient())
    {
        throw InvalidSocket();
    }

}

/**
  Destructeur de la classe
  */
Operator::~Operator()
{
    delete Operator::instance;
    Operator::instance = NULL;
}

/**
  Méthode pour avoir l'instance de la classe -> Singleton
  @return L'instance de la classe
  */
Operator* Operator::getInstance()
{
    assert(Operator::instance != NULL);

    return (Operator*)Operator::instance;
}

/**
  Demande l'arrêt de la recherche exhaustive
  */
void Operator::askCancel()
{
    this->fMessage.push_front(GeneralProtocol::CODE102);
    this->stop = true;
}

/**
  Redéfinition de execute() de la classe Thread.
  Cette méthode contient utilisation de la majorité des méthodes de la classe.
  Elle permet à l'application de fonctionner, elle gère la réception et l'envoi des messages
   @return DWORD
  */
DWORD Operator::execute()
{
    SocketSelection* selection = new SocketSelection();
    selection->addSocket(this->sock);
    OperatorCommunication::sendInitiate101(this->sock);
    ProgressionWindowOperator::getInstance()->updateQuota(this->quota);
    if (hashType == Algorithm::MD5)
    {
        std::string prob = calcProb(this->range.toStdString(), this->md5Setting.min, this->md5Setting.max);
        ProgressionWindowOperator::getInstance()->updateProb((char*)prob.c_str());
    }
    else if(hashType == Algorithm::SHA1)
    {
        std::string prob = calcProb(this->range.toStdString(), this->sha1Setting.min, this->sha1Setting.max);
        ProgressionWindowOperator::getInstance()->updateProb((char*)prob.c_str());
    }

    while(!this->stop)
    {
        int ms = 0;
        std::vector<Socket*> toRead = selection->selection(&ms);
        if(toRead.size() != 0)
        {
            GeneralProtocol::CodeProtocol code = GeneralProtocol::receiveData(this->sock);
            switch(code)
            {
            case GeneralProtocol::CODE201:
                {
                    bool isAccepted;
                    short nbClients;
                    OperatorCommunication::receiveCODE201(isAccepted, nbClients, this->sock);
                    if (isAccepted)
                    {
                        ProgressionWindowOperator::getInstance()->updateNbClients(nbClients);
                    }
                    break;
                }
            case GeneralProtocol::CODE202:
                this->stop;
                break;
            case GeneralProtocol::CODE203:
                {
                    EntierLong toAdd = OperatorCommunication::receiveCODE203(this->sock);
                    stringstream ss;
                    string tempo;
                    ss << toAdd;
                    ss >> tempo;
                    ProgressionWindowOperator::getInstance()->updateTotalCalculed(tempo);
                    OperatorCommunication::sendConfirm103(this->sock);
                    break;
                }
            case GeneralProtocol::CODE204:
                OperatorCommunication::receiveCODE204(this->sock);
                //Fonction à ajouter si 204 est éventuellement utilisé
                break;
            case GeneralProtocol::CODE205:
                {
                    short nbClients = OperatorCommunication::receiveCODE205(this->sock);
                    ProgressionWindowOperator::getInstance()->updateNbClients(nbClients);
                    break;
                }
            case GeneralProtocol::CODE206:
                {
                    std::string passwordFound = OperatorCommunication::receiveCODE206(this->sock);
                    this->stop = true;
                    ProgressionWindowOperator::getInstance()->stopIsFound(passwordFound);
                    break;
                }
            case GeneralProtocol::CODE207:
                {
                    this->stop;
                    OperatorCommunication::sendDisconnect106(this->sock);
                    delete this->sock;
                    break;
                }
            case GeneralProtocol::CODE208:
                {
                    this->stop = true;
                    ProgressionWindowOperator::getInstance()->stopNotFound();
                    break;
                }
            }

        }
        Sleep(1);

        if(this->fMessage.size() != 0)
        {
            GeneralProtocol::CodeProtocol code = this->fMessage.back();
            this->fMessage.pop_back();
            switch(code)
            {
            case GeneralProtocol::CODE102:
                OperatorCommunication::sendStop102(this->sock);
                break;
            case GeneralProtocol::CODE104:
                OperatorCommunication::sendUpdate104(this->sock);
                break;
            case GeneralProtocol::CODE105:
                OperatorCommunication::sendUpdate105(this->sock);
                break;
            }
        }
    }
}

/**
     Méthode qui transforme le hash qui est en string et le rend en caractère hexadecimal
     @param [in] mot Le hash à transformer
     @return Le hash en hexadecimal
*/
unsigned char* Operator::hashToHex(std::string mot)
{
    assert(mot.length() % 2 == 0);
    unsigned char * retour = new unsigned char[mot.length() / 2];
    int i = 0;
    for(std::string::iterator it =  mot.begin(); it != mot.end(); ++it)
    {
        char tmp[3] = {*it, *(++it), 0};
        char * pEnd;
        unsigned char res;
        res = strtol (tmp,&pEnd,16);
        retour[i] = res;
        i++;
    }

    return retour;
}


/**
  Méthode qui calcule le nombre de possibilités total avec les paramètres qui sont entrés
  @param [in] _range L'ensemble de caractère de la recherche
  @param [in] _minChar Le nombre de caractère minimum
  @param [in] _maxChar Le nombre de caractère maximum
  @return Le résultat en string
*/
std::string Operator::calcProb(std::string _range, short _minChar, short _maxChar)
{
    int base = _range.size();
    BigInteger total = 0;
    for (int i = _minChar; i <= _maxChar; i++)
    {
        total += powBig(base, i);
    }
    std::string retour = bigIntegerToString(total);

    for (int i = retour.length(); i > 0 ; i -= 3)
    {
        retour.insert(i, " ");
    }
    return retour;
}

/**
  Méthode pour faire calculer un exposant avec des BigInteger
   @param [in] base La base de l'exposant
   @param [in] exponent L'exposant
   @return Le résultat en BigInteger
  */
BigInteger Operator::powBig(int base, int exponent)
{
    assert(exponent >= 0);
    BigInteger total = base;
    if (exponent != 0)
    {
        for (int i = 1; i < exponent; ++i)
        {
            total = total * base;
        }
    }
    else
    {
        total = 1;
    }
    return total;
}
