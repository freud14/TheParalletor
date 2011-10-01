/**
	@file worker.cpp
	Fichier qui contient la classe Worker, qui sert � ex�cuter les t�ches qui lui est attribu�es
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
	Le destructeur de Worker se charge d'attendre la derni�re
	t�che et de lib�rer les ressources.
*/
Worker::~Worker()
{
	this->waitForThread();

	CloseHandle(this->hNewTaskEvent);
}

/**
	Attribue une t�che au Worker
	@param [in] task T�che � ex�cuter
*/
void Worker::SetTask(Task* task)
{
	this->task = task;
	SetEvent(this->hNewTaskEvent);
}

/**
	Fonction qui ex�cute la t�che qui lui a �t� affect�
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