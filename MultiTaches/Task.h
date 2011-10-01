/**
	@file Task.h
	Fichier contenant la declaration de la classe Task
	@author Frederik Paradis
	@author Louis-Etienne Dorval
	@date 8 avril 2011
	@version 1.0
*/

#pragma once

/**
	La classe Task représente un tâche à effectuer. C'est
	une classe virtuelle pure.
*/
class Task
{
public:

	/**
		Constructeur de Task
	*/
	Task(){}

	/**
		Cette méthode doit être implémenter afin de pouvoir exécuter une tâche.
	*/
	virtual void execute() = 0;
	
};
