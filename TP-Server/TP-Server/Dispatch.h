/**
	@file	Dispatch.h
			Fichier contenant la d�claration de la classe Dispatch.
	@author Fr�d�rik Paradis
	@author Louis-�tienne Dorval
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
	La classe Dispatch sert � repr�senter la division de la
	t�che donn�e par le client.
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