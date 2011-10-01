/**
    @file   SHA1.cpp
            Fichier contenant l'impl�mentation de la classe SHA1.
    @author Fr�d�rik Paradis
    @author Louis-�tienne Dorval
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
    Lib�re les ressources prises par l'objet.
*/
SHA1::~SHA1()
{

}

/**
    Cette m�thode compare si la string en param�tre peut �tre
    hach� en sha1 pour obtenir la m�me chose que le hash de l'objet.
    @param [in] str La chaine � v�rifier
    @return Retourne vrai si la cha�ne hach� est identique au hash; sinon faux.
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
