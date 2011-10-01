/**
        @file	invalidsocket.h
                        Fichier contenant la déclaration de l'exception InvalidSocket
        @author Frédérik Paradis
        @author Louis-Étienne Dorval
        @date   16 mai 2011
*/

#ifndef INVALIDSOCKET_H
#define INVALIDSOCKET_H

#include <exception>

/**
    Cette classe hérite de std::exception.
    Cette exception est utilisé lorsqu'il y a une erreur lors de la connection du socket
*/
class InvalidSocket : public std::exception
{
public:
    const char* what() const throw();
};

#endif // INVALIDSOCKET_H
