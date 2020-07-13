#pragma once
#include "defines.h"
#include <d3dx8.h>
#include <d3dx8core.h>



typedef void(__thiscall* const pOrthoNormalizeAbout)(D3DXMATRIX* pThis, DWORD axis);
void OrthoNormalizeAbout(D3DXMATRIX* matrix, DWORD axis)
{

	pOrthoNormalizeAbout(0x0040F0E0)(matrix, axis);
}


D3DXVECTOR3& Transform(D3DXVECTOR3& V, D3DXMATRIX& M)
{
	D3DXVECTOR3 Out;
	float x = V.x * M._11 + V.y * M._21 + V.z * M._31 + M._41;
	float y = V.x * M._12 + V.y * M._22 + V.z * M._32 + M._42;
	float z = V.x * M._13 + V.y * M._23 + V.z * M._33 + M._43;
	//float w = V.x * M._14 + V.y * M._24 + V.z * M._34 + M._44; <- don't need scaling for now
	
	Out.x = x;
	Out.y = y;
	Out.z = z;
	return Out;
}

