/**
        @file thread.h
        Fichier contenant la declaration de la classe Thread
        @author Frederik Paradis
        @author Louis-Etienne Dorval
        @date 8 avril 2011
        @version 1.0
 */

#pragma once
#ifdef linux
#define DWORD void*
#include <pthread.h>
#include <assert.h>
#elif defined _WIN32
#include <windows.h>  //POUR DWORD, HANDLE, WINAPI
#include <process.h> //Pour l'utilisation de _beginthreadex
#include <assert.h>
#endif

/**
        La classe Thread se charge de g�rer un processus l�ger.
 */
class Thread {
public:

    //On passe l'instance de la classe Thread en argument, on peut donc acc�der la partie priv�e de t�che qui h�rite de Thread!

    /**
            Ex�cute l'objet de type <code>Thread</code> en param�tre
            @param [in] pThread L'objet de type <code>Thread</code> � ex�cuter
     */
    #ifdef linux
    static void* entryPoint(void* pThread) {
    #elif defined _WIN32
    static unsigned int WINAPI entryPoint(void* pThread) {
    #endif
        Thread* t = (Thread*) pThread;
        return t->execute();
    }

    /**
            Cette m�thode initialise les ressources utilis�es par la classe.
     */
    Thread() {
        this->threadTerminated = false;
        #ifdef linux
        this->hThreadTerminated = new pthread_cond_t(PTHREAD_COND_INITIALIZER); /* Création de la condition */
        this->mtxThreadTerminated = new pthread_mutex_t(PTHREAD_MUTEX_INITIALIZER); /* Création du mutex */
        #elif defined _WIN32
        this->hThreadTerminated = CreateEvent(NULL, 0, 0, NULL);
        #endif
    }

    /**
            Se charge de lib�rer les ressources utilis�es.
     */
    virtual ~Thread() {
        #ifdef linux
        delete this->hThreadTerminated;
        delete this->mtxThreadTerminated;
        #elif defined _WIN32
        CloseHandle(this->hThreadTerminated);
        #endif

    }

    /**
            Cette m�thode se charge d'attendre que le thread se finisse.
     */
    void waitForThread() {
        this->threadTerminated = true;
        #ifdef linux
        pthread_mutex_lock (this->mtxThreadTerminated); /* On verrouille le mutex */
        pthread_cond_signal (this->hThreadTerminated); /* On délivre le signal : condition remplie */
        pthread_mutex_unlock (this->mtxThreadTerminated); /* On déverrouille le mutex */
        pthread_join(this->hThread, NULL);
        #elif defined _WIN32
        SetEvent(this->hThreadTerminated);
        WaitForSingleObject(this->hThread, INFINITE);
        #endif
    }

    /**
            M�thode virtuelle pure � impl�menter pour pouvoir ex�cuter une t�che.
     */
    virtual DWORD execute() = 0;

    /**
            D�marre le thread
     */
    DWORD start() {
        #ifdef linux
        pthread_create (&this->hThread, NULL, Thread::entryPoint, (void*) this);
        //assert(this->hThread == 0);
        #elif defined _WIN32
        hThread = (HANDLE) _beginthreadex(NULL, // attribut de securit� par defaut
                0, // taille de la pile par defaut
                Thread::entryPoint, // notre function
                this, // l'argument pour la fonction (l'instance de la classe)
                0, // flag de creation par defaut - voir CREATE_SUSPENDED
                NULL); // retourne l'id du thread, si le param�tre est NULL, le ID n'est pas retourn�.
        assert(hThread != NULL);
        #endif
        return 0;
    }

private:
    /**
            Le HANDLE du thread,
     */
    #ifdef linux
    pthread_t hThread;
    #elif _WIN32
    HANDLE hThread;
    #endif

protected:
    /**
            Permet de d�bloque la m�thode <code>execute()</code>
            afin que le processus l�ger se termine.
     */
    #ifdef linux
    pthread_cond_t* hThreadTerminated; /* Création de la condition */
    pthread_mutex_t* mtxThreadTerminated; /* Création du mutex */
    #elif defined _WIN32
    HANDLE hThreadTerminated;
    #endif

    /**
            Indique � la m�thode <code>execute()</code> que le processus l�ger a � se terminer.
     */
    volatile bool threadTerminated; //Afin de savoir au bon moment que le thread est termin�.

};
