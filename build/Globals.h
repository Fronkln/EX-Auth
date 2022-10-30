#pragma once
#include <Windows.h>
#include "Maps.h"
#include "Utils/PatternScan.h"

class Globals
{
public:
	static float version;

	static inline Game currentGame;
	static float* speedVariables;

	static long long* vfTableCache;
	//Node 1337
	static	long long* vfTableSystemSpeed;

	static std::uint8_t* GetAuthCreationFunc()
	{

		if(Globals::currentGame == Game::YakuzaKiwami2)
			return PatternScan(GetModuleHandle(NULL), "40 55 48 8B EC 48 83 EC ? 48 C7 45 F0 ? ? ? ? 48 89 5C 24 48");
		else if(Globals::currentGame == Game::Judgment)
			return PatternScan(GetModuleHandle(NULL), "40 57 48 83 EC ? 48 C7 44 24 20 ? ? ? ? 48 89 5C 24 48 48 89 74 24 58 48 8B FA 33 DB");
		else if (Globals::currentGame == Game::YakuzaLikeADragon)
			return (std::uint8_t*)0x1408D5D30;
		else if (Globals::currentGame >= Game::LostJudgment)
			return PatternScan(GetModuleHandle(NULL), "48 89 5C 24 10 55 56 57 48 8D AC 24 80 F9 FF FF");
		else
			return 0;
	}

	static void Initialize()
	{
		wchar_t exePath[MAX_PATH + 1];
		GetModuleFileNameW(NULL, exePath, MAX_PATH);

		wstring wstr(exePath);
		string currentGameName = basenameBackslashNoExt(string(wstr.begin(), wstr.end()));

		Globals::currentGame = getGame(currentGameName);

		FindSpeed();
	}
	
	static void FindSpeed()
	{
		if (Globals::currentGame == Game::YakuzaKiwami2)
			speedVariables = (float*)((uintptr_t)GetModuleHandle(NULL) + 0x2EFB1F8);
		if(Globals::currentGame == Game::Judgment)
			speedVariables = (float*)resolve_relative_addr(PatternScan(GetModuleHandle(NULL), "48 8D 0D ? ? ? ? C5 FA 11 0C 81 C3"), 7);
		else if (Globals::currentGame == Game::YakuzaLikeADragon)
			speedVariables = (float*)0x143FD66B0;
		else if (Globals::currentGame >= Game::LostJudgment)
			speedVariables = (float*)resolve_relative_addr(PatternScan(GetModuleHandle(NULL), "C5 F8 29 1D ? ? ? ? 48 8B 5C 24 50"), 8, true);
	}
};