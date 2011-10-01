/**
	@file Worker.h
	Fichier contenant la declaration de la classe Worker, qui hérite de Thread, et ses entetes de fonctions
	@author Frederik Paradis
	@author Louis-Etienne Dorval
	@date 8 avril 2011
	@version 1.0
*/

#pragma once
#include <windows.h>  //POUR DWORD, HANDLE, WINAPI
#include <process.h> //Pour l'utilisation de _beginthreadex

class Pool;	//Pour éviter dépendance circulaire... Utiliser la déclaration de Pool au lieu du .h (Pool inclut worker et worker inclut Pool)

/**
	La classe Worker se charge d'exécuter toutes les tâches qui lui sont données.
*/
class Worker: public Thread
{
public:
	Worker(Pool* pool);
	virtual ~Worker();
	void SetTask(Task* task);
	DWORD execute();
private:
	volatile Task* task;
	Pool* pool;
	HANDLE hNewTaskEvent;
};