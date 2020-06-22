#pragma once
#include <d3dx8.h>
#include <d3dx8core.h>



typedef void(__thiscall* const pOrthoNormalizeAbout)(D3DXMATRIX* pThis, DWORD axis);
void OrthoNormalizeAbout(D3DXMATRIX* matrix, DWORD axis)
{

	pOrthoNormalizeAbout(0x0040F0E0)(matrix, axis);
}