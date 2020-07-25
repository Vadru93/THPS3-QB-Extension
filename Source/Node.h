#pragma once
#ifndef NODE_H
#define NODE_H
#include "Script.h"
struct CArray;
struct CStructHeader;

//CArray, used for all scripted arrays including NodeArray
struct EXTERN CArray
{
	union
	{
		CStructHeader** Items;
		NodeContainer* container;
	};
	WORD Type;
	WORD NumItems;

	int& operator[] (int x) {
		if (Type != 1)
		{
			_printf("Unsupported ArrayType\n");
			return x;
		}
		if (x > NumItems)
			_printf("###TOOO HIGH INDEX IN CARRAY!!!###\n");
		x &= NumItems;
		return container->index;
	}

	int GetNumItems();
	void SetValues(CStructHeader* values);
	inline float GetFloat(int Item)
	{
		if (Type == 0x0A || Type == 0x0B)
		{
			return *(float*)&((CStructHeader***)Items)[Item][0]->Data;
		}
		return ((float*)Items)[Item];
	}

	inline float GetFloat(int Item, int Itm)
	{
		if (Type == 0x0A || Type == 0x0B)
		{
			return *(float*)((CStructHeader***)Items)[Item][Itm]->Data;
		}
		else return 0.0;
	}

	inline int GetInt(int Item)
	{
		if (Type == 0x0A || Type == 0x0B)
			return ((CStructHeader***)Items)[Item][0]->Data;
		return ((int*)Items)[Item];
	}

	inline int GetInt(int Item, int Itm)
	{
		if (Type == 0x0A || Type == 0x0B)
			return ((CStructHeader***)Items)[Item][Itm]->Data;
		else return 0;
	}

	inline char* GetString(int Item)
	{
		if (Type == 0x0A || Type == 0x0B)
			return (char*)((CStructHeader***)Items)[Item][0]->pData;
		return ((char**)Items)[Item];
	}

	inline char* GetString(int Item, int Itm)
	{
		if (Type == 0x0A || Type == 0x0B)
			return (char*)((CStructHeader***)Items)[Item][Itm]->pData;
		return 0;
	}

	inline D3DXVECTOR3* GetVector(int Item)
	{
		if (Type == 0x0A || Type == 0x0B)
			return (D3DXVECTOR3*)((CStructHeader***)Items)[Item][0]->pData;
		return ((D3DXVECTOR3**)Items)[Item];
	}

	inline D3DXVECTOR3* GetVector(int Item, int Itm)
	{
		if (Type == 0x0A || Type == 0x0B)
			return (D3DXVECTOR3*)((CStructHeader***)Items)[Item][Itm]->pData;
		return 0;
	}

	inline void* GetPair(int Item)
	{
		if (Type == 0x0A || Type == 0x0B)
			return (void*)((CStructHeader***)Items)[Item][0]->pData;
		return ((void**)Items)[Item];
	}

	inline void* GetPair(int Item, int Itm)
	{
		if (Type == 0x0A || Type == 0x0B)
			return (void*)((CStructHeader***)Items)[Item][Itm]->pData;
		return 0;
	}

	inline CStructHeader** GetCStructs(int Item)
	{
		return ((CStructHeader***)Items)[Item];
	}

	CStructHeader* GetCStruct(int Name, int Item, int Struct);

	inline CStructHeader* GetCStruct(char* Name, int Item, int Struct)
	{
		return GetCStruct(Checksum(Name), Item, Struct);
	}

	CStructHeader* GetCStruct(int Name, int Item, int* outItem);

	inline CStructHeader* GetCStruct(char* Name, int Item, int* outItem)
	{
		return GetCStruct(Checksum(Name), Item, outItem);
	}

	CStructHeader* GetCStruct(DWORD index)
	{
		if (index > 0xFFFF)
			_printf("###WARNING INDEX TOO HIGH###\n");
		index &= 0xFFFF;
		_printf("Index %d %p\n", index, Items[index]);
		return *(CStructHeader**)Items[index];
	}

	bool AddString(char* String);

	bool AddCStruct(CStruct* Struct);

	void Free();
};


struct Node
{
	DWORD pMemberFunctions;
	DWORD pNULL;
	DWORD numUnk1;
	DWORD flags;//Maybe?
	DWORD unknown[2];//NULL?
	D3DXVECTOR3 position;
	BYTE unknown2[52];
	DWORD name;//checksum


	//Gets pointer to the NodeArray, type is CArray(See Above)
	static CArray* GetNodeArray()
	{
		typedef CArray*(__cdecl* const pGetNodeArray)(DWORD checksum, DWORD param);
		_printf("NodeArray %p\n", pGetNodeArray(0x00426590)(0xC472ECC5, 1));
		return pGetNodeArray(0x00426590)(0xC472ECC5, 1);
	}

	//Gets CStructHeader of this node
	CStructHeader* GetNodeStruct()
	{
		return GetNodeArray()->GetCStruct(GetNodeIndex(name));
	}

	//Gets the CStructHeader of a node
	static CStructHeader* GetNodeStructByIndex(DWORD index)
	{
		return GetNodeArray()->GetCStruct(index);
	} 
	
	//Gets the CStructHeader of a node
	static CStructHeader* GetNodeStruct(DWORD checksum)
	{
		return GetNodeArray()->GetCStruct(GetNodeIndex(checksum));
	}

	//Gets the index of a node in NodeArray
	static DWORD GetNodeIndex(DWORD checksum)
	{
		typedef DWORD (__cdecl* const pGetNodeIndex)(DWORD checksum);
		return pGetNodeIndex(0x0042B2B0)(checksum);
	}
};
#endif