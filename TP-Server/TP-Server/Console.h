/**
	@file	Console.cpp
			Fichier contenant la déclaration d'une classe qui sert à
			effectuer l'affichage par des processus légers.
	@author Frédérik Paradis et Louis-Étienne Dorval
	@date   8 avril 2010
*/

#pragma once
#include <iostream>
#include <process.h>
#include <Windows.h>
#include <string>

/**
	La classe Console sert à effectuer l'affichage par
	différent processus léger.
*/
class Console
{
public:
	static Console* getInstance();
	Console();
	~Console();
	void println(const char* line);
private:
	static Console* instance;
	CRITICAL_SECTION _cs;
};
