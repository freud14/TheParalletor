/**
	@file	Dispatcher.cpp
			Fichier contenant l'impl�mentation de la classe Dispatcher.
	@author Fr�d�rik Paradis
	@author Louis-�tienne Dorval
	@date   16 mai 2011
*/

#include "Dispatcher.h"
#include "Dispatch.h"
#define DIVISION_COEF 2

/**
	Le constructeur se charge de cr�er et d'initiliser les
	Dispatch.
	@param [in] depart La chaine de d�part de la recherche exhaustive
	@param [in] nbEssai Le nombre d'essais � effectuer
*/
Dispatcher::Dispatcher(std::string depart, EntierLong nbEssai)
{
	this->result = NULL;
	this->finish = false;

	//On prend le nombre de coeurs et on fait le nombre de t�che avec.
#ifdef linux
	this->nbTache = sysconf( _SC_NPROCESSORS_ONLN ) * DIVISION_COEF;
#elif defined _WIN32
	SYSTEM_INFO siSysInfo;
	GetSystemInfo(&siSysInfo); 
	this->nbTache = siSysInfo.dwNumberOfProcessors * DIVISION_COEF;
#endif
	this->taches = new Dispatch*[this->nbTache];

	//On cr�e les t�ches en divisant le nombre d'essais total pour le nombre de t�che.
	EntierLong nbEssaisTache = nbEssai / this->nbTache;
	for(int i = 0; i < this->nbTache - 1; ++i)
	{
		this->taches[i] = new Dispatch(depart, nbEssaisTache, this);
		depart = depart + nbEssaisTache;
	}
	//On donne le restant � la derni�re t�che.
	this->taches[this->nbTache - 1] = new Dispatch(depart, nbEssaisTache + (nbEssai % this->nbTache), this);
}

/**
	Cette m�thode se charge d'ex�cuter la t�che dans plusieurs threads.
	L'appel de cette m�thode est bloquante et il est donc conseill� de
	l'ex�cuter dans un thread avec la m�thode <code>start()</code>.
	@return Retourne 0 s'il n'y aucun probl�me.
*/
DWORD Dispatcher::execute()
{
	//On d�marre le pool et on lui donne les t�ches.
	Pool* pool = new Pool(this->nbTache / DIVISION_COEF);
	pool->start();
	for(int i = 0; i < this->nbTache; ++i)
	{
		pool->schedule(this->taches[i]);
	}
	//On attend ces dites t�ches.
	delete pool;
	
	//On indique au serveur que les t�ches sont finis.
	this->finish = true;

	return 0;
}

/**
	Cette m�thode indique si la recherche est fini.
	@return Retourne vrai si la recherche est fini; sinon faux.
*/
bool Dispatcher::isFinish()
{
	return this->finish;
}

/**
	Retourne le r�sultat de la recherche.
	@return Retourne le r�sultat de la recherche 
	s'il a �t� trouv�; sinon NULL;
*/
std::string* Dispatcher::getResult()
{
	return (std::string*)this->result;
}

/**
	Cette m�thode permet � un Dispatch d'arr�ter la recherche.
	@param [in] result Le r�sulat de la recherche
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
	Cette m�thode permet d'arr�ter la recherche.
*/
void Dispatcher::stopDispatch()
{
	for(int i = 0; i < this->nbTache; ++i)
	{
		this->taches[i]->stop();
	}
}

/**
	Le destructeur lib�re les ressources prises par l'objet.
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
	aux lettres dans une chaine de caract�re.
	@param [in] mot La chaine de caract�re � traiter.
	@return Retourne une liste contenant les index qui 
	correspondent aux lettres dans une chaine de caract�re.
*/
std::deque<int> Dispatcher::createIndex(std::string& mot)
{
	std::deque<int> index;

	//On parcours la chaine et pour chacun des caract�re, on trouve
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
	recherche exhaustive et les transforment en chaine de caract�re.
	@param [in] index La liste d'index � transformer
	@return Retourne la chaine de caract�re correspondante.
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
	Cette m�thode permet d'additionner une chaine de caract�re
	avec un nombre qui repr�sente le nombre de possibilit� � 
	tester.
	@param [in] mot La chaine de caract�re � additionner
	@param [in] nombre Le nombre � additionner
	@return La chaine de caract�re r�sultante
*/
std::string operator+(std::string& mot, EntierLong nombre)
{
	int nbLettre = Dispatch::getLetters().length();
	//On cr�e l'index de la string
	std::deque<int> index = Dispatcher::createIndex(mot);
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

	return Dispatcher::indexToString(index); //On retourne une string.
}
