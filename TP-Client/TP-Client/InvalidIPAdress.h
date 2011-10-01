/**
	@file	InvalidIPAdress.h
			Fichier contenant la déclaration de la classe InvalidIPAdress.
	@author Frédérik Paradis
	@author Louis-Étienne Dorval
	@date   16 mai 2011
*/

#pragma once
#include <exception>

/**
	La classe InvalidIPAdress est une classe <code>exception</code>
	qui est lancée lorsque la connexion au serveur échoue.
*/
class InvalidIPAdress : public std::exception
{
public:
	
	/**
		Retourne une chaîne de caractère disant de quelle erreur il s'agit.
		@return Retourne une chaîne de caractère disant de quelle erreur il s'agit.
	*/
	const char* what() const throw()
	{
		return "L'adresse IP n'est pas un serveur valide";
	}
};
