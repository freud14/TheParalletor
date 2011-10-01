/**
	@file	Dispatch.h
			Fichier contenant la déclaration de la classe Dispatch.
	@author Frédérik Paradis
	@author Louis-Étienne Dorval
	@date   16 mai 2011
*/

#ifndef DISPATCH_H
#define DISPATCH_H
#include <string>

/**
	Un nom plus court pour un <code>unsigned long long</code>.
*/
typedef unsigned long long EntierLong;

/**
	La classe Dispatch sert à représenter la division de la
	tâche donnée par le client.
*/
class Dispatch
{
public:
	Dispatch(const std::string& depart, EntierLong nbEssais);
	const std::string& getDepart() const;
	const EntierLong getNbEssais() const;
private:
	std::string depart;
	EntierLong nbEssais;
};
#endif