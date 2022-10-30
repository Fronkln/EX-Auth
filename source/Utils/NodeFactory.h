#pragma once
#include "MinHook.h"
#include "..\build\Nodes\BaseAuthExtendedElement.h"
#include "..\build\Nodes\AuthNodeSystemSpeed.h"
#include "..\build\Globals.h"


typedef void* (__fastcall* auth_create_element)(int authID, void* buff);

void precache_vftable(long long* referenceAddress)
{
	std::cout << referenceAddress << std::endl;

	//This is only tested in LJ wasnt tried in other games
	const int vfTableLength = 35;
	
	Globals::vfTableCache = (long long*)malloc(vfTableLength * 8);
	memcpy_s((void*)Globals::vfTableCache, vfTableLength * 8, (void*)referenceAddress, vfTableLength * 8);

	//1337: System Speed
	Globals::vfTableSystemSpeed = (long long*)malloc(vfTableLength * 8);
	memcpy_s(Globals::vfTableSystemSpeed, vfTableLength * 8, Globals::vfTableCache, vfTableLength * 8);

	Globals::vfTableSystemSpeed[31] = (long long)&AuthNodeSystemSpeed::Play;

	std::cout << Globals::vfTableSystemSpeed << std::endl;

}

auth_create_element  create_element_trampoline;
static void* auth_create_element_hook(int elementID, void* buff)
{
	if(elementID < 1337)
		return create_element_trampoline(elementID, buff);

	BaseAuthExtendedElement* createdCustomNode = NULL;

	switch (elementID)
	{
	default:
	{
		std::wstring message = std::wstring(L"Unrecognized custom node detected.");
		message += L"\nPlease update EX Auth. Game will now exit.";
		MessageBox(0, message.c_str(), L"EX Auth Error", MB_OK);
		exit(0);
		break;
	}

	case 1337:
	{

		createdCustomNode = (BaseAuthExtendedElement*)create_element_trampoline(0x5C, buff);

		if (Globals::vfTableCache == nullptr)
			precache_vftable((*(long long**)createdCustomNode));

		*(long long**)createdCustomNode = Globals::vfTableSystemSpeed;
	}
	}

	if (createdCustomNode == NULL)
		return NULL;

	if (createdCustomNode->version > Globals::version)
	{
		std::wstring message = std::wstring(L"Custom node version is lower than current version.");
		message += L"\nPlease update EX Auth. Game will now exit.";
		MessageBox(0, message.c_str(), L"EX Auth", MB_OK);
		exit(0);

	}

	return createdCustomNode;
}

inline void InitNodeFactory()
{
	MH_Initialize();

	std::uint8_t* authRegistrationFunc = Globals::GetAuthCreationFunc();

	MH_CreateHook(reinterpret_cast<void**>(authRegistrationFunc), &auth_create_element_hook, reinterpret_cast<void**>(&create_element_trampoline));
	MH_EnableHook(reinterpret_cast<void**>(authRegistrationFunc));
}
