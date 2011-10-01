/**
    @file   invalidsocket.cpp
            Fichier contenant l'impl�mentation de l'exception InvalidSocket
    @author Fr�d�rik Paradis
    @author Louis-�tienne Dorval
    @date   16 mai 2011
*/

#include "invalidsocket.h"

/**
    Impl�mentation de la m�thode what() de Exception
*/
const char* InvalidSocket::what() const throw()
{
    return "Il y a eu une erreur lors de la connection. Veuillez v�rifier vos informations et r�essayer";
}
