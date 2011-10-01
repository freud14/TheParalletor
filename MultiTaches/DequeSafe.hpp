/**
        @file DequeSafe.hpp
        Fichier qui contient la classe DequeSafe, qui prot�ge le classe Deque des erreurs de processus l�gers
        @author Frederik Paradis
        @author Louis-Etienne Dorval
        @date 8 avril 2011
        @version 1.0
 */

#pragma once

/**
        Constructeur de DequeSafe
 */
template <class T>
DequeSafe<T>::DequeSafe() {
#ifdef linux
    _cs = new pthread_mutex_t; /* Création du mutex */
    pthread_mutex_init(_cs, NULL);
#elif defined _WIN32
    InitializeCriticalSection(&_cs);
#endif
}

/**
        Ajoute un �l�ment a la fin du DequeSafe
        @param [in] _x �l�ment � ajouter
 */
template <class T>
void DequeSafe<T>::push_back(const T& _x) {
#ifdef linux
    pthread_mutex_lock (_cs); /* On verrouille le mutex */
#elif defined _WIN32
    EnterCriticalSection(&_cs);
#endif

    std::deque<T>::push_back(_x);

#ifdef linux
    pthread_mutex_unlock (_cs); /* On déverrouille le mutex */
#elif defined _WIN32
    LeaveCriticalSection(&_cs);
#endif
}

/**
        Ajoute un �l�ment au d�but du DequeSafe
        @param [in] _x �l�ment � ajouter
 */
template <class T>
void DequeSafe<T>::push_front(const T& _x) {
#ifdef linux
    pthread_mutex_lock (_cs); /* On verrouille le mutex */
#elif defined _WIN32
    EnterCriticalSection(&_cs);
#endif

    std::deque<T>::push_front(_x);

#ifdef linux
    pthread_mutex_unlock (_cs); /* On déverrouille le mutex */
#elif defined _WIN32
    LeaveCriticalSection(&_cs);
#endif
}

/**
        Enl�ve l'�lement de la fin
 */
template <class T>
void DequeSafe<T>::pop_back() {
#ifdef linux
    pthread_mutex_lock (_cs); /* On verrouille le mutex */
#elif defined _WIN32
    EnterCriticalSection(&_cs);
#endif

    std::deque<T>::pop_back();

#ifdef linux
    pthread_mutex_unlock (_cs); /* On déverrouille le mutex */
#elif defined _WIN32
    LeaveCriticalSection(&_cs);
#endif
}

/**
        Enl�ve l'�lement du d�but
 */
template <class T>
void DequeSafe<T>::pop_front() {
#ifdef linux
    pthread_mutex_lock (_cs); /* On verrouille le mutex */
#elif defined _WIN32
    EnterCriticalSection(&_cs);
#endif

    std::deque<T>::pop_front();

#ifdef linux
    pthread_mutex_unlock (_cs); /* On déverrouille le mutex */
#elif defined _WIN32
    LeaveCriticalSection(&_cs);
#endif
}

/**
        V�rifie si le DequeSafe est vide.
        @return Retourne vrai si la file est vide; sinon faux.
 */
template <class T>
bool DequeSafe<T>::empty() {
#ifdef linux
    pthread_mutex_lock (_cs); /* On verrouille le mutex */
#elif defined _WIN32
    EnterCriticalSection(&_cs);
#endif

    bool retour = std::deque<T>::empty();

#ifdef linux
    pthread_mutex_unlock (_cs); /* On déverrouille le mutex */
#elif defined _WIN32
    LeaveCriticalSection(&_cs);
#endif

    return retour;
}

/**
        Op�rateur []
        @param [in] n Index de l'�l�ment d�sir�
        @return Retourne l'�lement d�sir�
 */
template <class T>
T& DequeSafe<T>::operator[](int n) {
#ifdef linux
    pthread_mutex_lock (_cs); /* On verrouille le mutex */
#elif defined _WIN32
    EnterCriticalSection(&_cs);
#endif

    T& retour = std::deque<T>::operator[](n);

#ifdef linux
    pthread_mutex_unlock (_cs); /* On déverrouille le mutex */
#elif defined _WIN32
    LeaveCriticalSection(&_cs);
#endif

    return retour;
}

/**
        Retourne la taille du DequeSafe
        @return Retourne la taille du DequeSafe
 */
template <class T>
int DequeSafe<T>::size() {
#ifdef linux
    pthread_mutex_lock (_cs); /* On verrouille le mutex */
#elif defined _WIN32
    EnterCriticalSection(&_cs);
#endif

    int retour = std::deque<T>::size();

#ifdef linux
    pthread_mutex_unlock (_cs); /* On déverrouille le mutex */
#elif defined _WIN32
    LeaveCriticalSection(&_cs);
#endif

    return retour;
}

/**
        Destructeur de DequeSafe
 */
template <class T>
DequeSafe<T>::~DequeSafe() {
#ifdef linux
    pthread_mutex_destroy(_cs);
    delete _cs;
#elif defined _WIN32
    DeleteCriticalSection(&_cs);
#endif
}
