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
	La classe Thread se charge de g�rer un processus l�ger.
*/
class Thread{
public:

	//On passe l'instance de la classe Thread en argument, on peut donc acc�der la partie priv�e de t�che qui h�rite de Thread!
	/**
		Ex�cute l'objet de type <code>Thread</code> en param�tre
		@param [in] pThread L'objet de type <code>Thread</code> � ex�cuter
	*/
	static unsigned int WINAPI entryPoint(void* pThread){  
		Thread* t=(Thread*)pThread;
		return t->execute();
	}

	/**
		Cette m�thode initialise les ressources utilis�es par la classe.
	*/
	Thread()
	{
		this->threadTerminated = false;
		this->hThreadTerminated = CreateEvent(NULL, 0, 0, NULL);
	}

	/**
		Se charge de lib�rer les ressources utilis�es.
	*/
	virtual ~Thread()
	{
		CloseHandle(this->hThreadTerminated);
	}

	/**
		Cette m�thode se charge d'attendre que le thread se finisse.
	*/
	void waitForThread()
	{
		this->threadTerminated = true;
		SetEvent(this->hThreadTerminated);
		WaitForSingleObject(this->hThread, INFINITE);
	}

	/**
		M�thode virtuelle pure � impl�menter pour pouvoir ex�cuter une t�che.
	*/
	virtual DWORD execute() = 0;

	/**
		D�marre le thread
	*/
	DWORD start()
	{		
		hThread = (HANDLE)_beginthreadex(NULL, // attribut de securit� par defaut
			                             0,                           // taille de la pile par defaut
										 Thread::entryPoint,          // notre function
										 this,                         // l'argument pour la fonction (l'instance de la classe)
										 0,                           // flag de creation par defaut - voir CREATE_SUSPENDED 
										 NULL);                // retourne l'id du thread, si le param�tre est NULL, le ID n'est pas retourn�.

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
		Permet de d�bloque la m�thode <code>execute()</code> 
		afin que le processus l�ger se termine.
	*/
	HANDLE hThreadTerminated;

	/**
		Indique � la m�thode <code>execute()</code> que le processus l�ger a � se terminer.
	*/
	volatile bool threadTerminated; //Afin de savoir au bon moment que le thread est termin�.

};