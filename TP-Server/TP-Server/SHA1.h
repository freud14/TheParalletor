/**
	@file	SHA1.h
			Fichier contenant la d�claration de la classe SHA1
			qui encapsule l'algorithm sha1 160 bits.
	@author Fr�d�rik Paradis
	@author Louis-�tienne Dorval
	@date   16 mai 2011
*/

#pragma once
#include <string>
#include "Algorithm.h"
#include "sha1Impl.h"

/**
	Cette structure sert � recevoir de l'op�rateur la recherche
	exhaustive � effectuer. Le hash contenu dans cette structure
	est un hash sha1 160 bits.
*/
typedef struct 
{
	unsigned char hash[20];
	unsigned char min;
	unsigned char max;
	unsigned short nbClient;
} SHA1Search;

/**
	Cette classe sert � encapsuler l'algorithme sha1 160 bits
	et permet de pouvoir comparer une cha�ne de caract�re avec
	le hash fournis par l'utilisateur.
*/
class SHA1 : public Algorithm
{
public:
	SHA1(unsigned char* hash);
	~SHA1();
	bool compare(std::string str);
};
