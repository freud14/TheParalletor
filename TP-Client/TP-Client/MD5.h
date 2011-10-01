/**
	@file	MD5.h
			Fichier contenant la déclaration de la classe MD5
			qui encapsule l'algorithm MD5.
	@author Frédérik Paradis
	@author Louis-Étienne Dorval
	@date   16 mai 2011
*/

#pragma once
#include <string>
#include "Algorithm.h"
#include "md5Impl.h"

/**
	Cette structure sert à recevoir de l'opérateur la recherche
	exhaustive à effectuer. Le hash contenu dans cette structure
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
	Cette classe sert à encapsuler l'algorithme MD5
	et permet de pouvoir comparer une chaîne de caractère avec
	le hash fournis par l'utilisateur.
*/
class MD5 : public Algorithm
{
public:
	MD5(unsigned char* hash);
	~MD5();
	bool compare(std::string str);
};
