/**
	@file	Dispatch.h
			Fichier contenant la déclaration de la classe Dispatch.
	@author Frédérik Paradis
	@author Louis-Étienne Dorval
	@date   16 mai 2011
*/

#ifndef DISPATCH_H
#define DISPATCH_H
#include <iostream>
#include <string>
#include <vector>
#include <Task.h>
#include "Algorithm.h"

//Debug
//#include "Console.h"

class Dispatcher;

typedef unsigned long long EntierLong;

/**
	La classe Dispatch représente la division de la tâche du
	Dispatcher. Cette classe implémente également l'algorithme
	de recherche exhaustive.
*/
class Dispatch : public Task
{
public:
	Dispatch(const std::string& depart, EntierLong nbEssais, Dispatcher* dispatcher);
	void stop();
	static void setAlgorithm(Algorithm* algo);
	static void setLettersAndSalts(const std::string& letters, const std::string& saltBefore, const std::string& saltAfter);
	static std::string& getLetters();
	static std::string& getSaltBefore();
	static std::string& getSaltAfter();
	void execute();
private:
	std::vector<int> createIndex();
	std::string depart;
	EntierLong nbEssais;
	volatile bool stopSearch;
	Dispatcher* dispatcher;
	static Algorithm* algo;
	static std::string letters;
	static std::string saltBefore;
	static std::string saltAfter;
};
#endif