/**
	@file thread.h
	Fichier contenant la declaration de la classe Thread
	@author Frederik Paradis
	@author Louis-Etienne Dorval
	@date 8 avril 2011
	@version 1.0
*/

#pragma once
#include <windows.h>  //POUR DWORD, HANDLE, WINAPI
#include <process.h> //Pour l'utilisation de _beginthreadex
#include <assert.h>

/**
	La classe Thread se charge de gérer un processus léger.
*/
class Thread{
public:

	//On passe l'instance de la classe Thread en argument, on peut donc accéder la partie privée de tâche qui hérite de Thread!
	/**
		Exécute l'objet de type <code>Thread</code> en paramètre
		@param [in] pThread L'objet de type <code>Thread</code> à exécuter
	*/
	static unsigned int WINAPI entryPoint(void* pThread){  
		Thread* t=(Thread*)pThread;
		return t->execute();
	}

	/**
		Cette méthode initialise les ressources utilisées par la classe.
	*/
	Thread()
	{
		this->threadTerminated = false;
		this->hThreadTerminated = CreateEvent(NULL, 0, 0, NULL);
	}

	/**
		Se charge de libérer les ressources utilisées.
	*/
	virtual ~Thread()
	{
		CloseHandle(this->hThreadTerminated);
	}

	/**
		Cette méthode se charge d'attendre que le thread se finisse.
	*/
	void waitForThread()
	{
		this->threadTerminated = true;
		SetEvent(this->hThreadTerminated);
		WaitForSingleObject(this->hThread, INFINITE);
	}

	/**
		Méthode virtuelle pure à implémenter pour pouvoir exécuter une tâche.
	*/
	virtual DWORD execute() = 0;

	/**
		Démarre le thread
	*/
	DWORD start()
	{		
		hThread = (HANDLE)_beginthreadex(NULL, // attribut de securité par defaut
			                             0,                           // taille de la pile par defaut
										 Thread::entryPoint,          // notre function
										 this,                         // l'argument pour la fonction (l'instance de la classe)
										 0,                           // flag de creation par defaut - voir CREATE_SUSPENDED 
										 NULL);                // retourne l'id du thread, si le paramètre est NULL, le ID n'est pas retourné.

		assert(hThread!=NULL);
		return 0;
	}

private:
	/**
		Le HANDLE du thread,
	*/
	HANDLE hThread;

protected:
	/**
		Permet de débloque la méthode <code>execute()</code> 
		afin que le processus léger se termine.
	*/
	HANDLE hThreadTerminated;

	/**
		Indique à la méthode <code>execute()</code> que le processus léger a à se terminer.
	*/
	volatile bool threadTerminated; //Afin de savoir au bon moment que le thread est terminé.

};