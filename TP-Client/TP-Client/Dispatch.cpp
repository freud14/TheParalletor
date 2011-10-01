/**
	@file	Dispatch.cpp
			Fichier contenant l'impl�mentation de la classe Dispatch.
	@author Fr�d�rik Paradis
	@author Louis-�tienne Dorval
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
	@param [in] depart La chaine de d�part de la recherche
	@param [in] nbEssais Le nombre d'essais � effectuer
	@param [in] dispatcher Le Dispatcher � avertir lorsque la r�ponse est trouv�.
*/
Dispatch::Dispatch(const std::string& depart, EntierLong nbEssais, Dispatcher* dispatcher)
{
	this->depart = depart;
	this->nbEssais = nbEssais;
	this->stopSearch = false;
	this->dispatcher = dispatcher;
}

/**
	Indique � l'objet d'arr�ter la recherche.
*/
void Dispatch::stop()
{
	this->stopSearch = true;
}

/**
	Dis aux Dispatch quel algorithme utilis� pour
	comparer les essais de la recherche.
	@param [in] algo L'Algorithm � utiliser
*/
void Dispatch::setAlgorithm(Algorithm* algo)
{
	Dispatch::algo = algo;
}

/**
	Dis aux Dispatch quelles sont les lettres � utiliser
	dans les essais de la recherche
	@param [in] letters Les lettres � utiliser.
*/
void Dispatch::setLettersAndSalts(const std::string& letters, const std::string& saltBefore, const std::string& saltAfter)
{
	Dispatch::letters = letters;
	Dispatch::saltBefore = saltBefore;
	Dispatch::saltAfter = saltAfter;
}

/**
	Retourne les lettres utilis�s dans la recherche exhaustive.
	@return Retourne les lettres utilis�s dans la recherche exhaustive.
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
	Cette m�thode se charge d'effectuer la recherche avec
	tous les param�tres sp�cifi�s.
*/
void Dispatch::execute()
{
	std::vector<int> indexMot = this->createIndex();

	int cpt = 0;
	//On agit toujours sur le dernier caract�re de la chaine.
	int i = indexMot[indexMot.size() - 1];
	//Tant que le nombre d'essais effectuer n'est pas atteint
	//ou bien qu'on ne dit pas d'arr�ter.
	while(cpt < this->nbEssais && !this->stopSearch)
	{
		//On regarde si la chaine actuelle est la r�ponse
		std::string strTotal = Dispatch::saltBefore + this->depart + Dispatch::saltAfter;
		if(Dispatch::algo->compare(strTotal))
		{
			//Si oui, on avertit le dispatcher et on arr�te la boucle.
			this->dispatcher->stopDispatch(strTotal);
			break;
		}

		++i;
		++cpt;
		//Si on a d�pass� le nombre, on arr�te.
		if(cpt >= this->nbEssais)
		{
			break;
		}

		//Si nous avons d�pass� la derni�re lettre de la recherche exhaustive
		if(i == Dispatch::letters.length())
		{
			//On recommence � la premi�re lettre
			i = 0;

			//Et on r�percute le changement sur les lettres d'avant.
			int j = this->depart.length() - 1;
			while(j >= 0 && this->depart[j] == Dispatch::letters[Dispatch::letters.length() - 1])
			{
				this->depart[j] = Dispatch::letters[0];
				indexMot[j] = 0;
				--j;
			}

			//Si j s'est rendu � -1, �a veut dire qu'on doit ajouter une lettre.
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

	//On parcours la chaine et pour chacun des caract�re, on trouve
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