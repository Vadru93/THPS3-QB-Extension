#pragma once
#ifndef SCRIPT_H
#define SCRIPT_H
#include "Defines.h"
#include "Checksum.h"
#include "source/QBKey.h"


struct CStruct;
struct CStructHeader;
struct CArray;
struct CScript;
//struct QBKeyHeader;


EXTERN struct Script;
EXTERN Script* qbTable;

void EXTERN SetStructValues(CStructHeader* pStruct, CStructHeader* values);
void EXTERN SetArrayValues(CArray* pArray, CStructHeader* values);

//calls a QB scripted function, remember to pass a CStruct.
typedef void(__cdecl* const ExecuteScript)(const char* __restrict name, const CStruct* __restrict pParams, const CScript* __restrict pScript);
__declspec(noalias) void inline ExecuteQBScript(const char* __restrict pScriptName, const CStruct* __restrict pParams, const CScript* __restrict pScript = NULL)
{
	return ExecuteScript(0x00428240)(pScriptName, pParams, pScript);
}

//Add a QBkeyHeader to game array, this can be a value/function that you can acces/call in qb script.
typedef QBKeyHeader* (__cdecl* const RegisterQBKeyHeaderFunc)(const int QBKey, const int HeaderID);
const RegisterQBKeyHeaderFunc AddQBKeyHeader = (RegisterQBKeyHeaderFunc)(0x0042B900);

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
struct Node;

//pack(1) to make sure alignment is correct when try to acess game data
//#pragma pack(1)
struct EXTERN CScript
{
	BYTE* address;//address of where script - parser/executer is in script
	CStruct* params;//params sent when called the script
	void* extras;//extra stuff
	BYTE unk[0x370];//not looked into
	//linked list
	CScript* next;
	CScript* prev;
	int nodeIndex;//-1 if no node
	bool spawned;
	BYTE padding[3];
	Node* node;
	DWORD scriptChecksum;//The name of the script

	CScript()
	{
		ZeroMemory(this, sizeof(CScript));
	}

	bool GotParam(DWORD name);
	CStructHeader* GetParam(DWORD name);

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
		_printf("###########################################################\n\n");
		_printf("All the CScripts that currently exist ...\n\n");

		CScript* p_scr = GetNextScript();

		while (p_scr)
		{
			_printf("CScript %s %X\n", FindChecksumName(p_scr->scriptChecksum), p_scr);

			p_scr = GetNextScript(p_scr);

		}
		_printf("\n");
	}

	DWORD GetNodeName();
	
};
//compressed map
struct QBKeyInfo
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

	QBKeyType type;
	DWORD shiftedKey;
	QBKeyInfo* next;
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
};


struct QBKeyInfoContainer
{
	QBKeyInfo* first;
	QBKeyInfo* last;
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


//Information about a QBKeys is stored in a few arrays inside the exe
struct CStructHeader
{
	QBKeyHeader::QBKeyType Type;
	int QBkey;
	const union
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

	
	CStructHeader(QBKeyHeader::QBKeyType Type, DWORD checksum, void* data)
	{
		this->Type = Type;
		this->QBkey = checksum;
		this->pData = data;
		NextHeader = NULL;
	}

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

	CArray* GetArray(DWORD checksum)
	{
		if (this->QBkey == checksum)
		{
			_printf("Array? %p %p\n", this, this->pArray);
			return this->pArray;
		}
		CStructHeader* header = this->NextHeader;
		while (header)
		{
			if (header->QBkey == checksum)
			{
				_printf("Array? %p %p\n", header, header->pArray);
				return header->pArray;
			}
			header = header->NextHeader;
		}
		return NULL;
	}

	D3DXVECTOR3* GetVector(DWORD checksum)
	{
		if (QBkey == checksum)
		{
			return pVec;
		}

		CStructHeader* pThis = NextHeader;
		while (pThis && !InvalidReadPtr(pThis) && !InvalidReadPtr(pThis, sizeof(CStructHeader)))
		{
			if (pThis->QBkey == checksum)
			{
				return pThis->pVec;
			}
			pThis = pThis->NextHeader;
		}
		return NULL;
	}

	bool GetStruct(DWORD checksum, CStructHeader** header)
	{
		if (QBkey == checksum)
		{
			*header = this;
			return true;
		}

		CStructHeader* pThis = NextHeader;
		while (pThis && !InvalidReadPtr(pThis) && !InvalidReadPtr(pThis, sizeof(CStructHeader)))
		{
			if (pThis->QBkey == checksum)
			{
				*header = pThis;
				return true;
			}
			pThis = pThis->NextHeader;
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
		Data = 0;
		NextHeader = NULL;
	}

	CStructHeader(QBKeyHeader::QBKeyType type, int _QBKey, float _value, CStructHeader* next)
	{
		Type = type;
		this->QBkey = _QBKey;
		this->value.f = _value;
		NextHeader = next;
	}

	CStructHeader(QBKeyHeader::QBKeyType type, int _QBKey, char* _pStr, CStructHeader* next)
	{
		Type = type;
		this->QBkey = _QBKey;
		this->pStr = _pStr;
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

/*struct NodeContainer
{
	int index;
	D3DXVECTOR3 pos;
};*/


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

	
	//These are injected into game source and should not be called seperatly
	//The compressed nodes are generated from the .q file upon loading of the [levelname].qb
	//if the .q file exists and starts with 0xAA, it will try to generate the compressed nodes
	BYTE* MaybeAddCompressed(DWORD qbKey, BYTE* pFile, QBKeyInfoContainer* other);
	void AddCompressedNode(DWORD checksum, QBKeyInfoContainer* container);
	

	CStructHeader* AddParam(const char* name, QBKeyHeader::QBKeyType type);

	typedef CStructHeader* (__cdecl* const mallocx)();
	static CStructHeader* AllocateCStruct(bool Allocate=false)
	{
		if (Allocate)
		{
			bool oldValue = *(bool*)0x008E1DF8;
			*(bool*)0x008E1DF8 = true;
			CStructHeader* temp = mallocx(0x00428B90)();
			*(bool*)0x008E1DF8 = oldValue;
			return temp;
		}
		return mallocx(0x00428B90)();
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

	CStructHeader* AddCStruct(DWORD name, QBKeyHeader::QBKeyType type, DWORD data)
	{
		typedef CStructHeader* (__thiscall* pAddCStruct)(CStruct* pThis, int name, QBKeyHeader::QBKeyType type, DWORD data);
		return pAddCStruct(0x00428C80)(this, name, type, data);
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


	bool GetVector(int checksum, D3DXVECTOR3** dest)
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

	/*inline bool GetScript(char* name, const char** dest)
	{
		bool rett;
		static const DWORD pGetScript = 0x00429DC0;
		_asm push 1
		_asm push dest
		_asm push name
		_asm mov ecx, this;
		_asm call pGetScript
		_asm mov rett, al;
		return rett;
	}*/

	inline bool GetScript(const char* script, const char** dest)
	{
		bool rett;
		static const DWORD pGetScript = 0x00429DC0;
		_asm push 1
		_asm push dest
		_asm push script
		_asm mov ecx, this;
		_asm call pGetScript
		_asm mov rett, al; 0049D0B3
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

	void AddParam(CStructHeader* pParam)
	{
		head = pParam;
		tail = pParam;
	}

	void FreeHeader();

	void Free();

	bool GetStruct(DWORD checksum, CStructHeader** header)
	{
		CStructHeader* pThis = head;
		while (pThis && pThis != tail)
		{
			if (pThis->QBkey == checksum)
			{
				*header = pThis;
				return true;
			}	
			if (pThis->NextHeader)
				pThis = pThis->NextHeader;
			else
				pThis++;
			
		}
		if (pThis && pThis->QBkey == checksum)
		{
			*header = pThis;
			return true;
		}
		return false;
		//return head->GetStruct(checksum, header);
	}

	D3DXVECTOR3* GetVector(const char* param)
	{
		return head->GetVector(Checksum(param));
	}
	//EOF My Functions
};
//#pragma pop(pack)
#endif