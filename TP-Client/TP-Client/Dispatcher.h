/**
	@file	Dispatcher.h
			Fichier contenant la d�claration de la classe Dispatcher.
	@author Fr�d�rik Paradis
	@author Louis-�tienne Dorval
	@date   16 mai 2011
*/

#ifndef DISPATCHER_H
#define DISPATCHER_H
//#include <Windows.h>
#include <string>
#include <deque>
#include <Pool.h>
#include <Thread.h>
#include "Dispatch.h"

/**
	La classe Dispatcher se charge d'effectuer la divsion de la t�che donn�e
	par le serveur, de les faire ex�cuter par le pool de thread et de les attendre.
	Pour cela, cette classe a besoin d'�tre dans un thread � lui seul et, donc, 
	d'h�riter de la classe Thread.
*/
class Dispatcher : public Thread
{
public:
	Dispatcher(std::string depart, EntierLong nbEssai);
	DWORD execute();
	bool isFinish();
	std::string* getResult();
	void stopDispatch(std::string& resultat);
	void stopDispatch();
	~Dispatcher();
private:
	static std::deque<int> createIndex(std::string& mot);
	static std::string indexToString(std::deque<int> index);
	friend std::string operator+(std::string& mot, EntierLong nombre);
	int nbTache;
	Dispatch** taches;
	volatile std::string* result;
	volatile bool finish;
};

std::string operator+(std::string& mot, EntierLong nombre);
#endif