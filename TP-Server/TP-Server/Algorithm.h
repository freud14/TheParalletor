/**
	@file	Algorithm.h
			Fichier contenant la déclaration de la classe Algorithm.
	@author Frédérik Paradis
	@author Louis-Étienne Dorval
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
		Représente le type d'algorithme utilisé
	*/
	enum TypeAlgo {MD5, SHA1};
	Algorithm(unsigned char* hash, int taille);
	virtual ~Algorithm();
	/**
		Cette fonction doit être réimplémenté pour pouvoir comparer un hash sous
		forme binaire avec une chaine de caractère que l'on doit haché.
		@param [in] str La chaine de caractère
		@return Retourne vrai si le résultat de la comparaison est le pareil; sinon faux.
	*/
	virtual bool compare(std::string str) = 0;
	static unsigned char* hashToHex(std::string mot);
	unsigned char* getHash();
protected:
	/**
		Cette propriété permet de stocker le hash passer par l'utilisateur.
		Normalement, elle devrait avoir une longueur de <code>taille</code>.
	*/
	unsigned char* hash;

	/**
		La longueur du hash.
	*/
	int taille;
};

