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
	La classe Task repr�sente un t�che � effectuer. C'est
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
		Cette m�thode doit �tre impl�menter afin de pouvoir ex�cuter une t�che.
	*/
	virtual void execute() = 0;
	
};
