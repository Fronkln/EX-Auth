#pragma once
#include "BaseAuthExtendedElement.h"
#include "..\build\Globals.h"

class AuthNodeSystemSpeed : public BaseAuthExtendedElement
{
public:
	unsigned int Type;
	float Speed;

	static void Play(AuthNodeSystemSpeed* thisObj, unsigned int tick, void* matrix, unsigned int unk)
	{
		if (Globals::speedVariables == nullptr || thisObj->Type > 5 || thisObj->Type < 0)
			return;

		Globals::speedVariables[thisObj->Type] = thisObj->Speed;	
	}
};