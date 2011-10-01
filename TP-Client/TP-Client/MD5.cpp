/**
	@file	MD5.cpp
			Fichier contenant l'implémentation de la classe MD5.
	@author Frédérik Paradis
	@author Louis-Étienne Dorval
	@date   16 mai 2011
*/

#include "MD5.h"

/**
	Le constructeur initialise l'objet. Il prend le hash MD5
	en format binaire. Le tableau de char contenant le hahs
	doit avoir une taille de 16.
	@param [in] hash Le hash en format binaire d'une longueur de 20.
*/
MD5::MD5(unsigned char* hash) : Algorithm(hash, 16)
{

}

/**
	Libère les ressources prises par l'objet.
*/
MD5::~MD5()
{

}

/**
	Cette méthode compare si la string en paramètre peut être
	haché en md5 pour obtenir la même chose que le hash de l'objet.
	@param [in] str La chaine à vérifier
	@return Retourne vrai si la chaîne haché est identique au hash; sinon faux.
*/
bool MD5::compare(std::string str)
{
	md5_state_t state;
	unsigned char resultat[16];
	md5_init(&state);
	md5_append(&state, (const unsigned char*)str.c_str(), str.length());
	md5_finish(&state, resultat);

	//On boucle pour comparer les deux hash.
	for(int i = 0; i < 16; i++)
	{
		if(resultat[i] != this->hash[i])
		{
			return false;
		}
	}

	return true;
}
