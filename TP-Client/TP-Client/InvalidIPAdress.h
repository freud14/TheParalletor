/**
	@file	InvalidIPAdress.h
			Fichier contenant la d�claration de la classe InvalidIPAdress.
	@author Fr�d�rik Paradis
	@author Louis-�tienne Dorval
	@date   16 mai 2011
*/

#pragma once
#include <exception>

/**
	La classe InvalidIPAdress est une classe <code>exception</code>
	qui est lanc�e lorsque la connexion au serveur �choue.
*/
class InvalidIPAdress : public std::exception
{
public:
	
	/**
		Retourne une cha�ne de caract�re disant de quelle erreur il s'agit.
		@return Retourne une cha�ne de caract�re disant de quelle erreur il s'agit.
	*/
	const char* what() const throw()
	{
		return "L'adresse IP n'est pas un serveur valide";
	}
};
