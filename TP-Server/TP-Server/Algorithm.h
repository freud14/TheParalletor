/**
	@file	Algorithm.h
			Fichier contenant la d�claration de la classe Algorithm.
	@author Fr�d�rik Paradis
	@author Louis-�tienne Dorval
	@date   16 mai 2011
*/

#pragma once
#include <cassert>
#include <string>

/**
	La classe Algorithm permet d'offrir une classe
	abstraite afin d'implmenter n'importe quel algorithm
	de hachage.
*/
class Algorithm
{
public:
	/**
		Repr�sente le type d'algorithme utilis�
	*/
	enum TypeAlgo {MD5, SHA1};
	Algorithm(unsigned char* hash, int taille);
	virtual ~Algorithm();
	/**
		Cette fonction doit �tre r�impl�ment� pour pouvoir comparer un hash sous
		forme binaire avec une chaine de caract�re que l'on doit hach�.
		@param [in] str La chaine de caract�re
		@return Retourne vrai si le r�sultat de la comparaison est le pareil; sinon faux.
	*/
	virtual bool compare(std::string str) = 0;
	static unsigned char* hashToHex(std::string mot);
	unsigned char* getHash();
protected:
	/**
		Cette propri�t� permet de stocker le hash passer par l'utilisateur.
		Normalement, elle devrait avoir une longueur de <code>taille</code>.
	*/
	unsigned char* hash;

	/**
		La longueur du hash.
	*/
	int taille;
};

