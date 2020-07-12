#pragma once
#include "Script.h"
struct CArray;
struct CStructHeader;

struct Node
{
	DWORD pMemberFunction;
	DWORD pNULL;
	DWORD numUnk1;
	DWORD flags;//Maybe?
	DWORD unknown[2];//NULL?
	D3DXVECTOR3 position;
	BYTE unknown2[52];
	DWORD name;

	static CArray* GetNodeArray()
	{
		typedef CArray*(__cdecl* const pGetNodeArray)(DWORD checksum, DWORD param);
		printf("NodeArray %p", pGetNodeArray(0x00426590)(0xC472ECC5, 1));
		return pGetNodeArray(0x00426590)(0xC472ECC5, 1);
	}

	CStructHeader* GetNodeStruct()
	{
		return GetNodeArray()->GetNodeStruct(GetNodeIndex(name));
	}

	
	static CStructHeader* GetNodeStructByIndex(DWORD index)
	{
		return GetNodeArray()->GetNodeStruct(index);
	} 
	static CStructHeader* GetNodeStruct(DWORD checksum)
	{
		return GetNodeArray()->GetNodeStruct(GetNodeIndex(checksum));
	}

	static DWORD GetNodeIndex(DWORD checksum)
	{
		typedef DWORD (__cdecl* const pGetNodeIndex)(DWORD checksum);
		return pGetNodeIndex(0x0042B2B0)(checksum);
	}
};
