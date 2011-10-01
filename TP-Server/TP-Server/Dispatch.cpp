/**
	@file	Dispatch.cpp
			Fichier contenant l'impl�mentation de la classe Dispatch.
	@author Fr�d�rik Paradis
	@author Louis-�tienne Dorval
	@date   16 mai 2011
*/

#include "Dispatch.h"

/**
	Le constructeur initialise une Dispatch.
	@param [in] depart La chaine de d�part de la recherche.
	@param [in] nbEssais Le nombre d'essais � effectuer.
*/
Dispatch::Dispatch(const std::string& depart, EntierLong nbEssais)
{
	this->depart = depart;
	this->nbEssais = nbEssais;
}

/**
	Retourne la chaine de d�part de la recherche.
	@return Retourne la chaine de d�part de la recherche.
*/
const std::string& Dispatch::getDepart() const
{
	return this->depart;
}

/**
	Retourne le nombre d'essais � effectuer.
	@return Retourne le nombre d'essais � effectuer.
*/
const EntierLong Dispatch::getNbEssais() const
{
	return this->nbEssais;
}