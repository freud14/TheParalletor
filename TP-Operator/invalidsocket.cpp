/**
    @file   invalidsocket.cpp
            Fichier contenant l'implémentation de l'exception InvalidSocket
    @author Frédérik Paradis
    @author Louis-Étienne Dorval
    @date   16 mai 2011
*/

#include "invalidsocket.h"

/**
    Implémentation de la méthode what() de Exception
*/
const char* InvalidSocket::what() const throw()
{
    return "Il y a eu une erreur lors de la connection. Veuillez vérifier vos informations et réessayer";
}
