/**
	@file	Dispatch.cpp
			Fichier contenant l'implémentation de la classe Dispatch.
	@author Frédérik Paradis
	@author Louis-Étienne Dorval
	@date   16 mai 2011
*/

#include "Dispatch.h"
#include "Dispatcher.h"

Algorithm* Dispatch::algo;
std::string Dispatch::letters;
std::string Dispatch::saltBefore;
std::string Dispatch::saltAfter;

/**
	Constructeur de Dispatch qui initialise la recherche exhaustive.
	@param [in] depart La chaine de départ de la recherche
	@param [in] nbEssais Le nombre d'essais à effectuer
	@param [in] dispatcher Le Dispatcher à avertir lorsque la réponse est trouvé.
*/
Dispatch::Dispatch(const std::string& depart, EntierLong nbEssais, Dispatcher* dispatcher)
{
	this->depart = depart;
	this->nbEssais = nbEssais;
	this->stopSearch = false;
	this->dispatcher = dispatcher;
}

/**
	Indique à l'objet d'arrêter la recherche.
*/
void Dispatch::stop()
{
	this->stopSearch = true;
}

/**
	Dis aux Dispatch quel algorithme utilisé pour
	comparer les essais de la recherche.
	@param [in] algo L'Algorithm à utiliser
*/
void Dispatch::setAlgorithm(Algorithm* algo)
{
	Dispatch::algo = algo;
}

/**
	Dis aux Dispatch quelles sont les lettres à utiliser
	dans les essais de la recherche
	@param [in] letters Les lettres à utiliser.
*/
void Dispatch::setLettersAndSalts(const std::string& letters, const std::string& saltBefore, const std::string& saltAfter)
{
	Dispatch::letters = letters;
	Dispatch::saltBefore = saltBefore;
	Dispatch::saltAfter = saltAfter;
}

/**
	Retourne les lettres utilisés dans la recherche exhaustive.
	@return Retourne les lettres utilisés dans la recherche exhaustive.
*/
std::string& Dispatch::getLetters()
{
	return Dispatch::letters;
}

std::string& Dispatch::getSaltBefore()
{
	return Dispatch::saltBefore;
}

std::string& Dispatch::getSaltAfter()
{
	return Dispatch::saltAfter;
}

/**
	Cette méthode se charge d'effectuer la recherche avec
	tous les paramètres spécifiés.
*/
void Dispatch::execute()
{
	std::vector<int> indexMot = this->createIndex();

	int cpt = 0;
	//On agit toujours sur le dernier caractère de la chaine.
	int i = indexMot[indexMot.size() - 1];
	//Tant que le nombre d'essais effectuer n'est pas atteint
	//ou bien qu'on ne dit pas d'arrêter.
	while(cpt < this->nbEssais && !this->stopSearch)
	{
		//On regarde si la chaine actuelle est la réponse
		std::string strTotal = Dispatch::saltBefore + this->depart + Dispatch::saltAfter;
		if(Dispatch::algo->compare(strTotal))
		{
			//Si oui, on avertit le dispatcher et on arrête la boucle.
			this->dispatcher->stopDispatch(strTotal);
			break;
		}

		++i;
		++cpt;
		//Si on a dépassé le nombre, on arrête.
		if(cpt >= this->nbEssais)
		{
			break;
		}

		//Si nous avons dépassé la dernière lettre de la recherche exhaustive
		if(i == Dispatch::letters.length())
		{
			//On recommence à la première lettre
			i = 0;

			//Et on répercute le changement sur les lettres d'avant.
			int j = this->depart.length() - 1;
			while(j >= 0 && this->depart[j] == Dispatch::letters[Dispatch::letters.length() - 1])
			{
				this->depart[j] = Dispatch::letters[0];
				indexMot[j] = 0;
				--j;
			}

			//Si j s'est rendu à -1, ça veut dire qu'on doit ajouter une lettre.
			if(j == -1)
			{
				this->depart += Dispatch::letters[0];
				indexMot.push_back(0);
			}
			else
			{
				++indexMot[j];
				this->depart[j] = Dispatch::letters[indexMot[j]];
			}
		}

		this->depart[this->depart.length() - 1] = Dispatch::letters[i];
		indexMot[this->depart.length() - 1] = i;
	}
}

/**
	Retourne une liste contenant les index qui correspondent
	aux lettres de l'attribut <code>depart</code>.
	@return Retourne une liste contenant les index qui 
	correspondent aux lettres de l'attribut <code>depart</code>.
*/
std::vector<int> Dispatch::createIndex()
{
	std::vector<int> index;

	//On parcours la chaine et pour chacun des caractère, on trouve
	//son index.
	for(std::string::iterator it = this->depart.begin(); it != this->depart.end(); ++it)
	{
		for(int i = 0; i < Dispatch::letters.length(); ++i)
		{
			if(*it == Dispatch::letters[i])
			{
				index.push_back(i);
				break;
			}
		}
	}

	return index;
}