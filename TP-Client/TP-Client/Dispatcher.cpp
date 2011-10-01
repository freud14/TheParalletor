/**
	@file	Dispatcher.cpp
			Fichier contenant l'implémentation de la classe Dispatcher.
	@author Frédérik Paradis
	@author Louis-Étienne Dorval
	@date   16 mai 2011
*/

#include "Dispatcher.h"
#include "Dispatch.h"
#define DIVISION_COEF 2

/**
	Le constructeur se charge de créer et d'initiliser les
	Dispatch.
	@param [in] depart La chaine de départ de la recherche exhaustive
	@param [in] nbEssai Le nombre d'essais à effectuer
*/
Dispatcher::Dispatcher(std::string depart, EntierLong nbEssai)
{
	this->result = NULL;
	this->finish = false;

	//On prend le nombre de coeurs et on fait le nombre de tâche avec.
#ifdef linux
	this->nbTache = sysconf( _SC_NPROCESSORS_ONLN ) * DIVISION_COEF;
#elif defined _WIN32
	SYSTEM_INFO siSysInfo;
	GetSystemInfo(&siSysInfo); 
	this->nbTache = siSysInfo.dwNumberOfProcessors * DIVISION_COEF;
#endif
	this->taches = new Dispatch*[this->nbTache];

	//On crée les tâches en divisant le nombre d'essais total pour le nombre de tâche.
	EntierLong nbEssaisTache = nbEssai / this->nbTache;
	for(int i = 0; i < this->nbTache - 1; ++i)
	{
		this->taches[i] = new Dispatch(depart, nbEssaisTache, this);
		depart = depart + nbEssaisTache;
	}
	//On donne le restant à la dernière tâche.
	this->taches[this->nbTache - 1] = new Dispatch(depart, nbEssaisTache + (nbEssai % this->nbTache), this);
}

/**
	Cette méthode se charge d'exécuter la tâche dans plusieurs threads.
	L'appel de cette méthode est bloquante et il est donc conseillé de
	l'exécuter dans un thread avec la méthode <code>start()</code>.
	@return Retourne 0 s'il n'y aucun problème.
*/
DWORD Dispatcher::execute()
{
	//On démarre le pool et on lui donne les tâches.
	Pool* pool = new Pool(this->nbTache / DIVISION_COEF);
	pool->start();
	for(int i = 0; i < this->nbTache; ++i)
	{
		pool->schedule(this->taches[i]);
	}
	//On attend ces dites tâches.
	delete pool;
	
	//On indique au serveur que les tâches sont finis.
	this->finish = true;

	return 0;
}

/**
	Cette méthode indique si la recherche est fini.
	@return Retourne vrai si la recherche est fini; sinon faux.
*/
bool Dispatcher::isFinish()
{
	return this->finish;
}

/**
	Retourne le résultat de la recherche.
	@return Retourne le résultat de la recherche 
	s'il a été trouvé; sinon NULL;
*/
std::string* Dispatcher::getResult()
{
	return (std::string*)this->result;
}

/**
	Cette méthode permet à un Dispatch d'arrêter la recherche.
	@param [in] result Le résulat de la recherche
*/
void Dispatcher::stopDispatch(std::string& result)
{
	if(this->result != NULL)
	{
		delete this->result;
	}
	this->result = new std::string(result);

	this->stopDispatch();
}

/**
	Cette méthode permet d'arrêter la recherche.
*/
void Dispatcher::stopDispatch()
{
	for(int i = 0; i < this->nbTache; ++i)
	{
		this->taches[i]->stop();
	}
}

/**
	Le destructeur libère les ressources prises par l'objet.
*/
Dispatcher::~Dispatcher()
{
	this->waitForThread();
	delete this->result;

	for(int i = 0; i < this->nbTache; ++i)
	{
		delete this->taches[i];
	}
	delete[] this->taches;
}

/**
	Retourne une liste contenant les index qui correspondent
	aux lettres dans une chaine de caractère.
	@param [in] mot La chaine de caractère à traiter.
	@return Retourne une liste contenant les index qui 
	correspondent aux lettres dans une chaine de caractère.
*/
std::deque<int> Dispatcher::createIndex(std::string& mot)
{
	std::deque<int> index;

	//On parcours la chaine et pour chacun des caractère, on trouve
	//son index.
	for(std::string::iterator it = mot.begin(); it != mot.end(); ++it)
	{
		for(int i = 0; i < Dispatch::getLetters().length(); ++i)
		{
			if(*it == Dispatch::getLetters()[i])
			{
				index.push_back(i);
				break;
			}
		}
	}

	return index;
}

/**
	Prend une liste d'index qui correspondent aux lettres de la
	recherche exhaustive et les transforment en chaine de caractère.
	@param [in] index La liste d'index à transformer
	@return Retourne la chaine de caractère correspondante.
*/
std::string Dispatcher::indexToString(std::deque<int> index)
{
	std::string retour;
	for(std::deque<int>::iterator it = index.begin(); it != index.end(); ++it)
	{
		retour += Dispatch::getLetters()[*it];
	}

	return retour;
}

/**
	Cette méthode permet d'additionner une chaine de caractère
	avec un nombre qui représente le nombre de possibilité à 
	tester.
	@param [in] mot La chaine de caractère à additionner
	@param [in] nombre Le nombre à additionner
	@return La chaine de caractère résultante
*/
std::string operator+(std::string& mot, EntierLong nombre)
{
	int nbLettre = Dispatch::getLetters().length();
	//On crée l'index de la string
	std::deque<int> index = Dispatcher::createIndex(mot);
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

	return Dispatcher::indexToString(index); //On retourne une string.
}
