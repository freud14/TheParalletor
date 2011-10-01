/**
	@file	Dispatcher.cpp
			Fichier contenant l'impl�mentation de la classe Dispatcher.
	@author Fr�d�rik Paradis
	@author Louis-�tienne Dorval
	@date   16 mai 2011
*/

#include "Dispatcher.h"
#include "Dispatch.h"

/**
	Le constructeur initialise l'objet avec les valeurs donn�es
	en param�tre.
	@param [in] minCharacter Le nombre de caract�re minimum de la recherche.
	@param [in] maxCharacter Le nombre de caract�re maximum de la recherche.
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

	//On cr�e notre chaine de d�part avec la premi�re lettre de la recherche
	for(int i = 0; i < minCharacter; ++i)
	{
		this->depart += this->letters[0];
	}
}

/**
	Retourne un Dispatch tel que configur� dans le constructeur.
	@return Retourne un Dispatch tel que configur� dans le constructeur.
*/
const Dispatch Dispatcher::getDispatch()
{
	//On cr�e notre Dispatch
	Dispatch dispatch(this->depart, this->nbEssaisParDispatch);
	this->depart = addLetters();

	return dispatch;
}

/**
	Cette m�thode cr�e un index � partir des lettres de la recherche
	exhaustive pour la chaine de caract�re donn�e en param�tre.
	@param [in] La cha�ne de caract�re � retourner en index.
	@return Retourne l'index de chacun des caract�re de la chaine de
	caract�re en param�tre.
*/
std::deque<int> Dispatcher::createIndex(std::string& mot)
{
	std::deque<int> index;

	//On parcours la chaine et pour chacun des caract�re, on trouve
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
	Cette m�thode cr�e la chaine de caract�re correspondante 
	� une liste d'index valide par rapport aux lettres de la
	recherche exhaustive.
	@param [in] index La liste d'index
	@return Retourne la chaine de caract�re r�sultante.
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
	G�n�re la prochaine chaine de caract�re pour un Dispatch.
	@return Retourne la chaine de caract�re voulu par rapport
	aux param�tres voulus.
*/
std::string Dispatcher::addLetters()
{
	EntierLong nombre = this->nbEssaisParDispatch;
	int nbLettre = this->letters.length();
	//On cr�e l'index de la string
	std::deque<int> index = this->createIndex(this->depart);
	//On commence au dernier caract�re de la chaine c'est-�-dire celui le plus � gauche.
	int i = index.size() - 1; 
	//Le nombre � additionner sera diminuer au fur et � mesure
	//jusqu'� ce qu'il soit � z�ro.
	while(nombre != 0)
	{
		// On prend la partie � additionner pour le pr�sent index
		// et on le soustrait au nombre
		int restant = nombre % nbLettre;
		nombre -= restant;
		// On divise le nombre pour aller � la lettre d'apr�s.
		nombre /= nbLettre;
		//On regarde si l'index actuel est n�gatif...
		if(i == -1)
		{
			//Si oui, on ajoute un caract�re � la chaine.
			++i;
			index.push_front(-1);
		}
		
		//On regarde si l'index actuel additionn�e � la partie 
		//� additionner est plus grand que la "base" c'est-�-dire
		//le nombre de lettres de la recherche exhaustive.
		if(index[i] + restant >= nbLettre)
		{
			//Si oui, on d�termine la lettre qui ira � l'index actuel
			index[i] = (index[i] + restant) % nbLettre;
			//et on r�percute les changements sur les lettres d'avant
			int tmp = i - 1;
			while(tmp >= 0 && index[tmp] == nbLettre - 1)
			{
				index[tmp] = 0;
				--tmp;
			}

			//Il est possible qu'il faille ajouter une lettre � la chaine
			if(tmp == -1)
			{
				index.push_front(0);
			}
			else
			{
				index[tmp] += 1;
			}
		}
		else //Sinon on additionne tout simplement la partie � additionner
		{
			index[i] += restant;
		}

		--i;
	}

	return this->indexToString(index); //On retourne une string.
}

/**
	Retourne le nombre maximal de caract�re de la t�che.
	@return Retourne le nombre maximal de caract�re de la t�che.
*/
short Dispatcher::getMinCharacter()
{
	return this->minCharacter;
}

/**
	Retourne le nombre minimal de caract�re de la t�che.
	@return Retourne le nombre minimal de caract�re de la t�che.
*/
short Dispatcher::getMaxCharacter()
{
	return this->maxCharacter;
}

/**
	Retourne le nombre d'essais voulu par division de la t�che.
	@return Retourne le nombre d'essais voulu par division de la t�che.
*/
EntierLong Dispatcher::getNbEssaisParDispatch()
{
	return this->nbEssaisParDispatch;
}
