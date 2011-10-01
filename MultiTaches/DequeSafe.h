/**
        @file DequeSafe.h
        Fichier contenant la declaration de la DequeSafe
        @author Frederik Paradis
        @author Louis-Etienne Dorval
        @date 8 avril 2011
        @version 1.0
 */

#include <deque>
#ifdef _WIN32
#include <Windows.h>
#elif defined linux
#include <pthread.h>
#endif

/**
        La classe DequeSafe h�rite de <code>deque</code> mais la rend
        � Thread Safe � ce qui permet qu'il n'y ait qu'un seul processus
        l�ger puisser l'utiliser � un instant donn�.
 */
template <class T>
class DequeSafe : public std::deque<T> {
public:
    DequeSafe();
    ~DequeSafe();
    void push_back(const T& _x);
    void push_front(const T& _x);
    void pop_back();
    void pop_front();
    bool empty();
    T & operator[](int n);
    int size();
private:
#ifdef _WIN32
    CRITICAL_SECTION _cs;
#elif defined linux
    pthread_mutex_t* _cs;
#endif
};

#include "DequeSafe.hpp"