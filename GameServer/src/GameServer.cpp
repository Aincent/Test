//============================================================================
// Name        : GameServer.cpp
// Author      : helh
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "ServiceMain.h"
#include "util.h"

using namespace std;
using namespace CommBaseOut;

int main()
{
#ifndef TEST
	CUtil::InitDaemon();
#endif

#ifdef USE_MEMORY_LEAKS
	MemoryLeaks::GetInstance()->Init(120);
#endif

#ifdef USE_MEMORY_POOL
	MemAllocator::GetInstance();
#endif

	{
		CServiceMain service;

		service.Start();
	}

#ifdef USE_MEMORY_LEAKS
	MemoryLeaks::GetInstance()->DestroyInstance();
#endif

#ifdef USE_MEMORY_POOL
	MemAllocator::GetInstance()->DestroyInstance();
#endif

	return 0;
}
