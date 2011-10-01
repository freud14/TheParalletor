/**
	@file	Console.cpp
			Fichier contenant la d�claration d'une classe qui sert �
			effectuer l'affichage par des processus l�gers.
	@author Fr�d�rik Paradis et Louis-�tienne Dorval
	@date   8 avril 2010
*/

#pragma once
#include <iostream>
#include <process.h>
#include <Windows.h>
#include <string>

/**
	La classe Console sert � effectuer l'affichage par
	diff�rent processus l�ger.
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
