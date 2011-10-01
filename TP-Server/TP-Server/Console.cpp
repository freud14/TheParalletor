/**
	@file	Console.cpp
			Fichier contenant la d�claration d'une classe qui sert �
			effectuer l'affichage par des processus l�gers.
	@author Fr�d�rik Paradis et Louis-�tienne Dorval
	@date   8 avril 2010
*/

#include "Console.h"

/**
	L'instance � Singleton � de notre objet.
*/
Console* Console::instance = NULL;

/**
	M�thode statique qui g�re le Singleton de la classe.
*/
Console* Console::getInstance()
{
	if(Console::instance == NULL)
	{
		Console::instance = new Console();
	}

	return instance;
}

/**
	Constructeur initialisant les ressources utilis�es par l'objet.
*/
Console::Console()
{
	InitializeCriticalSection(&this->_cs);
}

/**
	Destructeur qui lib�re les ressources utilis�es par l'objet.
*/
Console::~Console()
{
	DeleteCriticalSection(&this->_cs);
}

/**
	La m�thode sert � afficher la ligne pass� en param�tre
	avec le num�ro de ligne.
	@param[in] line La cha�ne de caract�res � afficher
*/
void Console::println(const char* line)
{
	EnterCriticalSection(&this->_cs); //_cs est barr� !

	//Utilisation de printf pour faire de l'optimisation car la fonction est plus rapide
	printf("%s\r\n", line);

	LeaveCriticalSection(&this->_cs); //_cs est d�barr� !
		
}