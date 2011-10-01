/**
	@file pool.h
	Fichier contenant la declaration de la classe Pool, qui h�rite de Thread, et ses entetes de fonctions
	@author Frederik Paradis
	@author Louis-Etienne Dorval
	@date 8 avril 2011
	@version 1.0
*/

#pragma once

#ifdef  _WIN32
#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif
#endif

#include <stdio.h>
#include <assert.h>

#ifdef  _WIN32
#include <tchar.h>
#include <windows.h>  //POUR DWORD, HANDLE, WINAPI
#include <process.h> //Pour l'utilisation de _beginthreadex
#endif

#include "Thread.h"
#include "Task.h"
#include "Worker.h"
#include "DequeSafe.h"

//debug
#include <iostream>
#include <string>
using namespace std;

/**
	La classe Pool est une classe qui se charge d'ex�cuter
	des t�ches avec un certain nombre de processus l�ger.
*/
class Pool : public Thread
{
public:
	Pool(short nbWorkers = 1);
	virtual ~Pool();
	void schedule(Task* task);
	void addFreeWorker(Worker* worker);
	virtual DWORD execute();
private:
	short nbWorkers;
	Worker** tabWorkers;
	DequeSafe<Task*> fTasks;
	DequeSafe<Worker*> fWorkers;
        #ifdef _WIN32
		HANDLE hNewTaskEvent;
		HANDLE hFreeWorkerEvent;
	#elif defined linux
		pthread_cond_t* hFreeWorkerEvent; /* Création de la condition */
		pthread_mutex_t* mtxFreeWorkerEvent; /* Création du mutex */
        #endif
};
