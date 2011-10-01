/**
	@file	Console.cpp
			Fichier contenant la déclaration d'une classe qui sert à
			effectuer l'affichage par des processus légers.
	@author Frédérik Paradis et Louis-Étienne Dorval
	@date   8 avril 2010
*/

#include "Console.h"

/**
	L'instance « Singleton » de notre objet.
*/
Console* Console::instance = NULL;

/**
	Méthode statique qui gère le Singleton de la classe.
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
	Constructeur initialisant les ressources utilisées par l'objet.
*/
Console::Console()
{
	InitializeCriticalSection(&this->_cs);
}

/**
	Destructeur qui libère les ressources utilisées par l'objet.
*/
Console::~Console()
{
	DeleteCriticalSection(&this->_cs);
}

/**
	La méthode sert à afficher la ligne passé en paramètre
	avec le numéro de ligne.
	@param[in] line La chaîne de caractères à afficher
*/
void Console::println(const char* line)
{
	EnterCriticalSection(&this->_cs); //_cs est barré !

	//Utilisation de printf pour faire de l'optimisation car la fonction est plus rapide
	printf("%s\r\n", line);

	LeaveCriticalSection(&this->_cs); //_cs est débarré !
		
}