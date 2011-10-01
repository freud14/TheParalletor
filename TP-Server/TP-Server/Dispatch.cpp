/**
	@file	Dispatch.cpp
			Fichier contenant l'implémentation de la classe Dispatch.
	@author Frédérik Paradis
	@author Louis-Étienne Dorval
	@date   16 mai 2011
*/

#include "Dispatch.h"

/**
	Le constructeur initialise une Dispatch.
	@param [in] depart La chaine de départ de la recherche.
	@param [in] nbEssais Le nombre d'essais à effectuer.
*/
Dispatch::Dispatch(const std::string& depart, EntierLong nbEssais)
{
	this->depart = depart;
	this->nbEssais = nbEssais;
}

/**
	Retourne la chaine de départ de la recherche.
	@return Retourne la chaine de départ de la recherche.
*/
const std::string& Dispatch::getDepart() const
{
	return this->depart;
}

/**
	Retourne le nombre d'essais à effectuer.
	@return Retourne le nombre d'essais à effectuer.
*/
const EntierLong Dispatch::getNbEssais() const
{
	return this->nbEssais;
}