/**
        @file Pool.cpp
        Fichier qui contient la classe Pool qui g�re les Taks et les distribue aux Worker
        @author Frederik Paradis
        @author Louis-Etienne Dorval
        @date 8 avril 2011
        @version 1.0
 */

#include "Pool.h"

/**
        Le constructeur de Pool initialise les ressources de l'objet.
        @param [in] nbWorkers Nombre de Workers d�sir�s
 */
Pool::Pool(short nbWorkers) {
    //On cr�e les Workers
    this->tabWorkers = new Worker*[nbWorkers];
    this->nbWorkers = nbWorkers;
    for (int i = 0; i < this->nbWorkers; i++) {
        this->tabWorkers[i] = new Worker(this);
        this->fWorkers.push_back(this->tabWorkers[i]);
    }

    //On cr�e les Events
#ifdef _WIN32
    this->hNewTaskEvent = CreateEvent(NULL, 0, 0, NULL);
    this->hFreeWorkerEvent = CreateEvent(NULL, 0, 0, NULL);
#elif defined linux
    this->hFreeWorkerEvent = new pthread_cond_t(PTHREAD_COND_INITIALIZER); /* Création de la condition */
    this->mtxFreeWorkerEvent = new pthread_mutex_t(PTHREAD_MUTEX_INITIALIZER); /* Création du mutex */
#endif

    //On d�marre les Workers
    for (int i = 0; i < this->nbWorkers; i++) {
        this->tabWorkers[i]->start();
    }
}

/**
        Le destructeur de Worker lib�re les ressources utilis�es.
 */
Pool::~Pool() {
    //On indique � Execute que le pool est Terminated
    this->waitForThread();

    //On supprime les Workers
    for (int i = 0; i < this->nbWorkers; i++) {
        delete this->tabWorkers[i];
    }

    delete[] this->tabWorkers;

    //On lib�re les ressources des �v�nements
#ifdef _WIN32
    CloseHandle(this->hNewTaskEvent);
    CloseHandle(this->hFreeWorkerEvent);
#elif defined linux
    delete this->hFreeWorkerEvent;
    delete this->mtxFreeWorkerEvent;
#endif
}

/**
        Permet d'ajouter des Task qui devront �tre ex�cut�
        @param [in] task T�ches � ajouter
 */
void Pool::schedule(Task* task) {
    this->fTasks.push_back(task);
#ifdef linux
    pthread_mutex_lock(this->mtxThreadTerminated); /* On verrouille le mutex */
    pthread_cond_signal(this->hThreadTerminated);
    pthread_mutex_unlock(this->mtxThreadTerminated); /* On déverrouille le mutex */
#elif defined _WIN32
    SetEvent(this->hNewTaskEvent);
#endif
}

/**
        Ajout d'un Worker libre, donc pr�t � effectuer une Task
        @param [in] worker Le Worker libre qui s'ajoute.
 */
void Pool::addFreeWorker(Worker* worker) {
    this->fWorkers.push_front(worker);
#ifdef linux
    pthread_mutex_lock(this->mtxFreeWorkerEvent); /* On verrouille le mutex */
    pthread_cond_signal(this->hFreeWorkerEvent);
    pthread_mutex_unlock(this->mtxFreeWorkerEvent); /* On déverrouille le mutex */
#elif defined _WIN32
    SetEvent(this->hFreeWorkerEvent);
#endif
}

/**
        Fonction principale qui g�re les Task, les Workers
        @return Returne le status du Pool
 */
DWORD Pool::execute() {
    while (!this->threadTerminated || !this->fTasks.empty()) {
        if (!this->fTasks.empty()) {
            if (!this->fWorkers.empty()) {
                this->fWorkers[0]->SetTask(this->fTasks[0]);
                this->fWorkers.pop_front();
                this->fTasks.pop_front();
            }
            else //La file de Worker est vide
            {
                //On attend qu'un Worker soit libre
#ifdef linux
            	pthread_mutex_lock(this->mtxFreeWorkerEvent); /* On verrouille le mutex */
           	pthread_cond_wait(this->hFreeWorkerEvent, this->mtxFreeWorkerEvent);
         	pthread_mutex_unlock(this->mtxFreeWorkerEvent); /* On déverrouille le mutex */
#elif defined _WIN32
                WaitForSingleObject(this->hFreeWorkerEvent, INFINITE);
#endif
            }
        }
        else //La file de t�che est vide
        {
#ifdef linux
            pthread_mutex_lock(this->mtxThreadTerminated); /* On verrouille le mutex */
            pthread_cond_wait(this->hThreadTerminated, this->mtxThreadTerminated);
            pthread_mutex_unlock(this->mtxThreadTerminated); /* On déverrouille le mutex */
#elif defined _WIN32
            //On attend qu'il y ait une nouvelle t�che ou une demande de fermeture du pool.
            HANDLE tabHandle[2] = {this->hNewTaskEvent, this->hThreadTerminated};
            WaitForMultipleObjects(2, tabHandle, false, INFINITE);
#endif
        }
    }

    return 0;
}
