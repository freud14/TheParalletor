/**
	@file worker.cpp
	Fichier qui contient la classe Worker, qui sert à exécuter les tâches qui lui est attribuées
	@author Frederik Paradis
	@author Louis-Etienne Dorval
	@date 8 avril 2011
	@version 1.0
*/

#include "Pool.h"


/**
	Constructeur de Worker
	@param [in] pool
*/
Worker::Worker(Pool* pool)
{
	this->pool = pool;
	this->task = NULL;

	this->hNewTaskEvent = CreateEvent(NULL, 0, 0, NULL);
}

/**
	Le destructeur de Worker se charge d'attendre la dernière
	tâche et de libérer les ressources.
*/
Worker::~Worker()
{
	this->waitForThread();

	CloseHandle(this->hNewTaskEvent);
}

/**
	Attribue une tâche au Worker
	@param [in] task Tâche à exécuter
*/
void Worker::SetTask(Task* task)
{
	this->task = task;
	SetEvent(this->hNewTaskEvent);
}

/**
	Fonction qui exécute la tâche qui lui a été affecté
	@return _DWORD Retour de la fonction
*/
DWORD Worker::execute()
{
	while(this->threadTerminated == false)
	{
		if (this->task != NULL) 
		{
			((Task*)this->task)->execute();
			this->task = NULL;
			this->pool->addFreeWorker(this);
		}
		else
		{
			HANDLE tabHandles[2] = {this->hNewTaskEvent, this->hThreadTerminated};
			WaitForMultipleObjects(2, tabHandles, false, INFINITE);
		}
	}

	return 0;
}