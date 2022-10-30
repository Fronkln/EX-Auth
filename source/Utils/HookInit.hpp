#pragma once

// This header file is intended to streamline the process of hooking inside executables by hooking into WinAPI functions
// It is intended to be included in a separate compilation unit after defining the following:

// #define HOOKED_FUNCTION WinAPIFunction - must be a define or else the plugin will patch into itself
// #define HOOKED_LIBRARY "LIBRARY.DLL"
// windows.h also must be included already (you probably had to include it to define the above)

// When integrated properly, the following function shall be exposed:
// void OnInitializeHook() - called once from the hooked WinAPI function

// The following exports are added to the binary:
// void InitializeASI()
// uint32_t GetBuildNumber() - returns revision/build number as defined in VersionInfo.lua (if defined)

// Hooks will be initialized by first attempting to patch IAT of the main module
// If this fails, selected WinAPI export will be hooked directly

#include "MemoryMgr.h"
#include "Trampoline.h"
#include "MinHook.h"
#include <iostream>
#include "NodeFactory.h"

#define STRINGIZE(s) STRINGIZE2(s)
#define STRINGIZE2(s) #s

extern void OnInitializeHook();

extern "C"
{
	static LONG InitCount = 0;
	__declspec(dllexport) void InitializeASI()
	{
		if ( _InterlockedCompareExchange(&InitCount, 1, 0) != 0 ) 
			return;

		Globals::Initialize();
		InitNodeFactory();
	}

#if defined(rsc_RevisionID) && defined(rsc_BuildID)
	__declspec(dllexport) uint32_t GetBuildNumber()
	{
		return (rsc_RevisionID << 8) | rsc_BuildID;
	}
#endif
}