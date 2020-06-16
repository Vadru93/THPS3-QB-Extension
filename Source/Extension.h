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



#ifdef EXPORTING
#define EXTERN __declspec(dllexport)
#else
#define EXTERN __declspec(dllimport)
#pragma comment(lib, "LevelMod.lib")
#endif


enum enum_matrix
{
	RIGHT,
	UP,
	AT,
	POS,
};

enum enum_vertex
{
	X,
	Y,
	Z,
	W
};

#define _T(x) x

//where p = pointer to data
#define VALIDATE_PTR(p) \
{ \
if (InvalidReadPtr(p)) \
{ \
  MessageBox(0, _T("Parameter ") _T(#p) \
  _T(" is not a valid read pointer\r\n"), __FUNCTION__, 0); return NULL; \
} \
}

//where p = pointer to data s = size of data
#define VALIDATE_DATA(p, s) \
{ \
if (InvalidReadPtr(p, s)) \
{ \
  MessageBox(0, _T("Parameter ") _T(#p) \
  _T(" is not a valid read area\r\n"), __FUNCTION__, 0); return NULL; \
} \
}

struct CStruct;
struct CStructHeader;
struct CArray;

void EXTERN SetStructValues(CStructHeader* pStruct, CStructHeader* values);
void EXTERN SetArrayValues(CArray* pArray, CStructHeader* values);

bool EXTERN InvalidReadPtr(const void* const __restrict ptr, const DWORD size);
bool EXTERN InvalidReadPtr(const void* const __restrict ptr);


DWORD EXTERN GetTagCount();

void EXTERN SendChatMsg(char* text);

//Use this to be able draw your own things in present hook.
typedef HRESULT(__stdcall* Hooked_Present)(LPDIRECT3DDEVICE8 pDevice, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);
EXTERN Hooked_Present pHookedPresent;



inline unsigned long Checksum(const char* string)//THPS3 Function for CRC32
{
	unsigned long checksum;

	typedef unsigned long(__cdecl* pCRC32Address)(const char* string);
	pCRC32Address pCRC32 = (pCRC32Address)(0x004265F0);
	checksum = pCRC32(string);

	return checksum;
}
EXTERN struct Script;

EXTERN char* FindChecksumName(DWORD checksum);
EXTERN Script* qbTable;
#pragma pack(1)
struct EXTERN CScript
{
	BYTE* address;//address of where script - parser/executer is in script
	CStruct* params;//params sent when called the script
	void* extras;//extra stuff
	BYTE unk[0x370];
	//linked list
	CScript* next;
	CScript* prev;
	int nodeIndex;//-1 if no node
	bool spawned;
	BYTE unk3[3];
	DWORD unkPTR;//Maybe CObject??
	DWORD scriptChecksum;
	


	static CScript* GetNextScript(CScript* pScript = NULL)
	{
		if (pScript)
			return pScript->next;
		else
			return (CScript*)*(DWORD*)0x008E1DFC;
	}

	

	static DWORD GetNumCScripts()
	{
		return *(DWORD*)0x008E1E18;//NumCStruct *(DWORD*)0x008E1E34;
	}

	static void DumpScripts()
	{
		printf("###########################################################\n\n");
		printf("All the CScripts that currently exist ...\n\n");

		CScript* p_scr = GetNextScript();

		while (p_scr)
		{
			printf("CScript %s %X\n", FindChecksumName(p_scr->scriptChecksum), p_scr);

			p_scr = GetNextScript(p_scr);

		}
	}
};







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


struct QBKeyHeader//Used to store information about QBKeys (Type, Value/Address)
{
	const enum QBKeyType
	{
		UNDEFINED = 0,
		INT = 1,
		FLOAT = 2,
		STRING = 3,
		LOCAL_STRING = 4,
		PAIR = 5,
		VECTOR = 6,
		SCRIPTED_FUNCTION = 7,
		COMPILED_FUNCTION = 8,
		GLOBAL = 9,
		STRUCT = 10,
		LOCAL_STRUCT = 11,
		ARRAY = 12,
		LOCAL = 13,
	};

	int QBKeyCheck;//First 20 bit of the QBKey
	//Types:
	//0x0 - Undefined - End of array / struct and used when parsing / calling scripts
	//0x1 - Int
	//0x2 - Float
	//0x3 - String
	//0x4 - Local String - Used in struct/array	
	//0x5 - Pair
	//0x6 - Vector
	//0x7 - Scripted Function
	//0x8 - Compiled Function
	//0x9 - Global Variable/Function - Member Function in Thps4
	//0xA - Struct
	//0xB - Local Struct - Used in struct/array Structure Pointer in Thps4
	//0xC - Array
	//0xD - Local Variable/Function - Used in struct/array Name in Thps4
	QBKeyType type;
	int id;//Always 0x2?
	union
	{
		//This is the Value/Address of the QBKey ex:
		//if Type is int this is the value of the int
		//if Type is function this is the address of the function
		//values:
		union
		{
			float f;
			int i;
		} value;
		//pointers:
		char* pStr;
		CScript* pScript;
		D3DXVECTOR3* pVec;
		bool(*pFunction)(CStruct*, CScript*);
		CStructHeader** pStruct;
	};
	inline float GetFloat()
	{
		return value.f;
	}
	inline int GetInt()
	{
		return value.i;
	}
	inline char* GetString()
	{
		return pStr;
	}
	inline CScript* GetScript()
	{
		return pScript;
	}
	inline bool CallScript(CStruct* pStruct, CScript* pScript)
	{
		return pFunction(pStruct, pScript);
	}
	void SetValues(CStructHeader* values)
	{
		switch (type)
		{
		case STRUCT:
		case LOCAL_STRUCT:
			SetStructValues(*pStruct, values);
		}
	}
};


//Get information about a QBKey, first it will try with dll function and then ingame function. If no information is found it will return NULL
EXTERN QBKeyHeader* GetQBKeyHeader(unsigned long QBKey);

const char* QBTypes[] = {
  "None",//End of array / struct and used when parsing / calling scripts
  "Int",
  "Float",
  "String",
  "Local String",//Used in struct / array	
  "Pair",
  "Vector",
  "Scripted Function",
  "Compiled Function",
  "Global Variable/Function",//Member Function in Thps4
  "Struct",
  "Local Struct",//Used in struct / array Structure Pointer in Thps4
  "Array",
  "Local Variable/Function",//Used in struct / array Name in Thps4
  "Unknown",
  "Unknown",
  "Unknown",
  "Unknown",
  "Unknown"
};


//Add a QBkeyHeader to game array, this can be a value/function that you can acces/call in qb script.
typedef QBKeyHeader* (__cdecl* const RegisterQBKeyHeaderFunc)(const int QBKey, const int HeaderID);
const RegisterQBKeyHeaderFunc AddQBKeyHeader = (RegisterQBKeyHeaderFunc)(0x0042B900);

typedef QBKeyHeader* (__cdecl* const pFreex)(void const* const __restrict addr);
static const pFreex freex = (pFreex)(0x00411400);


void* __restrict mallocx(const DWORD buflen)
{
	void* __restrict allocated;
	//static const DWORD addr = 0x00471C61;
	static const DWORD mx = 0x004112F0;
	/*DWORD pMem = *(DWORD*)addr;
	while(pMem==0)
	pMem = *(DWORD*)addr;
	DWORD old = pMem;
	pMem = *(DWORD*)pMem;
	while(pMem==0)
	pMem = *(DWORD*)old;
	pMem+=4;
	old = pMem;
	pMem = *(DWORD*)pMem;
	while(pMem==0)
	pMem = *(DWORD*)old;
	*/
	_asm push 0
	_asm push 1
	_asm push buflen
	_asm mov esi, 0;
	_asm call mx
	/*_asm pop esi
	_asm mov esp, ebp
	_asm pop ebp
	_asm ret
	return NULL;*/
	_asm mov allocated, eax
	return allocated;
}


//structure to hold a function that you can call from QB
//
//To add your own functions, make an array like this: 
//CompiledScript scripts[] = { {"ScriptName",         CFunctionName}, etc... };
//
//Then make a function like this:
//
//for (int i = 0; i < sizeof(scripts) / sizeof(CompiledScript); i++)
//{
//    QBKeyHeader* header = AddQBKeyHeader(Checksum(scripts[i].name), 1);
//    if (header)
//    {
//        header->type = QBKeyHeader::COMPILED_FUNCTION;
//        header->pFunction = scripts[i].pFunction;
//    }
//    else
//        MessageBox(0, "couldn't add script", scripts[i].name, 0);
//}
struct CompiledScript
{
	const char* name;
	bool(*pFunction)(CStruct*, CScript*);
};


//Information about a QBKeys is stored in a few arrays inside the exe
struct CStructHeader
{
	QBKeyHeader::QBKeyType Type;
	int QBkey;
	union
	{
		union
		{
			float f;
			int i;
		} value;
		char* pStr;
		CScript* pScript;
		bool(*pFunction)(CStruct*, CScript*);
		//int Data;
		void* pData;
		D3DXVECTOR3* pVec;
		int Data;
		CStructHeader** pStruct;
		CArray* pArray;
		//This is the Value/Address of the QBKey ex:
		//if Type is int this is the value of the int
		//if Type is function this is the address of the function
	};
	inline float GetFloat()
	{
		return value.f;
	}
	inline int GetInt()
	{
		return value.i;
	}
	inline char* GetString()
	{
		return pStr;
	}
	inline CScript* GetScript()
	{
		return pScript;
	}
	bool GetStruct(DWORD checksum, CStructHeader** header)
	{
		if (QBkey == checksum)
		{
			*header = this;
			return true;
		}

		CStructHeader* pThis = NextHeader;
		while (pThis)
		{
			if (pThis->QBkey == checksum)
			{
				*header = pThis;
				return true;
			}
		}

		return false;
	}
	inline bool CallScript(CStruct* pStruct, CScript* pScript)
	{
		return pFunction(pStruct, pScript);
	}
	inline void SetChecksum(const char* string)
	{
		value.i = Checksum(string);
	}
	inline void SetName(const char* name)
	{
		QBkey = Checksum(name);
	}
	CStructHeader* NextHeader;

	CStructHeader()
	{
		Type = QBKeyHeader::QBKeyType::UNDEFINED;
		QBkey = 0;
		NextHeader = NULL;
	}

	CStructHeader(QBKeyHeader::QBKeyType type, int QBKey, float value, CStructHeader* next)
	{
		Type = type;
		this->QBkey = QBKey;
		this->value.f = value;
		NextHeader = next;
	}

	CStructHeader(QBKeyHeader::QBKeyType type, int QBKey, char* pStr, CStructHeader* next)
	{
		Type = type;
		this->QBkey = QBKey;
		this->pStr = pStr;
		NextHeader = next;
	}

	inline int GetNumItems()
	{
		int numItems = 1;
		if (NextHeader)
			numItems++;
		else return numItems;
		CStructHeader* next = NextHeader;
		while (next->NextHeader != NULL)
		{
			next = next->NextHeader;
			numItems++;
		}
		return numItems;
	}


	//use this function to set multiply values in 1 function call
	void SetValues(CStructHeader* values)
	{
		CStructHeader* value = this;
		while (value)
		{
			CStructHeader* newValue = values;
			while (newValue)
			{
				if (value->QBkey == newValue->QBkey)
				{
					switch (value->Type)
					{
					case QBKeyHeader::PAIR:
						memcpy(value->pData, newValue->pData, 8);
						break;
					case QBKeyHeader::LOCAL_STRING:
					case QBKeyHeader::STRING:
						strcpy(value->pStr, newValue->pStr);
						break;
					case QBKeyHeader::VECTOR:
						memcpy(value->pData, newValue->pData, 12);
						break;
					}
				}
				else
					newValue = newValue->NextHeader;
			}
			switch (value->Type)
			{
			case QBKeyHeader::QBKeyType::LOCAL_STRUCT:
			case QBKeyHeader::QBKeyType::STRUCT:
				value->pStruct[0]->SetValues(values);
			case QBKeyHeader::QBKeyType::ARRAY:
				SetArrayValues(value->pArray, values);
			}
			value = value->NextHeader;
		}
	}
};







struct EXTERN CArray
{
	CStructHeader** Items;
	WORD Type;
	WORD NumItems;


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

	bool AddString(char* String);

	bool AddCStruct(CStruct* Struct);

	void Free();
};





//calls a QB scripted function, remember to pass a CStruct.
typedef void(__cdecl* const ExecuteScript)(const char* __restrict name, const CStruct* __restrict pParams, const CScript* __restrict pScript, const void* unk);
__declspec(noalias) void inline ExecuteQBScript(const char* __restrict pScriptName, const CStruct* __restrict pParams, const CScript* __restrict pScript = NULL, const void* __restrict unk = NULL)
{
	return ExecuteScript(0x00428240)(pScriptName, pParams, pScript, unk);
}


//Set Element Text
inline void SetElementText(int id, char* text)
{
	static const DWORD pSetText = 0x004CE940;
	_asm push text
	_asm push id
	_asm call pSetText
}



//CStructHeader Container.
struct EXTERN CStruct
{
	CStructHeader* head;//the first added header aka parent = SOF CStruct
	CStructHeader* tail;//the last added header = EOF CStruct

	CStruct()
	{
		head = NULL;
		tail = NULL;
	}


	void RemoveParam(DWORD name);

	CStructHeader* AddParam(const char* name, QBKeyHeader::QBKeyType type);

	typedef CStructHeader* (__thiscall* const mallocx)(CStruct* pThis);
	inline CStructHeader* AllocateCStruct()
	{
		return mallocx(0x00428B90)(this);
	}
	typedef CStructHeader* (__cdecl* const freex)(CStructHeader* param);
	inline CStructHeader* DellocateCStruct(CStructHeader* param)
	{
		return freex(0x004260E0)(param);
	}
	typedef CStructHeader* (__cdecl* const freextail)(CStructHeader* param);
	inline CStructHeader* DellocateCStruct()
	{
		return freextail(0x004260E0)(tail);
	}


	//Game Functions in order

	typedef bool(__thiscall* addString)(CStruct* pThis, int name, QBKeyHeader::QBKeyType type, const void* data);
	bool inline AddParam(int name, QBKeyHeader::QBKeyType type, const void* data)
	{
		return addString(0x00428BF0)(this, name, type, data);
	}

	inline CStructHeader* AddChecksum(int ParamName, int Data, int qbType)
	{
		//MessageBox(0,"Adding Checksum","Adding Checksum",MB_OK);
		int rett;
		static const DWORD pAddParam = 0x00428C80;

		_asm push Data;
		_asm push qbType;
		_asm push ParamName;
		_asm mov ecx, this;
		_asm call pAddParam;
		_asm mov rett, eax;
		return (CStructHeader*)rett;
	}

	/*typedef  bool (__stdcall *CStructMemberFunction)(void* Param, void* dest, int type );
	CStructMemberFunction pGetString;*/
	inline char* GetString()//use ONLY if your sure you got the param
	{
		static const DWORD pGetString = 0x00429690;
		char* pstr = NULL;
		_asm lea eax, pstr;
		_asm push 0;
		_asm push eax;
		_asm push 0;
		_asm mov ecx, this;
		_asm call pGetString;
		return pstr;
	}

	inline char* GetString(const char* Param)//use ONLY if your sure you got the param
	{
		static const DWORD pGetString = 0x00429760;
		char* pstr = NULL;
		_asm lea eax, pstr;
		_asm push 0;
		_asm push eax;
		_asm push Param;
		_asm mov ecx, this;
		_asm call pGetString;
		return pstr;
	}

	inline bool GetString(const char** dest)
	{
		bool rett;
		static const DWORD pGetString = 0x00429690;
		//char* pstr=NULL;
		//_asm lea eax, dest;
		_asm push 0;
		_asm push dest;
		_asm push 0;
		_asm mov ecx, this;
		_asm call pGetString;
		_asm mov rett, al
		return rett;
	}

	inline bool GetString(const char* Param, const char** dest)
	{
		bool rett;
		static const DWORD pGetString = 0x00429760;
		_asm push 0;
		_asm push dest;
		_asm push Param;
		_asm mov ecx, this;
		_asm call pGetString;
		_asm mov rett, al
		return rett;
	}

	inline int GetChecksum(const char* Param)//use ONLY if your sure you got the param
	{
		static const DWORD pGetChecksum = 0x00429840;
		int pint = 0;
		_asm lea eax, pint
		_asm push 0;
		_asm push eax;
		_asm push Param;
		_asm mov ecx, this;
		_asm call pGetChecksum;
		return pint;
	}

	inline bool GetChecksum(const char* Param, int* dest)
	{
		bool rett;
		static const DWORD pGetChecksum = 0x00429840;
		_asm push 0;
		_asm push dest;
		_asm push Param;
		_asm mov ecx, this
		_asm call pGetChecksum;
		_asm mov rett, al;
		return rett;
	}

	inline int GetInt()//use ONLY if your sure you got the param
	{
		static const DWORD pGetInt = 0x00429890;
		int pint = 0;
		_asm lea eax, pint;
		_asm push 0;
		_asm push eax;
		_asm push 0;
		_asm mov ecx, this
		_asm call pGetInt;
		return pint;
	}

	inline int GetInt(const char* Param)//use ONLY if your sure you got the param
	{
		static const DWORD pGetInt = 0x00429920;
		int pint = 0;
		_asm lea eax, pint;
		_asm push 0;
		_asm push eax;
		_asm push Param;
		_asm mov ecx, this
		_asm call pGetInt;
		return pint;
	}

	typedef bool(__thiscall* getInt)(CStruct* pThis, int unk1, int* dest, int unk2);
	inline bool GetInt(int* dest)
	{
		return getInt(0x00429890)(this, 0, dest, 0);
	}

	inline bool GetInt(const char* Param, int* dest)
	{
		bool rett;
		static const DWORD pGetInt = 0x00429920;
		_asm push 0;
		_asm push dest;
		_asm push Param;
		_asm mov ecx, this
		_asm call pGetInt;
		_asm mov rett, al
		return rett;
	}


	bool GetVector(int checksum, D3DXVECTOR3* dest)
	{
		bool rett;
		static const DWORD pGetVector = 0x00429970;
		_asm push 0;
		_asm push dest;
		_asm push checksum;
		_asm mov ecx, this
		_asm call pGetVector
		_asm mov rett, al;
		return rett;
	}

	inline bool GetFloat(const char* Param, float* dest)
	{
		bool rett;
		static const DWORD pGetFloat = 0x00429a10;
		_asm push 0;
		_asm push dest;
		_asm push Param;
		_asm mov ecx, this
		_asm call pGetFloat;
		_asm mov rett, al;
		return rett;
	}

	inline bool GetPair(const char* Param, void* dest)
	{
		bool rett;
		static const DWORD pGetPair = 0x00429b00;// <- points to 00429a60
		_asm push 1;
		_asm push dest;
		_asm push Param;
		_asm mov ecx, this
		_asm call pGetPair;
		_asm mov rett, al;
		return rett;
	}

	//GetInt/Float 00429b50---references to struct?
	//00429c10 points to ^


	inline bool GetStruct(int checksum, CStruct** dest)
	{
		bool rett;
		static const DWORD pGetStruct = 0x00429C60;
		_asm push 1;
		_asm push dest;
		_asm push checksum;
		_asm mov ecx, this;
		_asm call pGetStruct;
		_asm mov rett, al;
		return rett;
	}

	inline bool GetStruct(const char* Param, void** dest)
	{
		bool rett;
		static const DWORD pGetStruct = 0x00429CE0;
		_asm push 0
		_asm push dest
		_asm push Param
		_asm mov ecx, this
		_asm call pGetStruct
		_asm mov rett, al
		return rett;
	}

	inline bool GetScript(int* dest)
	{
		bool rett;
		static const DWORD pGetScript = 0x00429D30;
		_asm push 0
		_asm push dest
		_asm push 0
		_asm mov ecx, this;
		_asm call pGetScript
		_asm mov rett, al;
		return rett;
	}

	inline bool GetScript(const char** dest)
	{
		bool rett;
		static const DWORD pGetScript = 0x00429DC0;
		_asm push 0
		_asm push dest
		_asm push 0
		_asm mov ecx, this;
		_asm call pGetScript
		_asm mov rett, al;
		return rett;
	}

	inline bool GetScript(const char* script, const char** dest)
	{
		bool rett;
		static const DWORD pGetScript = 0x00429DC0;
		_asm push 1
		_asm push dest
		_asm push script
		_asm mov ecx, this;
		_asm call pGetScript
		_asm mov rett, al;
		return rett;
	}

	inline bool GetScript(const char* script, const int* dest)
	{
		bool rett;
		static const DWORD pGetScript = 0x00429DC0;
		_asm push 1
		_asm push dest
		_asm push script
		_asm mov ecx, this;
		_asm call pGetScript
		_asm mov rett, al;
		return rett;
	}

	inline bool GetArray(const char* Param, const CArray** dest)
	{
		bool rett;
		static const DWORD pGetArray = 0x00429F00;
		_asm push 0;
		_asm push dest;
		_asm push Param;
		_asm mov ecx, this;
		_asm call pGetArray;
		_asm mov rett, al;
		return rett;
	}


	inline bool ContainsFlag(const char* Param)
	{
		bool rett;
		static const DWORD pContainsFlag = 0x00429FC0;
		_asm push Param;
		_asm mov ecx, this
		_asm call pContainsFlag;
		_asm mov rett, al;
		return rett;
	}
	//EOF Game Functions



	//My Functions
	void FreeHeader();

	void Free();

	bool GetStruct(DWORD checksum, CStructHeader** header)
	{
		return head->GetStruct(checksum, header);
	}
	//EOF My Functions
};


//seems to be broken in Openspy...
void SendPm(char* who, const char* message)
{
	typedef void(__thiscall* const pSendPm)(void* pNetProfile, char* who, const char* message, DWORD unk);
	pSendPm(0x004E0F20)(*(void**)0x008F14F4, who, message, 0);
}


//Skater is not yet looked into that much.... 
struct Settings
{
	BYTE unk[256];
	typedef bool(__thiscall* const pIsTrue)(Settings* pThis, const char* name);
	bool IsTrue(const char* name)
	{
		return pIsTrue(0x004342E0)(this, name);
	}
};

struct SkaterProfile
{
	BYTE unk[256];

	typedef Settings* (__thiscall* const pGetSettings)(SkaterProfile* pThis);

	Settings* GetSettings()
	{
		return pGetSettings(0x00438B00)(this);
	}
};

struct SkaterProfileContainer
{
	SkaterProfile* profile;

	typedef SkaterProfile* (__cdecl* const GetSkaterProfile)(bool allocate);

	SkaterProfileContainer()
	{
		profile = GetSkaterProfile(0x004367E0)(false);//increas vp count
	}

	typedef void(__cdecl* const ReleaseSkaterProfile)();

	~SkaterProfileContainer()
	{
		ReleaseSkaterProfile(0x00436830)();//decrease vp count
	}
};


typedef void(__thiscall* const pOrthoNormalizeAbout)(D3DXMATRIX* pThis, DWORD axis);
void OrthoNormalizeAbout(D3DXMATRIX* matrix, DWORD axis)
{

	pOrthoNormalizeAbout(0x0040F0E0)(matrix, axis);
}

EXTERN struct Skater
{
private://0575a190
	DWORD memberFunctions;
	byte unk1[0x14];
	D3DXVECTOR3 position;
	float positionW;//1.0f
	D3DXVECTOR3 oldpos;
	float oldposW;//1.0f
	byte unk4[0x2FC];
	D3DXVECTOR3 velocity;
	byte unk5[0x14];
	D3DXMATRIX matrix;
	byte unknown5[0x3F];
	bool landedfromvert;
	bool truelandedfromvert;
	byte unknown4[0x7F4B];
	bool inVert;
	byte unknown6[0xF];
	bool tracking;
	byte unknown7[0x1F];
	bool canbreakvert;
	byte unknown8[0x4F];//1 more
	bool autoturn;
	byte unknown[0x57];//1 more
	D3DXMATRIX  old;
	D3DXMATRIX  lerp;//1 more
	byte unknown2[0x58];
	KeyState keystates[12];
	byte unknown3[0x3C];
	D3DXVECTOR3 startcol;
	float startcolW;//1.0f
	D3DXVECTOR3 endcol;
	float endcolW;//1.0f
	void* scripts;
	DWORD flag;//0 1 idk?
	float unk;//some normal??
	D3DXVECTOR3 normal;
	D3DXVECTOR3 hitpoint;
	byte unk3[0x134];
	D3DXVECTOR4 displaynormal;
	D3DXVECTOR4 currentnormal;
	D3DXVECTOR4 lastdisplaynormal;
	float wtf;
	float framelength;
	float un1;
	float un2;
	float normallerp;

#pragma pop(pack)
public:

	typedef void(__thiscall* const pSetNormal)(Skater* pThis, D3DXVECTOR4& normal);
	void SetNormal(D3DXVECTOR4& normal)
	{

		D3DXMATRIX* pMatrix = GetMatrix();
		displaynormal = *(D3DXVECTOR4*)pMatrix->m[Y];
		currentnormal = normal;
		lastdisplaynormal = displaynormal;
		normallerp = 1.0f;
		*(D3DXVECTOR4*)pMatrix->m[Y] = normal;
		OrthoNormalizeAbout(GetMatrix(), Y);
	}


	typedef bool(__thiscall* const pCallMemberFunction)(Skater* pThis, DWORD checksum, CStruct* pStruct, CScript* pScript);
	bool CallMemberFunction(DWORD checksum, CStruct* pStruct, CScript* pScript)
	{
		DWORD function = *(DWORD*)memberFunctions + 4;
		function = *(DWORD*)function;
		return pCallMemberFunction(function)(this, checksum, pStruct, pScript);
	}


	bool IsTracking()
	{
		return tracking;
	}

	bool InVert()
	{
		return inVert;
	}
	void SetLanded(bool value)
	{
		landedfromvert = value;
		truelandedfromvert = value;
	}

	KeyState* GetKeyState(DWORD idx)
	{
		if (idx > 11)
			idx = 11;
		return &keystates[idx];
	}

	float GetFrameLength()
	{
		return framelength;
	}

	static Skater* GetSkater()
	{
		static const DWORD ptr = 0x005D06C0;
		VALIDATE_PTR((void*)ptr);
		DWORD pSkater = *(DWORD*)ptr + 0x580;
		VALIDATE_PTR((void*)pSkater);
		pSkater = *(DWORD*)pSkater + 0x4;
		VALIDATE_PTR((void*)pSkater);
		pSkater = *(DWORD*)pSkater + 0x2C;
		VALIDATE_PTR((void*)pSkater);
		pSkater = *(DWORD*)pSkater + 0x48;
		VALIDATE_PTR((void*)pSkater);
		pSkater = *(DWORD*)pSkater;
		VALIDATE_DATA((Skater*)pSkater, sizeof(Skater));
		return (Skater*)pSkater;
	}

	D3DXMATRIX* GetMatrix()
	{
		return &matrix;
	}


	//used for spine transfer, don't call this
	EXTERN void Store();

	//used for spine transfer, don't call this
	EXTERN void Restore();

	//used for spine transfer, don't call this
	EXTERN void Slerping();

	//used for spine transfer, don't call this
	EXTERN void MultiplyMatrix(const D3DXMATRIX& delta);

	//used for spine transfer, don't call this
	EXTERN bool CheckForSpine();

	//used for spine transfer, don't call this
	EXTERN void ResetLerpingMatrix();


	D3DXVECTOR3* GetCollisionNormal()
	{
		return &normal;
	}



	//Tell Physcs we are inside vert air
	EXTERN void SetVertAir(bool value);

	typedef void(__thiscall* const pTriggerScript)(Skater* pThis, DWORD triggerType, void*);
	void TriggerScript(DWORD triggerType)
	{
		pTriggerScript(0x0049D070)(this, triggerType, scripts);
	}


	//call this before CheckCollision to set the ray for raytracing.
	EXTERN void SetRay(D3DXVECTOR3 start, D3DXVECTOR3 end);

	void SetNormals()
	{
		*(D3DXVECTOR3*)&currentnormal = normal;
		currentnormal.w = 1.0f;
	}

	//clear 
	typedef void(__thiscall* const pFlagException)(Skater* pThis, const char* name, DWORD unk2);
	void FlagException(const char* name, DWORD unk2 = 0)
	{
		pFlagException(0x0048F990)(this, name, unk2);
	}

	EXTERN bool CollisionCheck(BYTE flag, bool ignore = true);

	D3DXVECTOR3* GetNormal()
	{
		return &*(D3DXVECTOR3*)&currentnormal;
	}


	D3DXVECTOR3* GetHitPoint()
	{
		return &hitpoint;
	}


	D3DXVECTOR3* GetOldPosition()
	{
		return &oldpos;
	}

	D3DXVECTOR3* GetPosition()
	{
		return &position;
	}


	void SetPosition(D3DXVECTOR3 pos)
	{
		oldpos = position;
		position = pos;

	}


	D3DXVECTOR3* GetVelocity()
	{
		/*DWORD pSpeed = (DWORD)this + 0x334;
		VALIDATE_DATA((D3DXVECTOR3*)pSpeed, sizeof(D3DXVECTOR3));

		return (D3DXVECTOR3*)pSpeed;*/
		return &velocity;
	}
};



//Menu Element
struct Element
{
	BYTE unk[256];//not yet disambled
	typedef Element* (__thiscall* const pGetElement)(Element* pThis, DWORD name, DWORD id);

	Element* GetElement(DWORD name, DWORD id = 1)
	{
		return pGetElement(0x004D23B0)(this, name, id);
	}
	DWORD GetValue()
	{
		typedef DWORD(__thiscall* const GetElementValue)(Element* pThis);
		return GetElementValue(0x004D54B0)(this);
	}
};


typedef Element* (__cdecl* const pAllocateElement)(DWORD unk);
static const pAllocateElement AllocateElement = (pAllocateElement)(0x004D12A0);
typedef void(__cdecl* const pFreeElement)();
static const pFreeElement FreeElement = (pFreeElement)(0x004D12F0);
typedef void* (__cdecl* const pCastPointer)(void* pointer, LONG VfDelta, DWORD SrcType, DWORD TargetType, BOOL isReference);
static const pCastPointer CastPointer = (pCastPointer)(0x00577E58);


DWORD GetElementSliderValue(DWORD name)
{
	_asm pushad;
	Element* container = AllocateElement(0);
	Element* element = container->GetElement(name);
	element = (Element*)CastPointer((void*)element, 0, 0x005B6344, 0x005B6638, FALSE);
	DWORD value = element->GetValue();
	FreeElement();
	_asm popad;
	return value;
}


//LevelMod Example Script
//Get's the slider value of a menu element, check optionsmenu.qb
bool GetSliderValue(CStruct* pStruct, CScript* pScript)
{
	int name = 0;
	pStruct->GetScript("id", &name);
	int paramName = 0;
	if (pStruct->GetScript("name", &paramName))
	{
		CStructHeader* param = NULL;

		if (pScript->params->GetStruct(paramName, &param))
		{
			param->value.f = (float)GetElementSliderValue(name);
		}
		else
			return false;
	}
	else
	{
		CStructHeader* param = pScript->params->AddParam("value", QBKeyHeader::FLOAT);
		param->value.f = (float)GetElementSliderValue(name);
	}
	return true;
}

//LevelMod Example Script
//Change the values of a struct, check optionsmenu.qb
bool ChangeValues(CStruct* pStruct, CScript* pScript)
{
	int container = 0;

	if (pStruct->GetScript("Container", &container))
	{
		QBKeyHeader* pContainer = GetQBKeyHeader(container);
		if (pContainer)
		{
			CStructHeader* values = NULL;

			if (pStruct->GetStruct(Checksum("Values"), &values))
			{
				pContainer->SetValues(*values->pStruct);
				return true;
			}
		}
	}
	return false;
}




//is in graf game?
bool Modulating()
{
	SkaterProfileContainer skater;
	if (skater.profile)
	{
		Settings* settings = skater.profile->GetSettings();
		if (settings)
			return settings->IsTrue("should_modulate_color");
	}
	return false;
}
