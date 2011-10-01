/**
    @file   SHA1.cpp
            Fichier contenant l'implémentation de la classe SHA1.
    @author Frédérik Paradis
    @author Louis-Étienne Dorval
    @date   16 mai 2011
*/


#include "SHA1.h"

/**
    Le constructeur initialise l'objet. Il prend le hash sha1
    en format binaire. Le tableau de char contenant le hahs
    doit avoir une taille de 20.
    @param [in] hash Le hash en format binaire d'une longueur de 20.
*/
SHA1::SHA1(unsigned char* hash) : Algorithm(hash, 20)
{

}

/**
    Libère les ressources prises par l'objet.
*/
SHA1::~SHA1()
{

}

/**
    Cette méthode compare si la string en paramètre peut être
    haché en sha1 pour obtenir la même chose que le hash de l'objet.
    @param [in] str La chaine à vérifier
    @return Retourne vrai si la chaîne haché est identique au hash; sinon faux.
*/
bool SHA1::compare(std::string str)
{
	SHA1Context context;
	SHA1Reset(&context);
	SHA1Input(&context, (const unsigned char*)str.c_str(), str.length());
	unsigned char resultat[20];
	SHA1Result(&context, resultat);

	for(int i = 0; i < 20; i++)
	{
		if(resultat[i] != this->hash[i])
		{
			return false;
		}
	}

	return true;
}
