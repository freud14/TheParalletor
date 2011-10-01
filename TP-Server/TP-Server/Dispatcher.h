/**
	@file	Dispatcher.h
			Fichier contenant la déclaration de la classe Dispatcher.
	@author Frédérik Paradis
	@author Louis-Étienne Dorval
	@date   16 mai 2011
*/

#ifndef DISPATCHER_H
#define DISPATCHER_H
#include <string>
#include <deque>
#include "Dispatch.h"

/**
	La classe Dispatcher se charge de faire les divisions de 
	la tâche donnée par l'opérateur.
*/
class Dispatcher
{
public:
	Dispatcher(int minCharacter, int maxCharacter, EntierLong nbEssaisParDispatch, std::string& letters, std::string& saltBefore, std::string& saltAfter);
	const Dispatch getDispatch();
	short getMinCharacter();
	short getMaxCharacter();
	EntierLong getNbEssaisParDispatch();
private:
	std::deque<int> createIndex(std::string& mot);
	std::string indexToString(std::deque<int> index);
	std::string addLetters();
	short minCharacter;
	short maxCharacter;
	std::string depart;
	EntierLong nbEssaisParDispatch;
	std::string letters;
	std::string saltBefore;
	std::string saltAfter;
};
#endif