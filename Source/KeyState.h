#pragma once
#include "Defines.h"

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