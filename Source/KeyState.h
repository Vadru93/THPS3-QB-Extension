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
		UP, DOWN, LEFT, RIGHT, SPINLEFT, NOLLIE, SPINRIGHT, REVERT, GRAB, FLIP, GRIND, OLLIE
	};

	void Press(float time)
	{
		pressed = 1;
		timepress = time;
		//holding = sensitivity;
	}

	void Unpress(float time)
	{
		pressed = 0;
		timerelease = time;
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