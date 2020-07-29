#pragma once
#ifndef _KeyState_H_
#define _KeyState_H_
#include "Defines.h"
#include "source/CXBOXController.h"
struct Skater;
EXTERN CXBOXController* Player1;




struct KeyState
{
private:
	DWORD pressed;
	float timepress;
	float timerelease;
	DWORD holding;//0xFF
	DWORD unk1[2];//1
	DWORD checksum;

public:
	enum
	{
		NONE = -1, UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3, SPINLEFT = 4, NOLLIE = 5, SPINRIGHT = 6, REVERT = 7, GRAB = 8, FLIP = 9, GRIND = 10, OLLIE = 11
	};

	void Update(DWORD press)//the press is between 0x0-0xFF, press below or equal to 0x40 is deadzone
	{
		_printf("Updating press %d\nKeyState %p chc %X\n", press, this, this->checksum);
		typedef void(__thiscall* const pUpdate)(KeyState* pThis, DWORD press);
		pUpdate(0x0049BAA0)(this, press);
	}


	float GetReleasedTime()
	{
		return timerelease;
	}

	float GetPressedTime()
	{
		return timepress;
	}

	bool IsPressed()
	{
		return pressed != 0;
	}

};


EXTERN void ProxyPad(Skater* skater);

#endif