/**
	@file	Dispatcher.cpp
			Fichier contenant l'implémentation de la classe Dispatcher.
	@author Frédérik Paradis
	@author Louis-Étienne Dorval
	@date   16 mai 2011
*/

#include "Dispatcher.h"
#include "Dispatch.h"

/**
	Le constructeur initialise l'objet avec les valeurs données
	en paramètre.
	@param [in] minCharacter Le nombre de caractère minimum de la recherche.
	@param [in] maxCharacter Le nombre de caractère maximum de la recherche.
	@param [in] nbEssaisParDispatch Le nombre d'essais voulu par Dispatch.
	@param [in] letters Les lettres voulu dans la recherche exhaustive.
*/
Dispatcher::Dispatcher(int minCharacter, int maxCharacter, EntierLong nbEssaisParDispatch, std::string& letters, std::string& saltBefore, std::string& saltAfter)
{
	this->minCharacter = minCharacter;
	this->maxCharacter = maxCharacter;
	this->letters = letters;
	this->saltBefore = saltBefore;
	this->saltAfter = saltAfter;
	this->nbEssaisParDispatch = nbEssaisParDispatch;

	//On crée notre chaine de départ avec la première lettre de la recherche
	for(int i = 0; i < minCharacter; ++i)
	{
		this->depart += this->letters[0];
	}
}

/**
	Retourne un Dispatch tel que configuré dans le constructeur.
	@return Retourne un Dispatch tel que configuré dans le constructeur.
*/
const Dispatch Dispatcher::getDispatch()
{
	//On crée notre Dispatch
	Dispatch dispatch(this->depart, this->nbEssaisParDispatch);
	this->depart = addLetters();

	return dispatch;
}

/**
	Cette méthode crée un index à partir des lettres de la recherche
	exhaustive pour la chaine de caractère donnée en paramètre.
	@param [in] La chaîne de caractère à retourner en index.
	@return Retourne l'index de chacun des caractère de la chaine de
	caractère en paramètre.
*/
std::deque<int> Dispatcher::createIndex(std::string& mot)
{
	std::deque<int> index;

	//On parcours la chaine et pour chacun des caractère, on trouve
	//son index.
	for(std::string::iterator it = mot.begin(); it != mot.end(); ++it)
	{
		for(int i = 0; i < this->letters.length(); ++i)
		{
			if(*it == this->letters[i])
			{
				index.push_back(i);
				break;
			}
		}
	}

	return index;
}

/**
	Cette méthode crée la chaine de caractère correspondante 
	à une liste d'index valide par rapport aux lettres de la
	recherche exhaustive.
	@param [in] index La liste d'index
	@return Retourne la chaine de caractère résultante.
*/
std::string Dispatcher::indexToString(std::deque<int> index)
{
	std::string retour;
	for(std::deque<int>::iterator it = index.begin(); it != index.end(); ++it)
	{
		retour += this->letters[*it];
	}

	return retour;
}

/**
	Génère la prochaine chaine de caractère pour un Dispatch.
	@return Retourne la chaine de caractère voulu par rapport
	aux paramètres voulus.
*/
std::string Dispatcher::addLetters()
{
	EntierLong nombre = this->nbEssaisParDispatch;
	int nbLettre = this->letters.length();
	//On crée l'index de la string
	std::deque<int> index = this->createIndex(this->depart);
	//On commence au dernier caractère de la chaine c'est-à-dire celui le plus à gauche.
	int i = index.size() - 1; 
	//Le nombre à additionner sera diminuer au fur et à mesure
	//jusqu'à ce qu'il soit à zéro.
	while(nombre != 0)
	{
		// On prend la partie à additionner pour le présent index
		// et on le soustrait au nombre
		int restant = nombre % nbLettre;
		nombre -= restant;
		// On divise le nombre pour aller à la lettre d'après.
		nombre /= nbLettre;
		//On regarde si l'index actuel est négatif...
		if(i == -1)
		{
			//Si oui, on ajoute un caractère à la chaine.
			++i;
			index.push_front(-1);
		}
		
		//On regarde si l'index actuel additionnée à la partie 
		//à additionner est plus grand que la "base" c'est-à-dire
		//le nombre de lettres de la recherche exhaustive.
		if(index[i] + restant >= nbLettre)
		{
			//Si oui, on détermine la lettre qui ira à l'index actuel
			index[i] = (index[i] + restant) % nbLettre;
			//et on répercute les changements sur les lettres d'avant
			int tmp = i - 1;
			while(tmp >= 0 && index[tmp] == nbLettre - 1)
			{
				index[tmp] = 0;
				--tmp;
			}

			//Il est possible qu'il faille ajouter une lettre à la chaine
			if(tmp == -1)
			{
				index.push_front(0);
			}
			else
			{
				index[tmp] += 1;
			}
		}
		else //Sinon on additionne tout simplement la partie à additionner
		{
			index[i] += restant;
		}

		--i;
	}

	return this->indexToString(index); //On retourne une string.
}

/**
	Retourne le nombre maximal de caractère de la tâche.
	@return Retourne le nombre maximal de caractère de la tâche.
*/
short Dispatcher::getMinCharacter()
{
	return this->minCharacter;
}

/**
	Retourne le nombre minimal de caractère de la tâche.
	@return Retourne le nombre minimal de caractère de la tâche.
*/
short Dispatcher::getMaxCharacter()
{
	return this->maxCharacter;
}

/**
	Retourne le nombre d'essais voulu par division de la tâche.
	@return Retourne le nombre d'essais voulu par division de la tâche.
*/
EntierLong Dispatcher::getNbEssaisParDispatch()
{
	return this->nbEssaisParDispatch;
}
