/**
	@file	MD5.h
			Fichier contenant la d�claration de la classe MD5
			qui encapsule l'algorithm MD5.
	@author Fr�d�rik Paradis
	@author Louis-�tienne Dorval
	@date   16 mai 2011
*/

#pragma once
#include <string>
#include "Algorithm.h"
#include "md5Impl.h"

/**
	Cette structure sert � recevoir de l'op�rateur la recherche
	exhaustive � effectuer. Le hash contenu dans cette structure
	est un hash MD5.
*/
typedef struct 
{
	unsigned char hash[16];
	unsigned char min;
	unsigned char max;
	unsigned short nbClient;
} MD5Search;

/**
	Cette classe sert � encapsuler l'algorithme MD5
	et permet de pouvoir comparer une cha�ne de caract�re avec
	le hash fournis par l'utilisateur.
*/
class MD5 : public Algorithm
{
public:
	MD5(unsigned char* hash);
	~MD5();
	bool compare(std::string str);
};
