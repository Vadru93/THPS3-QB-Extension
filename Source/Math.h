#pragma once
#include "windows.h"
#include <conio.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include "Psapi.h"
#include <string.h>
#include <iostream>
#include <WinSock2.h>
#include "Shellapi.h"
#include "vector"
#include <tlhelp32.h>
#include <d3dx8.h>
#include <d3dx8core.h>
#include <sstream>
#include <map>
#include "Checksum.h"


typedef void(__thiscall* const pOrthoNormalizeAbout)(D3DXMATRIX* pThis, DWORD axis);
void OrthoNormalizeAbout(D3DXMATRIX* matrix, DWORD axis)
{

	pOrthoNormalizeAbout(0x0040F0E0)(matrix, axis);
}