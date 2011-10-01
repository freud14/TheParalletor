// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#include <stdio.h>
#include <deque>
using namespace std;

#ifdef _WIN32
#include <tchar.h>
#include "windows.h"  //POUR DWORD
#include <process.h> //Pour l'utilisation de _beginthreadex
#endif

#include "Thread.h"
#include "Task.h"
#include "Worker.h"
#include "Pool.h"



// TODO: reference additional headers your program requires here
