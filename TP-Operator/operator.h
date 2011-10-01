/**
        @file	Operator.h
                        Fichier contenant la déclaration de la classe Operator
        @author Frédérik Paradis
        @author Louis-Étienne Dorval
        @date   16 mai 2011
*/

#ifndef OPERATOR_H
#define OPERATOR_H
#include <QString>
#include <assert.h>
#include "Socket.h"
#include "Algorithm.h"
#include "MD5.h"
#include "SHA1.h"
#include "progressionwindowoperator.h"
#include "BigInteger/BigIntegerLibrary.hh"
#include "MultiTaches/Thread.h"
#include "MultiTaches/DequeSafe.h"
#include "GeneralProtocol.h"

typedef unsigned long long EntierLong;


/**
    Cette classe hérite de Thread et gère toute l'interaction entre les autres
    classes.
*/
class Operator : public Thread
{
public:
    static void createInstance(QString _ip, unsigned short _port, QString _range, short _hashType, QString _hash, short _minChar, short _maxChar, short _quota, QString _saltBefore, QString _saltAfter);
    static Operator* getInstance();
    DWORD execute();
    friend class OperatorCommunication;
    void askCancel();
    std::string calcProb(std::string _range, short _minChar, short _maxChar);
    BigInteger powBig(int base, int exponent);
    ~Operator();
private:
    Operator(QString _ip, unsigned short _port, QString _range, short _hashType, QString _hash, short _minChar, short _maxChar, short _quota, QString _saltBefore, QString _saltAfter);
    unsigned char* hashToHex(std::string mot);

    QString ip;
    unsigned short port;
    QString range;
    Algorithm::TypeAlgo hashType;
    MD5Search md5Setting;
    SHA1Search sha1Setting;
    QString hashString;
    unsigned char* hashHex;
    short quota;
    QString saltBefore;
    QString saltAfter;
    Socket* sock;
    static volatile Operator* instance;
    volatile bool stop;
    DequeSafe<GeneralProtocol::CodeProtocol> fMessage;
};

#endif // OPERATOR_H
