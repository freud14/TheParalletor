/**
        @file	invalidsocket.h
                        Fichier contenant la d�claration de l'exception InvalidSocket
        @author Fr�d�rik Paradis
        @author Louis-�tienne Dorval
        @date   16 mai 2011
*/

#ifndef INVALIDSOCKET_H
#define INVALIDSOCKET_H

#include <exception>

/**
    Cette classe h�rite de std::exception.
    Cette exception est utilis� lorsqu'il y a une erreur lors de la connection du socket
*/
class InvalidSocket : public std::exception
{
public:
    const char* what() const throw();
};

#endif // INVALIDSOCKET_H
