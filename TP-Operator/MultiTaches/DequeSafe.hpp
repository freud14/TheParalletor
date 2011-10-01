/**
	@file DequeSafe.hpp
	Fichier qui contient la classe DequeSafe, qui prot�ge le classe Deque des erreurs de processus l�gers
	@author Frederik Paradis
	@author Louis-Etienne Dorval
	@date 8 avril 2011
	@version 1.0
*/

#pragma once

#include <iostream>
#include <deque>
using namespace std;
/**
	Constructeur de DequeSafe
*/
template <class T>
        DequeSafe<T>::DequeSafe(){
    InitializeCriticalSection(&_cs) ;
}

/**
	Ajoute un �l�ment a la fin du DequeSafe
	@param [in] _x �l�ment � ajouter 
*/
template <class T>
        void DequeSafe<T>::push_back(const T& _x){
    EnterCriticalSection(&_cs);

    deque<T>::push_back(_x);

    LeaveCriticalSection(&_cs);
}

/**
	Ajoute un �l�ment au d�but du DequeSafe
	@param [in] _x �l�ment � ajouter 
*/
template <class T>
        void DequeSafe<T>::push_front(const T& _x){
    EnterCriticalSection(&_cs);

    deque<T>::push_front(_x);

    LeaveCriticalSection(&_cs);
}

/**
	Enl�ve l'�lement de la fin
*/
template <class T>
        void DequeSafe<T>::pop_back(){
    EnterCriticalSection(&_cs);

    deque<T>::pop_back();

    LeaveCriticalSection(&_cs);
}

/**
	Enl�ve l'�lement du d�but
*/
template <class T>
        void DequeSafe<T>::pop_front(){
    EnterCriticalSection(&_cs);

    deque<T>::pop_front();

    LeaveCriticalSection(&_cs);
}

/**
	V�rifie si le DequeSafe est vide.
	@return Retourne vrai si la file est vide; sinon faux.
*/
template <class T>
        bool DequeSafe<T>::empty()
{
    EnterCriticalSection(&_cs);

    bool retour = deque<T>::empty();

    LeaveCriticalSection(&_cs);

    return retour;
}

/**
	Op�rateur []
	@param [in] n Index de l'�l�ment d�sir�
	@return Retourne l'�lement d�sir�
*/
template <class T>
        T& DequeSafe<T>::operator[](int n)
{
    EnterCriticalSection(&_cs);

    T retour = deque<T>::operator[](n);

    LeaveCriticalSection(&_cs);

    return retour;
}

/**
	Retourne la taille du DequeSafe
	@return Retourne la taille du DequeSafe
*/
template <class T>
        int DequeSafe<T>::size()
{
    EnterCriticalSection(&_cs);

    int retour = deque<T>::size();

    LeaveCriticalSection(&_cs);

    return retour;
}

/**
	Destructeur de DequeSafe
*/
template <class T>
        DequeSafe<T>::~DequeSafe()
{
    DeleteCriticalSection(&_cs) ;
}
