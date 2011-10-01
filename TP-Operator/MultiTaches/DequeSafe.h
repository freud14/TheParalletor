/**
	@file DequeSafe.h
	Fichier contenant la declaration de la DequeSafe
	@author Frederik Paradis
	@author Louis-Etienne Dorval
	@date 8 avril 2011
	@version 1.0
*/

#include <deque>
#include <Windows.h>
#include <process.h>

/**
	La classe DequeSafe hérite de <code>deque</code> mais la rend
	« Thread Safe » ce qui permet qu'il n'y ait qu'un seul processus
	léger puisser l'utiliser à un instant donné.
*/
template <class T>
class DequeSafe : public std::deque<T>
{
public:
	DequeSafe();
	~DequeSafe();
	void push_back(const T& _x);
	void push_front(const T& _x);
	void pop_back();
	void pop_front();
	bool empty();
	T& operator[](int n);
	int size();
private:
	CRITICAL_SECTION _cs;
};

#include "MultiTaches/DequeSafe.hpp"
