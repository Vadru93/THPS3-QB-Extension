// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "Extension.h"
#include "unzip.h"
#include "windows.h"
#include <conio.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
//#include "detours.h"
#include "Psapi.h"
#include <string.h>
#include <iostream>
#include <WinSock2.h>
#include "Shellapi.h"
#include "unzip.h"
#include "vector"
#include <tlhelp32.h>
#include <d3dx8.h>
#include <d3dx8core.h>
#include <sstream>
#include "proxydll.h"
#include "debug.h"
#include "SuperSectors.h"

#pragma comment(lib, "d3dx8.lib")

#define PAGE_READABLE (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)




BYTE SpineButton = 7;

using namespace std;

char qbPath[256] = "";
char Level[256] = "";
char msg[256] = "";
bool downloading = false;
bool debugMode = false;
//int pSpeed;
static const char* clamp = "Clamp";
BYTE clampfunc[] = { 0x2E, 0x3E, 0x2B, 0xFF, 0xFE, 0xCE, 0xAE, 0xBE, 0x90, 0x90, 0x23 };
bool FirstTime = true;
void* __restrict mallocx(const DWORD buflen);
static const char* lastQB = NULL;
#define REV_END(x)  ((x << 24) | ((x << 8) & 0xFF0000) | (x >> 24) | ((x >> 8) & 0xFF00))
//#define CScript void
#define MAXRECV 20576
#undef SendMessage
struct CStruct;
struct CStructHeader;
struct CArray;
struct KeyState;
void SetStructValues(CStructHeader* pStruct, CStructHeader* values);
void SetArrayValues(CArray* pArray, CStructHeader* values);
#define VERSION 4.8f
static float version = VERSION;

bool InvalidReadPtr(const void* const __restrict ptr, const DWORD size);
bool InvalidReadPtr(const void* const __restrict ptr);
void TestForAcid_naked();
void Fopen_naked();
//FILE* _cdecl _fopen(const char* p, const char* b);
//void Ncomp_naked();
void Checksum_naked();
struct Matrix;

bool SetTagLimit(CStruct* pStruct, CScript* pScript);



Matrix& CreateRotateMatrix(Matrix& mat, const D3DXVECTOR3& axis, const float angle);
Matrix& CreateRotateYMatrix(Matrix& rotY, float angle);



struct CompressedNode
{
	DWORD checksum;
	CStructHeader* first;
	CStructHeader* last;
	bool added;


	CompressedNode()
	{
		checksum = 0;
		first = NULL;
		last = NULL;
		added = false;
	}
};
vector<CompressedNode> compNodes;
std::vector<MovingObject> movingObjects;



struct Script
{
	std::map<int, char*> qbTable;
	void CreateQBTable(BYTE* table)
	{
		while (*table == 0x2B)
		{
			table++;
			int key = *(int*)table;
			table += 4;
			char* name = (char*)table;
			//printf("QbNAme %s\n", name);
			DWORD len = strlen(name) + 1;
			name = new char[len];
			if (!name)
				MessageBox(0, "nooo", "", 0);
			memcpy(name, (char*)table, len);
			//printf("QbAllocated %s\n", name);
			if (qbTable.find(key) == qbTable.end())
				qbTable.insert(std::pair<int, char*>(key, name));
			//printf("inserted\n");
			table += len;
			//printf("added\n");
		}
	}


	Script()
	{
		char* qdir = (char*)0x5BBAF8;
		char dir[256] = "data\\";
		DWORD len = strlen(qdir) + 1;
		memcpy(&dir[5], qdir, len);

		FILE* f = fopen(dir, "rb+");


		fseek(f, 0, SEEK_END);
		DWORD size = ftell(f);

		fseek(f, 0, SEEK_SET);
		BYTE* pFile = new BYTE[size + 1];
		fread(pFile, size, 1, f);
		fclose(f);
		BYTE* oldData = pFile;

		//MessageBox(0, dir, "going to parse", 0);
		while (pFile < (oldData + size - 5))
		{
			unsigned int i = 13;
			while (pFile < (oldData + size) && *pFile != 0x0A && *pFile != 0x0D)
			{
				dir[i] = *pFile;
				dir[i + 1] = 0;
				i++;
				pFile++;
			}

			/*char* p = dir;
			while (*p != '.')
				p++;
			p += 2;
			*p = '\0x0';
			p++;
			*p = '\0x0';*/
			dir[i] = 0;


			OpenScript(dir);
			pFile++;
			while (pFile < (oldData + size) && (*pFile == 0x0D || *pFile == 0x0A))
				pFile++;
		}
		delete[] oldData;
	}
	char unkString[256] = "";
	char* GetQBKeyName(int checksum)
	{
		if (qbTable.size())
		{
			std::map<int, char*>::iterator it;

			it = qbTable.find(checksum);

			if (it != qbTable.end())
			{
				return it->second;
			}
		}
		return NULL;
	}

	void OpenScript(char* path)
	{
		printf("OpenScript: %s\n", path);//MessageBox(0, "OpenScript", path, 0);
		FILE* f = fopen(path, "rb+");

		fseek(f, 0, SEEK_END);
		DWORD size = ftell(f);

		fseek(f, 0, SEEK_SET);
		BYTE* pFile = new BYTE[size + 1];
		fread(pFile, size, 1, f);
		fclose(f);

		//data = pFile;
		BYTE* oldData = pFile;

		while (true)
		{
			switch (*pFile)
			{
			case 0x2:
			case 0x16:
			case 0x17:
			case 0x1A:
			case 0x2E:
				pFile += 5;
				break;

			case 0x1B:
				pFile++;
				pFile += *(int*)pFile + 4;
				break;

			case 0x1C:
				pFile++;
				pFile += *(int*)pFile + 4;
				break;

			case 0x1E:
				pFile += 13;
				break;

			case 0x1F:
				pFile += 9;
				break;

			case 0x00:
			case 0x2B:
				printf("parsing qbTable %X\n", pFile);
				goto done;

			case 0x47:
			case 0x48:
			case 0x49:
				pFile += 3;
				break;

			default:
				//printf("default: %X\n", *pFile);
				/*char def[2];
				sprintf(def, "%X", *pFile);
				MessageBox(0,"default", def, 0);*/
				pFile++;
				break;

			case 0x2F:
			case 0x37:
			case 0x40:
			case 0x41:
				pFile++;
				int count = *(int*)pFile;
				pFile += 4;
				/*if (*(int*)pFile == 65537 || (count == 1 && *(WORD*)pFile == 1))
				{
					pFile += 2 * count;
				}*/
				pFile += count * 4;
				break;
			}
		}
	done:
		CreateQBTable(pFile);
		printf("END OpenScript\n");
		delete[] oldData;

	}

	~Script()
	{
		std::map<int, char*>::iterator end = qbTable.end();
		for (std::map<int, char*>::iterator it = qbTable.begin(); it != end; it++)
		{
			delete[] it->second;
		}
		qbTable.clear();
	}
};


EXTERN char* FindChecksumName(DWORD checksum)
{
	if (qbTable)
	{
		char* name = qbTable->GetQBKeyName(checksum);
		if (name)
			return name;
		else
		{
			char unkString[128] = "";
			sprintf(unkString, "UNKNOWN(0X%X)", checksum);
			return unkString;
		}
	}
	else
		return (char*)"NOT_INIT";
}


EXTERN void CStruct::FreeHeader()
{
	CStructHeader* p = (CStructHeader*)this;

	while (p != NULL)
	{
		if (p->pData != NULL)
		{
			if (p->Type == 10 || p->Type == 11)
				((CStruct*)p->pData)->FreeHeader();
			else if (p->Type == 12)
				((CArray*)p->pData)->Free();
			free(p->pData);
		}
		p = p->NextHeader;
	}
}

EXTERN void CStruct::RemoveParam(DWORD name)
{
	CStructHeader* header = head;
	while (header != NULL)
	{
		if (header->QBkey == name)
		{
			DellocateCStruct(header);
			break;
		}
		header = header->NextHeader;
	}
}

EXTERN void CStruct::Free()
{
	if (head == NULL)
		return;

	CStructHeader* p = head;

	while (p != tail)
	{
		if (p->pData != NULL)
		{
			if (p->Type == 10 || p->Type == 11)
				((CStruct*)p->pData)->FreeHeader();
			else if (p->Type == 12)
				((CArray*)p->pData)->Free();
			free(p->pData);
		}
		p = p->NextHeader;
	}
}

EXTERN CStructHeader* CStruct::AddParam(const char* name, QBKeyHeader::QBKeyType type)
{
	if (head)
	{
		if (tail)
		{
			CStructHeader* param = AllocateCStruct();
			tail->NextHeader = param;
			tail = param;
			tail->Type = type;
			tail->SetName(name);
			tail->NextHeader = NULL;
		}
		else
		{
			CStructHeader* param = head->NextHeader;
			CStructHeader* prevparam = NULL;
			while (param)
			{
				prevparam = param;
				param = param->NextHeader;
			}
			prevparam->NextHeader = AllocateCStruct();
			param = prevparam->NextHeader;
			tail->NextHeader = param;
			tail = param;
			tail->Type = type;
			tail->SetName(name);
			tail->NextHeader = NULL;
		}
	}
	else
	{
		head = AllocateCStruct();
		tail = head;
		tail->Type = type;
		tail->SetName(name);
		tail->NextHeader = NULL;
	}
	return tail;
}

EXTERN CStructHeader* CArray::GetCStruct(int Name, int Item, int* outItem)
{
	if (Type == 0x0A || Type == 0x0B)
	{
		for (int i = Item; i < NumItems; i++)
		{
			int numItems = ((CStructHeader***)Items)[i][0]->GetNumItems();
			for (int j = 0; j < numItems; j++)
			{
				if (((CStructHeader***)Items)[i][j]->QBkey == Name)
				{
					*outItem = i;
					return ((CStructHeader***)Items)[i][j];
				}
			}
		}
		return 0;
	}

	return 0;
}

EXTERN void CArray::SetValues(CStructHeader* values)
{
	for (DWORD i = 0; i < NumItems; i++)
	{
		CStructHeader* value = ((CStructHeader***)Items)[i][0];
		/*CStructHeader*** StructHeader = (CStructHeader***)Items;
		DWORD numItems = StructHeader[i][0]->GetNumItems();
		for(DWORD j=0; j<numItems; j++)
		{
		CStructHeader* value = ((CStructHeader***)Items)[i][j];*/
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
					case QBKeyHeader::LOCAL_STRING:
					case QBKeyHeader::STRING:
						strcpy(value->pStr, newValue->pStr);
					case QBKeyHeader::VECTOR:
						memcpy(value->pData, newValue->pData, 12);
					}
					goto next;
				}
				newValue = newValue->NextHeader;
			}
			switch (value->Type)
			{
			case QBKeyHeader::QBKeyType::LOCAL_STRUCT:
			case QBKeyHeader::QBKeyType::STRUCT:
				value->pStruct[0]->SetValues(values);
			case QBKeyHeader::QBKeyType::ARRAY:
				SetArrayValues(value->pArray, values);//value->pArray->SetValues(values);
			}
		next:
			value = value->NextHeader;
		}
		//}
	}
}


EXTERN void CArray::Free()
{
	if (Type == 10 || Type == 11)
	{
		for (int i = 0; i < NumItems; ++i)
		{
			int numItems = ((CStructHeader***)Items)[i][0]->GetNumItems();
			for (int j = 0; j < numItems; ++j)
			{
				if (((CStructHeader***)Items)[i][j]->Type == 3 || ((CStructHeader***)Items)[i][j]->Type == 4 || ((CStructHeader***)Items)[i][j]->Type == 5 || ((CStructHeader***)Items)[i][j]->Type == 6 || ((CStructHeader***)Items)[i][j]->Type == 10 || ((CStructHeader***)Items)[i][j]->Type == 11 || ((CStructHeader***)Items)[i][j]->Type == 12)
				{
					free(((CStructHeader***)Items)[i][j]->pData);
				}
				((CStructHeader***)Items)[i][j]->pData = NULL;
			}
			free(((CStructHeader***)Items)[i]);
			((CStructHeader***)Items)[i] = NULL;
		}
		free(Items);
		Items = NULL;
	}
	else if (Type == 6 || Type == 3 || Type == 5)
	{
		for (int i = 0; i < NumItems; ++i)
		{
			free((void*)Items[i]);
			Items[i] = NULL;
		}
		free(Items);
		Items = NULL;
	}
	else
	{
		free(Items);
		Items = NULL;
	}
}

EXTERN bool CArray::AddCStruct(CStruct* Struct)
{
	if (Type == 10 || Type == 11)
	{
		NumItems++;
		Items = (CStructHeader**)realloc(Items, (sizeof(void**) * NumItems));
		if (Items == NULL)
			return false;
		Items[NumItems] = (CStructHeader*)Struct;
		return true;
	}
	return false;
}

EXTERN CStructHeader* CArray::GetCStruct(int Name, int Item, int Struct)
{
	if (Type == 0x0A || Type == 0x0B)
	{
		CStructHeader*** StructHeader = (CStructHeader***)Items;
		int numItems = StructHeader[Item][Struct]->GetNumItems();
		for (int i = Struct; i < numItems; i++)
		{
			if (Name == StructHeader[Item][i]->QBkey)
			{
				return StructHeader[Item][i];
			}
		}

		return 0;

	}
	else return 0;
}

EXTERN int CArray::GetNumItems()
{
	int numItems = 0;
	if (Type == 0x0A || Type == 0x0B)
	{
		for (int i = 0; i < NumItems; ++i)
		{
			numItems += ((CStructHeader***)Items)[i][0]->GetNumItems();
		}
		return numItems;
	}
	return NumItems;
}
EXTERN bool CArray::AddString(char* String)
{
	if (Type == 3 || Type == 4)
	{
		NumItems++;
		Items = (CStructHeader**)realloc(Items, (sizeof(void**) * NumItems));
		if (Items == NULL)
			return false;
		Items[NumItems] = (CStructHeader*)String;
		return true;
	}
	return false;
}


inline	float	Determinant3(float a1, float a2, float a3, float b1, float b2, float b3, float c1, float c2, float c3)
{
	return 	a1 * (b2 * c3 - b3 * c2) -
		b1 * (a2 * c3 - a3 * c2) +
		c1 * (a2 * b3 - a3 * b2);
}
struct Quat
{
	D3DXVECTOR4 quat;


	inline void		SetVector(const float x, const float y, const float z)
	{


		quat.x = x;
		quat.y = y;
		quat.z = z;
	}
	Quat()
	{

	}
	inline Quat(const D3DXVECTOR3& axis, float angle)
	{


		float	mod = D3DXVec3Length(&axis);
		float	ang = angle / 2.0f;

		mod = (mod > 0.0f) ? (1.0f / mod) : 0.0f;
		mod *= sinf(ang);

		SetVector(mod * axis[X], mod * axis[Y], mod * axis[Z]);
		quat.w = cosf(ang);
	}

	Quat(float x, float y, float z, float w)
	{
		quat.x = x;
		quat.y = y;
		quat.z = z;
		quat.w = w;
	}

	Quat& operator *=(Quat& other)
	{
		quat.x *= other.quat.x;
		quat.y *= other.quat.y;
		quat.z *= other.quat.z;
		quat.w *= other.quat.w;
		return *this;
	}

	inline Quat& Invert(void) 							// this = Invert ( this )
	{


		quat.x = -quat.x;
		quat.y = -quat.y;
		quat.z = -quat.z;


		return	*this;
	}

	inline Quat& Invert(const Quat& src) 			// this = Invert ( src )
	{


		quat.x = -src.quat.x;
		quat.y = -src.quat.y;
		quat.z = -src.quat.z;
		quat.w = src.quat.w;

		return	*this;
	}



	inline	D3DXVECTOR3	Rotate(const D3DXVECTOR3& vec) const
	{


		Quat	inv;
		//	Quat 	pt( vec[X], vec[Y], vec[Z], vec[W] );
		Quat 	pt(vec.x, vec.y, vec.z, 1.0f);		// Mick: Setting W to sensible value, otherwise can cause overflow
		Quat 	res = *this;

		inv.Invert(*this);
		res *= pt;
		res *= inv;

		return D3DXVECTOR3(res.quat.x, res.quat.y, res.quat.z);
	}
};

struct Matrixx : D3DXMATRIX
{



	Matrixx& CreateRotateMatrix(Matrixx& mat, D3DXVECTOR3& axis, const float angle)
	{

		D3DXVECTOR3	unitAxis = axis;
		D3DXVec3Normalize(&unitAxis, &unitAxis);

		float oneMinusCosine = 1.0f - cosf(angle);

		D3DXVECTOR3	leading;

		leading.x = 1.0f - (unitAxis.x * unitAxis.x);
		leading.y = 1.0f - (unitAxis.y * unitAxis.y);
		leading.z = 1.0f - (unitAxis.z * unitAxis.z);
		leading *= oneMinusCosine;

		D3DXVECTOR3	crossed;

		crossed.x = (unitAxis.y * unitAxis.z);
		crossed.y = (unitAxis.z * unitAxis.x);
		crossed.z = (unitAxis.x * unitAxis.y);
		crossed *= oneMinusCosine;

		unitAxis *= sinf(angle);

		mat.m[RIGHT][X] = 1.0f - leading[X];
		mat.m[RIGHT][Y] = crossed[Z] + unitAxis[Z];
		mat.m[RIGHT][Z] = crossed[Y] - unitAxis[Y];
		mat.m[RIGHT][W] = 0.0f;

		mat.m[UP][X] = crossed[Z] - unitAxis[Z];
		mat.m[UP][Y] = 1.0f - leading[Y];
		mat.m[UP][Z] = crossed[X] + unitAxis[X];
		mat.m[UP][W] = 0.0f;

		mat.m[AT][X] = crossed[Y] + unitAxis[Y];
		mat.m[AT][Y] = crossed[X] - unitAxis[X];
		mat.m[AT][Z] = 1.0f - leading[Z];
		mat.m[AT][W] = 0.0f;

		mat.m[POS][X] = 0.0f;
		mat.m[POS][Y] = 0.0f;
		mat.m[POS][Z] = 0.0f;
		mat.m[POS][W] = 1.0f;

		return mat;
	}

	Matrixx(D3DXVECTOR3& axis, const float angle)
	{
		CreateRotateMatrix(*this, axis, angle);
	}

	inline D3DXVECTOR3			Rotate(const D3DXVECTOR3& src) const
	{


		D3DXVECTOR3	v;

		v[X] = src[X] * m[RIGHT][X] + src[Y] * m[UP][X] + src[Z] * m[AT][X];
		v[Y] = src[X] * m[RIGHT][Y] + src[Y] * m[UP][Y] + src[Z] * m[AT][Y];
		v[Z] = src[X] * m[RIGHT][Z] + src[Y] * m[UP][Z] + src[Z] * m[AT][Z];

		return v;
	}
};


struct Vertex
{
	float X;
	float Y;
	float Z;

	Vertex()
	{
		X = 0.0f;
		Y = 0.0f;
		Z = 0.0f;
	}

	Vertex operator =(D3DXVECTOR3 v)
	{
		return *(Vertex*)&v;
	}

	Vertex(float x, float y, float z)
	{
		X = x;
		y = Y;
		Z = z;
	}

	inline Vertex& Rotate(const Vertex& axis, const float angle)
	{

#if 1
		Matrixx	mat(*(D3DXVECTOR3*)&axis, angle);
		*this = *(Vertex*)&mat.Rotate(*(D3DXVECTOR3*)&*this);
#else
		Quat	quat(*(D3DXVECTOR3*)&axis, angle);
		*this = *(Vertex*)&quat.Rotate(*(D3DXVECTOR3*)&*this);
#endif

		return *this;
	}

	void Scale(float scale)
	{
		this->X *= scale;
		this->Y *= scale;
		this->Z *= scale;
	}

	Vertex& ProjectToPlane(const Vertex& normal)
	{


		float perp_component = D3DXVec3Dot((D3DXVECTOR3*)this, (D3DXVECTOR3*)&normal);

		*this -= normal * perp_component;

		return *this;

	}

	Vertex& RotateToPlane(const Vertex& normal)
	{
		// get the length of the vector
		float length = Length();

		// Project the vector onto the plane
		ProjectToPlane(normal);

		// now we've projected it ontot he plane
		// we need to handle the case where it dissapears into a point
		// which would indicate that we were perpendicular to the plane
		// so need to get an arbitary vector in the plane	
		float  projected_length = Length();
		if (projected_length == 0.0f) 		// is this a valid comparision?
		{
			// Rotate vector through -90 degrees about Y then +90 about X
			this->X = -normal.Z;
			this->Y = normal.X;
			this->Z = -normal.Y;
		}

		// get unit vector in this direction
		Normalize();

		// multiply by original speed to rotate velocity onto plane
		*this *= length;
		return *this;
	}

	Vertex& RotateToNormal(const Vertex& normal)
	{

		*this = normal * Length();

		return *this;
	}

	Vertex& ProjectToNormal(const Vertex& normal)
	{

		float dot = D3DXVec3Dot((D3DXVECTOR3*)this, (D3DXVECTOR3*)&normal);
		*this = normal * dot;

		return *this;
	}

	inline Vertex& Normalize(float len)
	{


		float l = Length();
		if (l > 0.0f)
		{
			l = len / l;

			this->X *= l;
			this->Y *= l;
			this->Z *= l;
		}
		else
		{
			//		Dbg_MsgAssert(0,("Normalizing vector of zero length"));
		}
		return *this;
	}

	void Normalize()
	{
		float l = Length();

		if (l > 0.0f)
		{
			l = 1.0f / l;

			this->X *= l;
			this->Y *= l;
			this->Z *= l;
		}
		else
		{

			this->X *= 0.5f;
			this->Y *= 0.5f;
			this->Z *= 0.5f;
			//MessageBox(0,("Normalizing vector of zero length"), "", 0);
		}
	}

	float LengthSqr()
	{
		return this->X * this->X + this->Y * this->Y + this->Z * this->Z;
	}

	float Length()
	{
		return sqrtf(LengthSqr());
	}
	Vertex& operator -= (const Vertex& v)
	{
		X -= v.X;
		Y -= v.Y;
		Z -= v.Z;
		return *this;
	}

	Vertex& operator += (const Vertex& v)
	{
		X += v.X;
		Y += v.Y;
		Z += v.Z;
		return *this;
	}

	Vertex& operator *= (float f)
	{
		X *= f;
		Y *= f;
		Z *= f;
		return *this;
	}

	Vertex&
		operator /= (float f)
	{
		float fInv = 1.0f / f;
		X *= fInv;
		Y *= fInv;
		Z *= fInv;
		return *this;
	}


	// unary operators
	Vertex operator + () const
	{
		return *this;
	}

	Vertex operator - () const
	{
		return Vertex(-X, -Y, -Z);
	}


	// binary operators
	Vertex operator + (const Vertex& v) const
	{
		return Vertex(X + v.X, Y + v.Y, Z + v.Z);
	}

	Vertex operator - (const Vertex& v) const
	{
		return Vertex(X - v.X, Y - v.Y, Z - v.Z);
	}

	Vertex operator * (float f) const
	{
		return Vertex(X * f, Y * f, Z * f);
	}

	Vertex operator / (float f) const
	{
		float fInv = 1.0f / f;
		return Vertex(X * fInv, Y * fInv, Z * fInv);
	}

	bool
		operator == (const Vertex& v) const
	{
		return X == v.X && Y == v.Y && Z == v.Z;
	}

	bool operator != (const Vertex& v) const
	{
		return X != v.X || Y != v.Y || Z != v.Z;
	}

};

inline const float GetSpeed(const Vertex* const __restrict vertex)
{
	const float x = vertex->X;
	const float y = vertex->Y;
	const float z = vertex->Z;

	return sqrtf(x * x + y * y + z * z);
}


Vertex GetNormal(Vertex* v)
{
	Vertex b;
	float speed = GetSpeed(v);
	if (speed < 0.0001f)
	{
		speed = 1.0f;
	}
	b.X = v->X / speed;
	b.Y = v->Y / speed;
	b.Z = v->Z / speed;

	return b;
}


struct Matrix : D3DXMATRIX
{

	typedef void(__thiscall* const pOrthoNormalizeAbout)(Matrix* pThis, DWORD axis);
	void OrthoNormalizeAbout(DWORD axis)
	{
		pOrthoNormalizeAbout(0x0040F0E0)(this, axis);
	}

	typedef void(__thiscall* const pRotate)(Matrix* pThis, const Matrix& rot);
	void Rotate(const Matrix& rot)
	{
		pRotate(0x0040F7A0)(this, rot);
	}

	inline Matrix& InvertUniform()
	{


		// Only works for orthonormal Matrix

#if 0	// (Mike) this version is buggy!

		Transpose(src);

		row[POS][X] = -DotProduct(src[POS], src[RIGHT]);
		row[POS][Y] = -DotProduct(src[POS], src[UP]);
		row[POS][Z] = -DotProduct(src[POS], src[AT]);

		return *this;
#else	// (Mike) try this one instead...

	// (Dan) actually, don't use the vu0; it returns before the correct values are safely stored in memory; if you access the results too
	// quickly, you get the wrong answers; this is because the compiler may reorder instructions
#if 0
	// (Ken) or try this one, uses vu0 
		Vu0Invert((sceVu0FMATRIX)this);
		return *this;
#else

	// need to copy the old row,
	// or else it will change on us as we're doing
	// our dot products...

		Vertex oldPos = *(Vertex*)m[POS];

		m[POS][X] = -D3DXVec3Dot((D3DXVECTOR3*)&oldPos, (D3DXVECTOR3*)m[RIGHT]);
		m[POS][Y] = -D3DXVec3Dot((D3DXVECTOR3*)&oldPos, (D3DXVECTOR3*)m[UP]);
		m[POS][Z] = -D3DXVec3Dot((D3DXVECTOR3*)&oldPos, (D3DXVECTOR3*)m[AT]);

		std::swap(m[X][Y], m[Y][X]);
		std::swap(m[X][Z], m[Z][X]);
		std::swap(m[Y][Z], m[Z][Y]);

		return *this;

#endif

#endif
	}


	void Rotate(D3DXVECTOR3 axis, float angle)
	{

		Matrix rot_mat;
		D3DXMatrixIdentity(&rot_mat);
		*this *= CreateRotateMatrix(rot_mat, axis, angle);
	}

	inline float			Determinant(void) const
	{


		return m[X][X] * Determinant3(m[Y][Y], m[Z][Y], m[W][Y], m[Y][Z], m[Z][Z], m[W][Z], m[Y][W], m[Z][W], m[W][W])
			- m[X][Y] * Determinant3(m[Y][X], m[Z][X], m[W][X], m[Y][Z], m[Z][Z], m[W][Z], m[Y][W], m[Z][W], m[W][W])
			+ m[X][Z] * Determinant3(m[Y][X], m[Z][X], m[W][X], m[Y][Y], m[Z][Y], m[W][Y], m[Y][W], m[Z][W], m[W][W])
			- m[X][W] * Determinant3(m[Y][X], m[Z][X], m[W][X], m[Y][Y], m[Z][Y], m[W][Y], m[Y][Z], m[Z][Z], m[W][Z]);
	}


	inline Matrix& Adjoint(const Matrix& src)
	{


		m[X][X] = Determinant3(src.m[Y][Y], src.m[Z][Y], src.m[W][Y], src.m[Y][Z], src.m[Z][Z], src.m[W][Z], src.m[Y][W], src.m[Z][W], src.m[W][W]);
		m[Y][X] = -Determinant3(src.m[Y][X], src.m[Z][X], src.m[W][X], src.m[Y][Z], src.m[Z][Z], src.m[W][Z], src.m[Y][W], src.m[Z][W], src.m[W][W]);
		m[Z][X] = Determinant3(src.m[Y][X], src.m[Z][X], src.m[W][X], src.m[Y][Y], src.m[Z][Y], src.m[W][Y], src.m[Y][W], src.m[Z][W], src.m[W][W]);
		m[W][X] = -Determinant3(src.m[Y][X], src.m[Z][X], src.m[W][X], src.m[Y][Y], src.m[Z][Y], src.m[W][Y], src.m[Y][Z], src.m[Z][Z], src.m[W][Z]);

		m[X][Y] = -Determinant3(src.m[X][Y], src.m[Z][Y], src.m[W][Y], src.m[X][Z], src.m[Z][Z], src.m[W][Z], src.m[X][W], src.m[Z][W], src.m[W][W]);
		m[Y][Y] = Determinant3(src.m[X][X], src.m[Z][X], src.m[W][X], src.m[X][Z], src.m[Z][Z], src.m[W][Z], src.m[X][W], src.m[Z][W], src.m[W][W]);
		m[Z][Y] = -Determinant3(src.m[X][X], src.m[Z][X], src.m[W][X], src.m[X][Y], src.m[Z][Y], src.m[W][Y], src.m[X][W], src.m[Z][W], src.m[W][W]);
		m[W][Y] = Determinant3(src.m[X][X], src.m[Z][X], src.m[W][X], src.m[X][Y], src.m[Z][Y], src.m[W][Y], src.m[X][Z], src.m[Z][Z], src.m[W][Z]);

		m[X][Z] = Determinant3(src.m[X][Y], src.m[Y][Y], src.m[W][Y], src.m[X][Z], src.m[Y][Z], src.m[W][Z], src.m[X][W], src.m[Y][W], src.m[W][W]);
		m[Y][Z] = -Determinant3(src.m[X][X], src.m[Y][X], src.m[W][X], src.m[X][Z], src.m[Y][Z], src.m[W][Z], src.m[X][W], src.m[Y][W], src.m[W][W]);
		m[Z][Z] = Determinant3(src.m[X][X], src.m[Y][X], src.m[W][X], src.m[X][Y], src.m[Y][Y], src.m[W][Y], src.m[X][W], src.m[Y][W], src.m[W][W]);
		m[W][Z] = -Determinant3(src.m[X][X], src.m[Y][X], src.m[W][X], src.m[X][Y], src.m[Y][Y], src.m[W][Y], src.m[X][Z], src.m[Y][Z], src.m[W][Z]);

		m[X][W] = -Determinant3(src.m[X][Y], src.m[Y][Y], src.m[Z][Y], src.m[X][Z], src.m[Y][Z], src.m[Z][Z], src.m[X][W], src.m[Y][W], src.m[Z][W]);
		m[Y][W] = Determinant3(src.m[X][X], src.m[Y][X], src.m[Z][X], src.m[X][Z], src.m[Y][Z], src.m[Z][Z], src.m[X][W], src.m[Y][W], src.m[Z][W]);
		m[Z][W] = -Determinant3(src.m[X][X], src.m[Y][X], src.m[Z][X], src.m[X][Y], src.m[Y][Y], src.m[Z][Y], src.m[X][W], src.m[Y][W], src.m[Z][W]);
		m[W][W] = Determinant3(src.m[X][X], src.m[Y][X], src.m[Z][X], src.m[X][Y], src.m[Y][Y], src.m[Z][Y], src.m[X][Z], src.m[Y][Z], src.m[Z][Z]);

		return *this;

	}


	inline Matrix& Invert(const Matrix& src)
	{


		Adjoint(src);

		float det = src.Determinant();

		if (det != 1.0f)
		{
			*this *= (1.0f / det);
		}

		return *this;
	}

	inline Matrix& Invert(void)
	{


		Matrix	d;

		d.Invert(*this);

		*this = d;

		return *this;
	}

	void GetRotationAxisAndAngle(Vertex* pAxis, float* pRadians)
	{
		float		nTwoSinTheta, nTwoCosTheta;
		Vertex	vTwoSinThetaAxis;

		nTwoCosTheta = m[RIGHT][X] + m[UP][Y] + m[AT][Z] - 1.0f;

		vTwoSinThetaAxis.X = m[UP][Z] - m[AT][Y];
		vTwoSinThetaAxis.Y = m[AT][X] - m[RIGHT][Z];
		vTwoSinThetaAxis.Z = m[RIGHT][Y] - m[UP][X];

		// Gary:  There used to be a magic patch added by Dave
		// (basically negating the axis) which made it work with
		// the RW-based Slerp object.  This doesn't seem to be
		// necessary any more, but I'm going to leave it
		// in the code just in case our Slerp stops working...
		/*vTwoSinThetaAxis.X = m[AT][Y] - m[UP][Z];
		vTwoSinThetaAxis.Y = m[RIGHT][Z] - m[AT][X];
		vTwoSinThetaAxis.Y = 0.0f;// m[UP][X] - m[RIGHT][Y];
		vTwoSinThetaAxis.Z = 0.0f;*/
		//vTwoSinThetaAxis.W = 1.0f;

		nTwoSinTheta = vTwoSinThetaAxis.Length();

		if (nTwoSinTheta > 0.0f)
		{
			float  recipLength = (1.0f / (nTwoSinTheta));

			*pAxis = vTwoSinThetaAxis;
			pAxis->Scale(recipLength);
		}
		else
		{
			*pAxis = Vertex(0.0f, 0.0f, 0.0f);
		}

		(*pRadians) = (float)atan2(nTwoSinTheta, nTwoCosTheta);
		if ((nTwoSinTheta <= 0.01f) && (nTwoCosTheta <= 0.0f))
		{
			/*
			* sin theta is 0; cos theta is -1; theta is 180 degrees
			* vTwoSinThetaAxis was degenerate
			* axis will have to be found another way.
			*/

			//Vector	vTwoSinThetaAxis;

			/*
			* Matrix is:
			* [ [ 2 a_x^2 - 1,  2 a_x a_y,   2 a_x a_z,  0 ]
			*   [  2 a_x a_y,  2 a_y^2 - 1,  2 a_y a_z,  0 ]
			*   [  2 a_x a_z,   2 a_y a_z,  2 a_z^2 - 1, 0 ]
			*   [      0,           0,           0,      1 ] ]
			* Build axis scaled by 4 * component of maximum absolute value
			*/

			if (m[RIGHT][X] > m[UP][Y])
			{
				if (m[RIGHT][X] > m[AT][Z])
				{
					vTwoSinThetaAxis.X = 1.0f + m[RIGHT][X];
					vTwoSinThetaAxis.X = vTwoSinThetaAxis.X + vTwoSinThetaAxis.X;
					vTwoSinThetaAxis.Y = m[RIGHT][Y] + m[UP][X];
					vTwoSinThetaAxis.Z = m[RIGHT][Z] + m[AT][X];
				}
				else
				{
					vTwoSinThetaAxis.Z = 1.0f + m[AT][Z];
					vTwoSinThetaAxis.Z = vTwoSinThetaAxis.Z + vTwoSinThetaAxis.Z;
					vTwoSinThetaAxis.X = m[AT][X] + m[RIGHT][Z];
					vTwoSinThetaAxis.Y = m[AT][Y] + m[UP][Z];
				}
			}
			else
			{
				if (m[UP][Y] > m[AT][Z])
				{
					vTwoSinThetaAxis.Y = 1.0f + m[UP][Y];
					vTwoSinThetaAxis.Y = vTwoSinThetaAxis.Y + vTwoSinThetaAxis.Y;
					vTwoSinThetaAxis.Z = m[UP][Z] + m[AT][Y];
					vTwoSinThetaAxis.X = m[UP][X] + m[RIGHT][Y];
				}
				else
				{
					vTwoSinThetaAxis.Z = 1.0f + m[AT][Z];
					vTwoSinThetaAxis.Z = vTwoSinThetaAxis.Z + vTwoSinThetaAxis.Z;
					vTwoSinThetaAxis.X = m[AT][X] + m[RIGHT][Z];
					vTwoSinThetaAxis.Y = m[AT][Y] + m[UP][Z];
				}
			}

			/*
			* and normalize the axis
			*/

			*pAxis = vTwoSinThetaAxis;
			pAxis->Normalize();
		}
	}

	inline Matrix& RotateYLocal(const float angle)
	{



		Matrix rotY;
		(D3DXMATRIX)*this = CreateRotateYMatrix(rotY, angle) * *this;

		return *this;
	}
};



Vertex skpos;
Vertex skopos;
Vertex skvel;
Vertex scol;
Vertex ecol;



struct Vector : D3DXVECTOR4
{

	void Scale(float scale)
	{
		this->x *= scale;
		this->y *= scale;
		this->z *= scale;
		this->w *= scale;
	}
};

Matrix& CreateRotateYMatrix(Matrix& rotY, float angle)
{


	float	s = sinf(angle);
	float	c = cosf(angle);

	rotY.m[RIGHT][X] = c;
	rotY.m[RIGHT][Y] = 0.0f;
	rotY.m[RIGHT][Z] = -s;
	rotY.m[RIGHT][W] = 0.0f;

	rotY.m[UP][X] = 0.0f;
	rotY.m[UP][Y] = 1.0f;
	rotY.m[UP][Z] = 0.0f;
	rotY.m[UP][W] = 0.0f;

	rotY.m[AT][X] = s;
	rotY.m[AT][Y] = 0.0f;
	rotY.m[AT][Z] = c;
	rotY.m[AT][W] = 0.0f;

	rotY.m[POS][X] = 0.0f;
	rotY.m[POS][Y] = 0.0f;
	rotY.m[POS][Z] = 0.0f;
	rotY.m[POS][W] = 1.0f;

	return rotY;
}

bool GetZAngle(CStruct* pParams, CScript* pScript)
{
	static const DWORD ptr = 0x008E1E90;
	VALIDATE_PTR((void*)ptr);
	DWORD pAngle = *(DWORD*)ptr + 0xC0;
	VALIDATE_PTR((void*)pAngle);
	pAngle = *(DWORD*)pAngle + 0x5C4;
	VALIDATE_PTR((void*)pAngle);
	pAngle = *(DWORD*)pAngle + 0x0C;
	VALIDATE_PTR((void*)pAngle);
	pAngle = *(DWORD*)pAngle + 0x10;
	VALIDATE_PTR((void*)pAngle);
	pAngle = *(DWORD*)pAngle + 0x40;
	VALIDATE_DATA((float*)pAngle, sizeof(float));
	CStructHeader* header = pScript->params->head;
	while (header)
	{
		if (header->QBkey == 0xFF7EBAF6)//angle
		{
			header->value.f = *(float*)pAngle * (180.0f / 3.14159f);
			break;
		}
		header = header->NextHeader;
	}
	return true;
}

bool GetSkaterLook(CStruct* pParams, CScript* pScript)
{
	static const DWORD ptr = 0x008E2498;
	VALIDATE_PTR((void*)ptr);
	DWORD pPos = *(DWORD*)ptr + 0x13C;
	VALIDATE_PTR((void*)pPos);
	pPos = *(DWORD*)pPos + 0x14;
	VALIDATE_PTR((void*)pPos);
	const DWORD old = pPos;
	pPos = *(DWORD*)pPos + 0x4BC;
	if (InvalidReadPtr((void*)pPos))
	{
		pPos = old;
		pPos = *(DWORD*)pPos + 0x3B4;
		VALIDATE_PTR((void*)pPos);
	}
	pPos = *(DWORD*)pPos + 0x18;
	VALIDATE_DATA((Vertex*)pPos, sizeof(Vertex));
	Vertex look = *(Vertex*)pPos;
	static const DWORD ptr2 = 0x005D06C0;
	VALIDATE_PTR((void*)ptr2);
	DWORD pSpeed = *(DWORD*)ptr2 + 0x580;
	VALIDATE_PTR((void*)pSpeed);
	pSpeed = *(DWORD*)pSpeed + 0x4;
	VALIDATE_PTR((void*)pSpeed);
	pSpeed = *(DWORD*)pSpeed + 0x2C;
	VALIDATE_PTR((void*)pSpeed);
	pSpeed = *(DWORD*)pSpeed + 0x48;
	VALIDATE_PTR((void*)pSpeed);
	pSpeed = *(DWORD*)pSpeed + 0x334;
	VALIDATE_DATA((Vertex*)pSpeed, sizeof(Vertex));
	CStructHeader* header = pScript->params->head;
	look.X += ((Vertex*)pSpeed)->X;
	look.Y += ((Vertex*)pSpeed)->Y + 100.0f;
	look.Z += ((Vertex*)pSpeed)->Z;

	while (header)
	{
		if (header->QBkey == 0x7F261953)
		{
			*header->pVec = *(D3DXVECTOR3*)&look;
			break;
		}
		header = header->NextHeader;
	}
	return true;
}

Vertex vertex;


bool observing = false;
struct ObserveMode
{
	//PlayerInfo: 0x008E2498; + 0x13C;
	float cameraAngle;
	Vertex* pos;
	Vertex* camera;
	Vertex* velocity;
	void* current;
	void* first;
	void* last;
	char name[64];
	ObserveMode()//get data
	{
		UpdateInfo(true);
	}

	void Enter()
	{
		observing = true;
		cameraAngle = atan2f((pos->X - camera->X), (pos->Z - camera->Z));
	}

	void Leave()
	{
		observing = false;
	}

	static Vertex* GetCamera()
	{
		static const DWORD ptr = 0x008E1E78;
		if (InvalidReadPtr(((void*)ptr)))
			return NULL;
		DWORD pCamera = *(DWORD*)ptr + 0x84;
		if (InvalidReadPtr(((void*)pCamera)))
			return NULL;
		pCamera = *(DWORD*)pCamera + 0xB0;
		if (InvalidReadPtr(((void*)pCamera)))
			return NULL;
		pCamera = *(DWORD*)pCamera + 0x320;
		if (InvalidReadPtr(((void*)pCamera)))
			return NULL;
		pCamera = *(DWORD*)pCamera + 0x20C;
		if (InvalidReadPtr(((void*)pCamera)))
			return NULL;
		return (Vertex*)(*(DWORD*)pCamera + 0x40);
	}

	bool PlayerExist()//check if player exist and pointers work
	{
		if (current && !InvalidReadPtr(current) && *(DWORD*)current == 0x0058E504 && *(DWORD*)((DWORD)current + 4) != 0xFFFFFFFF)
			return true;
		printf("wtf no exist\n");
		first = (void*)*(DWORD*)(*(DWORD*)0x008E2498 + 0x13C);
		last = (void*)*(DWORD*)(*(DWORD*)0x008E2498 + 0x140);
		current = first;
		while (current != last && current && !InvalidReadPtr(current) && *(DWORD*)current == 0x0058E504 && *(DWORD*)((DWORD)current + 4) != 0xFFFFFFFF)
		{
			if (!strcmp(name, (char*)((DWORD)current + 32)))
				return true;
			current = *(void**)((DWORD)current + 12);
		}
		UpdateInfo();
		return false;
	}

	void UpdateInfo(bool firstTime = false)//update all pointers and information
	{
		first = (void*)*(DWORD*)(*(DWORD*)0x008E2498 + 0x13C);
		last = (void*)*(DWORD*)(*(DWORD*)0x008E2498 + 0x140);
		if (firstTime)
			current = first;
		camera = GetCamera();
		if (current && !InvalidReadPtr(current) && *(DWORD*)current == 0x0058E504 && *(DWORD*)((DWORD)current + 4) != 0xFFFFFFFF)
		{
			pos = (Vertex*)(*(DWORD*)((DWORD)current + 20) + 0x18);
			velocity = (Vertex*)(*(DWORD*)((DWORD)current + 20) + 0x334);
			char* c = (char*)((DWORD)current + 32);
			DWORD i = 0;
			while (*c != 0)
			{
				name[i] = *c;
				c++;
				i++;
			}
			name[i] = 0;
		}
	}

	//curr +12
	void Next()//follow next player
	{
		static const DWORD ptr = 0x008E1E90;
		if (InvalidReadPtr((void*)ptr))
			return;
		DWORD camMode = *(DWORD*)ptr + 0xC0;
		camMode = *(DWORD*)camMode + 0xC;
		if (InvalidReadPtr((void*)camMode))
			return;
		camMode = *(DWORD*)camMode + 0x10;
		if (InvalidReadPtr((void*)camMode))
			return;
		camMode = *(DWORD*)camMode + 0x580;
		if (InvalidReadPtr((void*)camMode))
			return;
		*(DWORD*)camMode = 0;
		first = (void*)*(DWORD*)(*(DWORD*)0x008E2498 + 0x13C);
		last = (void*)*(DWORD*)(*(DWORD*)0x008E2498 + 0x140);
		if (PlayerExist())
		{
			if (current == last)
			{
				current = first;
				UpdateInfo();
			}

			else if (*(DWORD*)*(DWORD*)((DWORD)current + 12) == 0x0058E504 && *(DWORD*)(*(DWORD*)((DWORD)current + 12) + 4) != 0xFFFFFFFF)
			{
				current = *(void**)((DWORD)current + 12);
				UpdateInfo();
			}
		}
		printf("NewObserver: %s\n", name);
	}

	Vertex oldCamera;

	void InterpolateCamera()
	{
		static float didit = false;
		float* matrix = (float*)((DWORD)camera - 0x30);



		/*float oldAngle = asin(*(float*)((DWORD)matrix + 8));

		Vertex direction = *(Vertex*)((DWORD)matrix + 32);

		Vertex target = *pos;
		direction.X += target.X;
		direction.Y += target.Y;
		direction.Z += target.Z;
		float normalizer = sqrtf(direction.X*direction.X + direction.Y*direction.Y + direction.Z*direction.Z);
		direction.X /= normalizer;
		direction.Y /= normalizer;
		direction.Z /= normalizer;
		Vertex diff;
		diff.X = target.X - camera->X;
		diff.Y = target.Y - camera->Y;
		diff.Z = target.Z - camera->Z;
		normalizer = sqrtf(diff.X*diff.X + diff.Y*diff.Y + diff.Z*diff.Z);
		diff.X /= normalizer;
		diff.Y /= normalizer;
		diff.Z /= normalizer;
		Vertex reverseAngle;
		reverseAngle = dot(diff, direction);
		float angle = acos(reverseAngle);*/

		/*Vertex normalizedCam = oldCamera;
		float normalizer = sqrtf(normalizedCam.X*normalizedCam.X + normalizedCam.Y*normalizedCam.Y + normalizedCam.Z*normalizedCam.Z);
		normalizedCam.X /= normalizer;
		normalizedCam.Y /= normalizer;
		normalizedCam.Z /= normalizer;
		Vertex normalizedPos = *pos;
		normalizer = sqrtf(normalizedPos.X*normalizedPos.X + normalizedPos.Y*normalizedPos.Y + normalizedPos.Z*normalizedPos.Z);
		normalizedPos.X /= normalizer;
		normalizedPos.Y /= normalizer;
		normalizedPos.Z /= normalizer;

		float angle = acos(normalizedCam.X*normalizedPos.X + normalizedCam.Y*normalizedPos.Y + normalizedCam.Z*normalizedPos.Z);

		Vertex axis;
		axis.X = normalizedCam.Y*normalizedPos.Z - normalizedPos.Y*normalizedCam.Z;
		axis.Y = normalizedCam.Z*normalizedPos.X - normalizedPos.Z*normalizedCam.X;
		axis.Z = normalizedCam.X*normalizedPos.Y - normalizedPos.X*normalizedCam.Y;
		normalizer = sqrtf(axis.X*axis.X + axis.Y*axis.Y + axis.Z*axis.Z);
		axis.Y /= normalizer;*/

		/*float oldAngle = asinf(*(float*)((DWORD)matrix + 8));
		angle /= oldAngle;*/
		/*angle = sinf(angle);
		angle /= *(float*)((DWORD)matrix + 8);*/
		//skater pos - camera pos
		float x = pos->X - camera->X;
		float z = pos->Z - camera->Z;
		static bool first = false;
		static bool done = false;

		float angle = atan2f(z, x);//calculating angle between skater and camera
		float skaterAngle = angle;
		angle += 0.03f;//for rounding errors
		float oldAngle = (*(float*)((DWORD)matrix + 8) + 1.0f) * 3.14159f * 0.5f;//getting old camera angle
		if (*(float*)matrix < 0.0f)
			oldAngle = -oldAngle;
		if (oldAngle >= 3.141f)
			oldAngle = 3.141f;//oldAngle = angle - 0.025f;
		else if (oldAngle <= -3.141f)
		{
			oldAngle = -3.141f;
		}

		if ((angle <= 0.08f && angle >= -0.08f))
		{
			if (done)
			{
				first = true;
				done = false;
			}
			else
			{
				first = false;
				if (angle > 0.0f)
					angle = 0.0f;
				else
					angle = 0.0f;
			}
		}
		else if (angle >= 3.1f)
		{
			if (done)
			{
				first = true;
				done = false;
			}
			else
			{
				first = false;
				angle = 3.1f;
			}
		}
		else if (angle <= -3.1f)
		{
			if (done)
			{
				first = true;
				done = false;
			}
			else
			{
				first = false;
				angle = -3.1f;
			}
		}
		else
		{
			first = false;
			done = false;
		}

		if ((fabsf(fabsf(oldAngle) - fabsf(angle)) <= 0.08f && fabsf(oldAngle - angle) < 1.5f))
		{
			//printf("too small\n");
			return;
		}


		//printf("angle: %f old angle: %f x: %f z %f\n", angle, oldAngle, x, z);
		/*bool nulling = false;
		if (angle>=3.14f || angle<=-3.14f)
		nulling = true;
		BYTE reversing = 0;
		if (oldAngle < 0.0f && angle > 0.0f)
		reversing = 1;
		else
		if (oldAngle > 0.0f && angle < 0.0f)
		reversing = 2;*/

		angle -= oldAngle;

		printf("oldAngle(%f) deltaAngle(%f) skaterAngle(%f)\n", oldAngle, angle, angle + oldAngle);

		if (angle >= 3.1f || angle <= -3.1f)
		{
			angle += oldAngle;

			/*angle += oldAngle;
			if (angle > 0.0f && oldAngle < 0.0f)
			{

			angle = ((angle - 3.141f) + (3.141f + oldAngle));

			}
			else if (angle < 0.0f && oldAngle > 0.0f)
			{
			angle = -((angle + 3.141f) + (3.141f - oldAngle));
			}
			else
			{*/
			if (angle > 0.0f && oldAngle < 0.0f)
			{
				float absAng = fabsf(oldAngle);
				if (fabsf(angle) > absAng)
				{
					angle -= absAng;
					angle = -angle;
				}
				else
					angle -= absAng;


			}
			else if (angle < 0.0f && oldAngle > 0.0f)
			{
				float absAng = fabsf(oldAngle);
				if (fabsf(angle) > absAng)
				{
					angle += absAng;
					angle = -angle;
				}
				else
					angle += absAng;
				//printf("reversing with angle %f old(%f) angl(%f)\n", angle, oldAngle, (atan2f(z, x) + 0.03f));

			}
			else
			{
				angle -= oldAngle;
				angle = 3.141f - fabsf(angle);
				//angle = -angle;
			}


			//}
			if ((angle <= 0.04f && angle >= -0.04f))
				return;
		}

		/*if (angle >= 3.141f)
		{
		if(oldAngle <= -3.141f)
		angle = 0.0f;
		else
		angle = -angle;
		printf("reversing cause angle is 3.14\n");
		}
		else if (angle <= -3.141f)
		{
		if(oldAngle >= 3.141f)
		angle = 0.0f;
		else
		angle = -angle;
		printf("reversing cause angle is 3.14\n");
		}*/

		if (!first)
		{
			if (angle)
				angle /= 1.5f;
			if (angle >= 0.04f)
			{
				if (angle > 0.08f)
					angle = 0.04f;
			}
			else if (angle <= -0.04f)
			{
				if (angle < -0.08f)
					angle = -0.04f;
			}
			else
			{
				return;
				//printf("nulling cause smaller than 0.02\n");
			}
		}
		if (angle == 0.0f)
			return;
		/*else
		{
		if (skaterAngle>0.0f)
		angle += 0.04f;
		if (skaterAngle<0.0f)
		angle -= 0.04f;
		}*/
		/*

		if (angle >= 0.1f)
		angle = 0.08f;
		if (angle <= -0.1f)
		angle = -0.08f;

		if (reversing == 1)
		angle = 0.08f;
		else if (reversing)
		angle = -0.08f;
		if (nulling)
		angle = 0.0f;
		if (angle > 0.0f)
		{
		if (angle < 0.02f)
		angle = 0.0f;
		}
		else if (angle > -0.02f)
		angle = 0.0f;
		*/

		/*angle = atan2f(z, x);//calculating angle between skater and camera
		angle += 0.03f;//for rounding errors
		oldAngle = (*(float*)((DWORD)matrix + 8) + 1.0f)*3.14159f*0.5f;//getting old camera angle
		if ((angle-oldAngle)>=3.1f || (angle-oldAngle)<=-3.1f)
		{
		if (angle > 0.0f && oldAngle < 0.0f)
		{
		float absAng = fabsf(oldAngle);
		if (fabsf(angle)>absAng)
		{
		angle -= absAng;
		angle = -angle;
		}
		else
		angle -= absAng;


		}
		else if (angle < 0.0f && oldAngle > 0.0f)
		{
		float absAng = fabsf(oldAngle);
		if (fabsf(angle) > absAng)
		{
		angle += absAng;
		angle = -angle;
		}
		else
		angle += absAng;
		//printf("reversing with angle %f old(%f) angl(%f)\n", angle, oldAngle, (atan2f(z, x) + 0.03f));

		}
		}
		else
		angle -= oldAngle;
		if (angle>0.0f)
		{
		if (angle >= 1.6f)
		angle = 1.6f;
		else if (angle >= 1.2f)
		angle = 1.2f;
		else if (angle >= 1.0f)
		angle = 1.0f;
		else if (angle >= 0.8f)
		angle = 0.8f;
		else if (angle >= 0.6f)
		angle = 0.6f;
		else if (angle >= 0.4f)
		angle = 0.4f;
		else if (angle >= 0.2f)
		angle = 0.2f;
		else if (angle >= 0.08f)
		angle = 0.08f;
		else if (angle >= 0.06f)
		angle = 0.06f;
		else if (angle >= 0.04f)
		angle = 0.04f;
		else if (angle >= 0.02f)
		angle = 0.02f;
		else
		angle = 0.0f;
		}
		else if (angle < 0.0f)
		{
		if (angle <= -1.6f)
		angle = -1.6f;
		else if (angle <= -1.2f)
		angle = -1.2f;
		else if (angle <= -1.0f)
		angle = -1.0f;
		else if (angle <= -0.8f)
		angle = -0.8f;
		else if (angle <= -0.6f)
		angle = -0.6f;
		else if (angle <= -0.4f)
		angle = -0.4f;
		else if (angle <= -0.2f)
		angle = -0.2f;
		else if (angle <= -0.1f)
		angle = -0.1f;
		else if (angle <= -0.08f)
		angle = -0.08f;
		else if (angle <= -0.06f)
		angle = -0.06f;
		else if (angle <= -0.04f)
		angle = -0.04f;
		else if (angle <= -0.02f)
		angle = -0.02f;
		else
		angle = 0.0f;
		}
		else
		return;*/
		if (((oldAngle + angle) >= 1.54f && (oldAngle + angle) <= 1.6f) || ((oldAngle + angle) <= -1.54f && (oldAngle + angle) >= -1.6f) || ((oldAngle + angle) >= 3.11f && (oldAngle + angle) <= 3.17f) || ((oldAngle + angle) <= -3.11f && (oldAngle + angle) >= -3.17f))
			angle *= 1.5f;
		if (((oldAngle + angle) >= 1.54f && (oldAngle + angle) <= 1.6f) || ((oldAngle + angle) <= -1.54f && (oldAngle + angle) >= -1.6f) || ((oldAngle + angle) >= 3.11f && (oldAngle + angle) <= 3.17f) || ((oldAngle + angle) <= -3.11f && (oldAngle + angle) >= -3.17f))
			angle *= 0.5f;
		if (((oldAngle + angle) >= 1.54f && (oldAngle + angle) <= 1.6f) || ((oldAngle + angle) <= -1.54f && (oldAngle + angle) >= -1.6f) || ((oldAngle + angle) >= 3.11f && (oldAngle + angle) <= 3.17f) || ((oldAngle + angle) <= -3.11f && (oldAngle + angle) >= -3.17f))
			angle *= 4.0f;
		if (((oldAngle + angle) >= 1.54f && (oldAngle + angle) <= 1.6f) || ((oldAngle + angle) <= -1.54f && (oldAngle + angle) >= -1.6f) || ((oldAngle + angle) >= 3.11f && (oldAngle + angle) <= 3.17f) || ((oldAngle + angle) <= -3.11f && (oldAngle + angle) >= -3.17f))
			angle *= 0.5f;
		if (((oldAngle + angle) >= 1.54f && (oldAngle + angle) <= 1.6f) || ((oldAngle + angle) <= -1.54f && (oldAngle + angle) >= -1.6f) || ((oldAngle + angle) >= 3.11f && (oldAngle + angle) <= 3.17f) || ((oldAngle + angle) <= -3.11f && (oldAngle + angle) >= -3.17f))
			return;
		/*if (((angle + oldAngle) >= 3.13f || (angle + oldAngle) <= -3.13f))
		  angle *= 0.8f;
		  if (((angle + oldAngle) >= 3.13f || (angle + oldAngle) <= -3.13f))
		  angle *= 0.8f;
		  if (((angle + oldAngle) >= 3.13f || (angle + oldAngle) <= -3.13f))
		  angle *= 0.8f;*/
		if ((angle + oldAngle) >= 3.13f)
			angle -= (angle + oldAngle - 3.13f) - 0.02f;
		else if ((angle + oldAngle) <= -3.13f)
			angle += (angle + oldAngle + 3.13f) + 0.02f;
		/*if (((oldAngle + angle) >= 1.54f && (oldAngle + angle) <= 1.6f) || ((oldAngle + angle) <= -1.54f && (oldAngle + angle) >= -1.6f) || ((oldAngle + angle) >= 3.11f && (oldAngle + angle) <= 3.17f) || ((oldAngle + angle) <= -3.11f && (oldAngle + angle) >= -3.17f))
		angle *= 0.8f;
		if (((oldAngle + angle) >= 1.54f && (oldAngle + angle) <= 1.6f) || ((oldAngle + angle) <= -1.54f && (oldAngle + angle) >= -1.6f) || ((oldAngle + angle) >= 3.11f && (oldAngle + angle) <= 3.17f) || ((oldAngle + angle) <= -3.11f && (oldAngle + angle) >= -3.17f))
		angle *= 1.5f;
		if (((oldAngle + angle) >= 1.54f && (oldAngle + angle) <= 1.6f) || ((oldAngle + angle) <= -1.54f && (oldAngle + angle) >= -1.6f) || ((oldAngle + angle) >= 3.11f && (oldAngle + angle) <= 3.17f) || ((oldAngle + angle) <= -3.11f && (oldAngle + angle) >= -3.17f))
		return;*/

		float rotation[16] = { 1.0f, 0.0f, sinf(angle), 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -sinf(angle), 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };


		float xn = *(float*)matrix * rotation[0];
		float yn = *(float*)((DWORD)matrix + 4) * rotation[4];
		float zn = *(float*)((DWORD)matrix + 8) * rotation[8];
		*(float*)matrix = xn + yn + zn;

		xn = *(float*)((DWORD)matrix) * rotation[1];
		yn = *(float*)((DWORD)matrix + 4) * rotation[5];
		zn = *(float*)((DWORD)matrix + 8) * rotation[9];
		*(float*)((DWORD)matrix + 4) = xn + yn + zn;

		xn = *(float*)((DWORD)matrix) * rotation[2];
		yn = *(float*)((DWORD)matrix + 4) * rotation[6];
		zn = *(float*)((DWORD)matrix + 8) * rotation[10];
		*(float*)((DWORD)matrix + 8) = xn + yn + zn;

		xn = *(float*)((DWORD)matrix + 16) * rotation[0];
		yn = *(float*)((DWORD)matrix + 20) * rotation[4];
		zn = *(float*)((DWORD)matrix + 24) * rotation[8];
		*(float*)((DWORD)matrix + 16) = xn + yn + zn;

		xn = *(float*)((DWORD)matrix + 16) * rotation[1];
		yn = *(float*)((DWORD)matrix + 20) * rotation[5];
		zn = *(float*)((DWORD)matrix + 24) * rotation[9];
		*(float*)((DWORD)matrix + 20) = xn + yn + zn;

		xn = *(float*)((DWORD)matrix + 16) * rotation[2];
		yn = *(float*)((DWORD)matrix + 20) * rotation[6];
		zn = *(float*)((DWORD)matrix + 24) * rotation[10];
		*(float*)((DWORD)matrix + 24) = xn + yn + zn;

		xn = *(float*)((DWORD)matrix + 32) * rotation[0];
		yn = *(float*)((DWORD)matrix + 36) * rotation[4];
		zn = *(float*)((DWORD)matrix + 40) * rotation[8];
		*(float*)((DWORD)matrix + 32) = xn + yn + zn;

		xn = *(float*)((DWORD)matrix + 32) * rotation[1];
		yn = *(float*)((DWORD)matrix + 36) * rotation[5];
		zn = *(float*)((DWORD)matrix + 40) * rotation[9];
		*(float*)((DWORD)matrix + 36) = xn + yn + zn;

		xn = *(float*)((DWORD)matrix + 32) * rotation[2];
		yn = *(float*)((DWORD)matrix + 36) * rotation[6];
		zn = *(float*)((DWORD)matrix + 40) * rotation[10];
		*(float*)((DWORD)matrix + 40) = xn + yn + zn;

		xn = *(float*)((DWORD)matrix + 48) * rotation[0];
		yn = *(float*)((DWORD)matrix + 52) * rotation[4];
		zn = *(float*)((DWORD)matrix + 56) * rotation[8];
		*(float*)((DWORD)matrix + 48) = xn + yn + zn;

		xn = *(float*)((DWORD)matrix + 48) * rotation[1];
		yn = *(float*)((DWORD)matrix + 52) * rotation[5];
		zn = *(float*)((DWORD)matrix + 56) * rotation[9];
		*(float*)((DWORD)matrix + 52) = xn + yn + zn;

		xn = *(float*)((DWORD)matrix + 48) * rotation[2];
		yn = *(float*)((DWORD)matrix + 52) * rotation[6];
		zn = *(float*)((DWORD)matrix + 56) * rotation[10];
		*(float*)((DWORD)matrix + 56) = xn + yn + zn;
		/*float xn = *(float*)matrix*1.0f;
		float yn = 0.0f;
		float zn = 0.0f;
		*(float*)matrix = xn + yn + zn;

		xn = 0.0f;
		yn = *(float*)((DWORD)matrix + 4)*1.0f;
		if (yaw != 0.0f)
		zn = *(float*)((DWORD)matrix + 8) * (-yaw);
		else
		zn = 0.0f;
		*(float*)((DWORD)matrix + 4) = xn + yn + zn;

		xn = 0.0f;
		yn = *(float*)((DWORD)matrix + 4)*yaw;
		zn = *(float*)((DWORD)matrix + 8)*1.0f;
		*(float*)((DWORD)matrix + 8) = xn + yn + zn;

		xn = *(float*)((DWORD)matrix + 16)*1.0f;
		yn = 0.0f;
		zn = 0.0f;
		*(float*)((DWORD)matrix + 16) = xn + yn + zn;

		xn = 0.0f;
		yn = *(float*)((DWORD)matrix + 20)*1.0f;
		if (yaw != 0.0f)
		zn = *(float*)((DWORD)matrix + 24) * (-yaw);
		else
		zn = 0.0f;
		*(float*)((DWORD)matrix + 20) = xn + yn + zn;

		xn = 0.0f;
		yn = *(float*)((DWORD)matrix + 20)*yaw;
		zn = *(float*)((DWORD)matrix + 24)*1.0f;
		*(float*)((DWORD)matrix + 24) = xn + yn + zn;

		xn = *(float*)((DWORD)matrix + 32)*1.0f;
		yn = 0.0f;
		zn = 0.0f;
		*(float*)((DWORD)matrix + 32) = xn + yn + zn;

		xn = 0.0f;
		yn = *(float*)((DWORD)matrix + 36)*1.0f;
		if (yaw != 0.0f)
		zn = *(float*)((DWORD)matrix + 40) * (-yaw);
		else
		zn = 0.0f;
		*(float*)((DWORD)matrix + 36) = xn + yn + zn;

		xn = 0.0f;
		yn = *(float*)((DWORD)matrix + 36)*yaw;
		zn = *(float*)((DWORD)matrix + 40)*1.0f;
		*(float*)((DWORD)matrix + 40) = xn + yn + zn;

		xn = *(float*)((DWORD)matrix + 48)*1.0f;
		yn = 0.0f;
		zn = 0.0f;
		*(float*)((DWORD)matrix + 48) = xn + yn + zn;

		xn = 0.0f;
		yn = *(float*)((DWORD)matrix + 52)*1.0f;
		if (yaw != 0.0f)
		zn = *(float*)((DWORD)matrix + 56) * (-yaw);
		else
		zn = 0.0f;
		*(float*)((DWORD)matrix + 52) = xn + yn + zn;

		xn = 0.0f;
		yn = *(float*)((DWORD)matrix + 52)*yaw;
		zn = *(float*)((DWORD)matrix + 56)*1.0f;
		*(float*)((DWORD)matrix + 56) = xn + yn + zn;*/
	}

	void Update()//called each frame, update camera pos
	{
		printf("Update camer\n");
		static Vertex oldOffset;
		static BYTE sixth = 5;
		if (PlayerExist())
		{
			printf("Player Exists\n");
			static const DWORD ptr = 0x008E1E90;
			if (InvalidReadPtr((void*)ptr))
				return;
			DWORD camMode = *(DWORD*)ptr + 0xC0;
			camMode = *(DWORD*)camMode + 0xC;
			if (InvalidReadPtr((void*)camMode))
				return;
			camMode = *(DWORD*)camMode + 0x10;
			if (InvalidReadPtr((void*)camMode))
				return;
			camMode = *(DWORD*)camMode + 0x580;
			if (InvalidReadPtr((void*)camMode))
				return;
			if (sixth == 5)
			{
				*(DWORD*)camMode = 2;
				sixth = 0;
			}
			else
			{
				*(DWORD*)camMode = 0;
				sixth++;
			}
			printf("Got CamMode\n");
			InterpolateCamera();
			cameraAngle = atan2f((camera->X - pos->X), (camera->Z - pos->Z));
			/*float unk1 = 1.0f;   0
			float unk4 = 0.0f;   1
			float unk7 = 0.0f;   2
			0.0f    3
			float unk2 = 0.0f;   4
			float unk5 = 1.0f;   5
			float unk8 = 0.01f;  6//will make it yaw
			0.0f    7
			float unk3 = 0.0f;   8
			float unk6 = -0.01f; 9//need to angle it according to yaw
			float unk9 = 1.0f;   10
			0.0f    11








			float* pAngle = (float*)((DWORD)camera - 0x30);
			float x = *(float*)pAngle*unk1;
			float y = *(float*)((DWORD)pAngle + 4)*unk2;
			float z = *(float*)((DWORD)pAngle + 8)*unk3;
			*(float*)pAngle = x + y + z;


			x = *(float*)pAngle*unk4;
			y = *(float*)((DWORD)pAngle + 4)*unk5;
			z = *(float*)((DWORD)pAngle + 8)*unk6;
			*(float*)((DWORD)pAngle + 4) = x + y + z;

			x = *(float*)(pAngle)*unk7;
			y = *(float*)((DWORD)pAngle + 4)*unk8;
			z = *(float*)((DWORD)pAngle + 8)*unk9;
			*(float*)((DWORD)pAngle + 8) = x + y + z;


			x = *(float*)((DWORD)pAngle + 16)*unk1;
			y = *(float*)((DWORD)pAngle + 20)*unk2;
			z = *(float*)((DWORD)pAngle + 24)*unk3;
			*(float*)((DWORD)pAngle + 16) = x + y + z;

			x = *(float*)((DWORD)pAngle + 16)*unk4;
			y = *(float*)((DWORD)pAngle + 20)*unk5;
			z = *(float*)((DWORD)pAngle + 24)*unk6;
			*(float*)((DWORD)pAngle + 20) = x + y + z;

			x = *(float*)((DWORD)pAngle + 16)*unk7;
			y = *(float*)((DWORD)pAngle + 20)*unk8;
			z = *(float*)((DWORD)pAngle + 24)*unk9;
			*(float*)((DWORD)pAngle + 24) = x + y + z;

			x = *(float*)((DWORD)pAngle + 32)*unk1;
			y = *(float*)((DWORD)pAngle + 36)*unk2;
			z = *(float*)((DWORD)pAngle + 40)*unk3;
			*(float*)((DWORD)pAngle + 32) = x + y + z;

			x = *(float*)((DWORD)pAngle + 32)*unk4;
			y = *(float*)((DWORD)pAngle + 36)*unk5;
			z = *(float*)((DWORD)pAngle + 40)*unk6;
			*(float*)((DWORD)pAngle + 36) = x + y + z;

			x = *(float*)((DWORD)pAngle + 32)*unk7;
			y = *(float*)((DWORD)pAngle + 36)*unk8;
			z = *(float*)((DWORD)pAngle + 40)*unk9;
			*(float*)((DWORD)pAngle + 40) = x + y + z;

			x = *(float*)((DWORD)pAngle + 48)*unk1;
			y = *(float*)((DWORD)pAngle + 52)*unk2;
			z = *(float*)((DWORD)pAngle + 56)*unk3;
			*(float*)((DWORD)pAngle + 48) = x + y + z;

			x = *(float*)((DWORD)pAngle + 48)*unk4;
			y = *(float*)((DWORD)pAngle + 52)*unk5;
			z = *(float*)((DWORD)pAngle + 56)*unk6;
			*(float*)((DWORD)pAngle + 52) = x + y + z;

			x = *(float*)((DWORD)pAngle + 48)*unk7;
			y = *(float*)((DWORD)pAngle + 52)*unk8;
			z = *(float*)((DWORD)pAngle + 56)*unk9;
			*(float*)((DWORD)pAngle + 56) = x + y + z;*/


			* camera = *pos;
			camera->Y += 70.0f;
			/*static const DWORD ptr = 0x008E2498;
			if (InvalidReadPtr((void*)ptr))
			return;*/
			/*DWORD pSpeed = *(DWORD*)ptr + 0x13C;
			if (InvalidReadPtr((void*)pSpeed))
			return;
			pSpeed = *(DWORD*)pSpeed + 0x14;
			if (InvalidReadPtr((void*)pSpeed))
			return;
			pSpeed = *(DWORD*)pSpeed + 0x334;*/
			Vertex offset;
			offset = (*(Vertex*)velocity);
			if ((fabsf(offset.X) + fabsf(offset.Z)) != 0.0f)
			{

				float normalizer = sqrtf((offset.X * offset.X) + (offset.Z * offset.Z));
				if (normalizer)
				{
					offset.X /= normalizer;
					offset.Z /= normalizer;
					offset.X *= 150.0f;
					offset.Z *= 150.0f;
					if ((fabsf(offset.X) + fabsf(offset.Z)) >= 50.0f)
					{
						camera->X -= offset.X;
						camera->Z -= offset.Z;
						oldOffset = offset;
					}
					else
					{
						camera->X -= oldOffset.X;
						camera->Z -= oldOffset.Z;
					}
				}
				else
				{
					camera->X -= oldOffset.X;
					camera->Z -= oldOffset.Z;
				}
			}
			else
			{
				camera->X -= oldOffset.X;
				camera->Z -= oldOffset.Z;
			}

		}
	}

}; ObserveMode* observe = NULL;

Vertex pos;
float angleAxis;
float angle;
bool stored = false;

bool EnterObserveMode(CStruct* pParams, CScript* pScript)
{
	if (observe)
	{
		delete observe;
		observe = NULL;
	}
	observe = new ObserveMode;
	observe->Enter();
	printf("Entered ObserveMode, Observing player: %s\n", observe->name);
	return observe->current != NULL;
	return true;
}

bool ObserveNext(CStruct* pParams, CScript* pScript)
{
	if (observe)
	{
		observe->Next();
		return true;
	}
	return false;
}

bool LeaveObserveMode(CStruct* pParams, CScript* pScript)
{
	if (observe)
	{
		observe->Leave();
		delete observe;
		observe = NULL;
		static const DWORD ptr = 0x008E1E90;
		if (InvalidReadPtr((void*)ptr))
			return false;
		DWORD camMode = *(DWORD*)ptr + 0xC0;
		camMode = *(DWORD*)camMode + 0xC;
		if (InvalidReadPtr((void*)camMode))
			return false;
		camMode = *(DWORD*)camMode + 0x10;
		if (InvalidReadPtr((void*)camMode))
			return false;
		camMode = *(DWORD*)camMode + 0x580;
		if (InvalidReadPtr((void*)camMode))
			return false;
		*(DWORD*)camMode = 0;
	}
	return true;
}

bool StoreSkaterPos(CStruct* pParams, CScript* pScript)
{
	static const DWORD ptr = 0x008E2498;
	VALIDATE_PTR((void*)ptr);
	DWORD pPos = *(DWORD*)ptr + 0x13C;
	VALIDATE_PTR((void*)pPos);
	pPos = *(DWORD*)pPos + 0x14;
	VALIDATE_PTR((void*)pPos);
	const DWORD old = pPos;
	pPos = *(DWORD*)pPos + 0x4BC;
	if (InvalidReadPtr((void*)pPos))
	{
		pPos = old;
		pPos = *(DWORD*)pPos + 0x3B4;
		VALIDATE_PTR((void*)pPos);
	}
	pPos = *(DWORD*)pPos + 0x18;
	VALIDATE_DATA((Vertex*)pPos, sizeof(Vertex));
	stored = true;
	pos = *(Vertex*)pPos;


	angleAxis = *(float*)(pPos + 0x35C);
	angle = *(float*)(pPos + 0x364);
	return true;
}

bool SetSkaterPos(CStruct* pParams, CScript* pScript)
{
	static const DWORD ptr = 0x008E2498;
	VALIDATE_PTR((void*)ptr);
	DWORD pPos = *(DWORD*)ptr + 0x13C;
	VALIDATE_PTR((void*)pPos);
	pPos = *(DWORD*)pPos + 0x14;
	VALIDATE_PTR((void*)pPos);
	const DWORD old = pPos;
	pPos = *(DWORD*)pPos + 0x4BC;
	if (InvalidReadPtr((void*)pPos))
	{
		pPos = old;
		pPos = *(DWORD*)pPos + 0x3B4;
		VALIDATE_PTR((void*)pPos);
	}
	pPos = *(DWORD*)pPos + 0x18;
	VALIDATE_DATA((Vertex*)pPos, sizeof(Vertex));
	if (stored)
	{
		static const DWORD ptr = 0x005D06C0;
		VALIDATE_PTR((void*)ptr);
		DWORD pSpeed = *(DWORD*)ptr + 0x580;
		VALIDATE_PTR((void*)pSpeed);
		pSpeed = *(DWORD*)pSpeed + 0x4;
		VALIDATE_PTR((void*)pSpeed);
		pSpeed = *(DWORD*)pSpeed + 0x2C;
		VALIDATE_PTR((void*)pSpeed);
		pSpeed = *(DWORD*)pSpeed + 0x48;
		VALIDATE_PTR((void*)pSpeed);
		pSpeed = *(DWORD*)pSpeed + 0x334;
		VALIDATE_DATA((Vertex*)pSpeed, sizeof(Vertex));
		(*(Vertex*)pSpeed).X = 0.0f;
		(*(Vertex*)pSpeed).Y = 0.0f;
		(*(Vertex*)pSpeed).Z = 0.0f;
		*(float*)(pPos + 0x35C) = angleAxis;
		*(float*)(pPos + 0x364) = angle;
		*(Vertex*)(pPos + 16) = pos;
		*(Vertex*)pPos = pos;
		*(Vertex*)(pPos + 16) = pos;
		*(Vertex*)pPos = pos;

		*(float*)(pPos + 0x35C) = angleAxis;
		*(float*)(pPos + 0x364) = angle;

		(*(Vertex*)pSpeed).X = 0.0f;
		(*(Vertex*)pSpeed).Y = 0.0f;
		(*(Vertex*)pSpeed).Z = 0.0f;
	}
	return stored;
}

bool GetSkaterPos(CStruct* pParams, CScript* pScript)
{
	static const DWORD ptr = 0x008E2498;
	VALIDATE_PTR((void*)ptr);
	DWORD pPos = *(DWORD*)ptr + 0x13C;
	VALIDATE_PTR((void*)pPos);
	pPos = *(DWORD*)pPos + 0x14;
	VALIDATE_PTR((void*)pPos);
	const DWORD old = pPos;
	pPos = *(DWORD*)pPos + 0x4BC;
	if (InvalidReadPtr((void*)pPos))
	{
		pPos = old;
		pPos = *(DWORD*)pPos + 0x3B4;
		VALIDATE_PTR((void*)pPos);
	}
	pPos = *(DWORD*)pPos + 0x18;
	VALIDATE_DATA((Vertex*)pPos, sizeof(Vertex));
	CStructHeader* header = pScript->params->head;
	while (header)
	{
		if (header->QBkey == 0x7F261953)
		{
			*header->pVec = *(D3DXVECTOR3*)pPos;
			//header->pVec->Z-=400.0f;
			break;
		}
		header = header->NextHeader;
	}
	return true;
}



Matrix& CreateRotateMatrix(Matrix& mat, const D3DXVECTOR3& axis, const float angle)
{


	D3DXVECTOR3	unitAxis = axis;
	(*(Vertex*)&unitAxis).Normalize();

	float oneMinusCosine = 1.0f - cosf(angle);

	D3DXVECTOR3	leading;

	leading[X] = 1.0f - (unitAxis[X] * unitAxis[X]);
	leading[Y] = 1.0f - (unitAxis[Y] * unitAxis[Y]);
	leading[Z] = 1.0f - (unitAxis[Z] * unitAxis[Z]);
	leading *= oneMinusCosine;

	D3DXVECTOR3	crossed;

	crossed[X] = (unitAxis[Y] * unitAxis[Z]);
	crossed[Y] = (unitAxis[Z] * unitAxis[X]);
	crossed[Z] = (unitAxis[X] * unitAxis[Y]);
	crossed *= oneMinusCosine;

	unitAxis *= sinf(angle);

	mat.m[RIGHT][X] = 1.0f - leading[X];
	mat.m[RIGHT][Y] = crossed[Z] + unitAxis[Z];
	mat.m[RIGHT][Z] = crossed[Y] - unitAxis[Y];
	mat.m[RIGHT][W] = 0.0f;

	mat.m[UP][X] = crossed[Z] - unitAxis[Z];
	mat.m[UP][Y] = 1.0f - leading[Y];
	mat.m[UP][Z] = crossed[X] + unitAxis[X];
	mat.m[UP][W] = 0.0f;

	mat.m[AT][X] = crossed[Y] + unitAxis[Y];
	mat.m[AT][Y] = crossed[X] - unitAxis[X];
	mat.m[AT][Z] = 1.0f - leading[Z];
	mat.m[AT][W] = 0.0f;

	mat.m[POS][X] = 0.0f;
	mat.m[POS][Y] = 0.0f;
	mat.m[POS][Z] = 0.0f;
	mat.m[POS][W] = 1.0f;

	return mat;
}

enum TransferType
{
	ACID, BANK, SPINE
};

namespace Slerp
{
	static bool transfer = false;
	static Matrix start;
	static Matrix end;
	static Matrix old;
	static float duration = 0.0f;
	static float radians = 0.0f;
	static float timer = 0.0f;
	D3DXVECTOR3 axis = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Vertex vel = Vertex(0.0f, 0.0f, 0.0f);
	Vertex facing = Vertex(0.0f, 0.0f, 0.0f);
	float height = 0.0f;
	Vertex goal = Vertex(0.0f, 0.0f, 0.0f);
	bool lerp = 0.0f;
	Vertex last = Vertex(0.0f, 0.0f, 0.0f);
	bool inAcid = false;
	bool vert = false;
	bool addedvel = false;
	bool landed = true;
	bool OnGround = false;
	bool done = false;
	bool trying = false;
	float value = 0.0f;
	bool slerping = false;
	TransferType type;
	Vertex targetNormal = Vertex(0.0f, 0.0f, 0.0f);
	D3DXVECTOR4 target_normal = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);

	float speed = 0.0f;
};

bool TestForAcid(CStruct* pParams, CScript* pScript);

namespace LevelModSettings
{
	BYTE NewMenu[] = { 0x90, 0x90, 0x90, 0x90, 0x74, 0x70, 0xEB, 0x7B, 0x90 };//000404ce
	BYTE OldMenu[] = { 0xC6, 0x46, 0x04, 0x01, 0x74, 0x70, 0x57, 0x6A, 0x00 };

	bool UseNewMenu = true;
	bool AA = false;
	bool UnlimitedGraf = false;
	bool FixSound = true;
	bool TeleFix = true;
	bool grafCounter = true;
	DWORD MemorySize = 0xFA000;

	void ToggleNewMenu()
	{
		const static DWORD Addr = 0x004404CE;
		UseNewMenu = !UseNewMenu;

		if (UseNewMenu)
			memcpy((void*)Addr, &NewMenu, sizeof(NewMenu));
		else
			memcpy((void*)Addr, &OldMenu, sizeof(OldMenu));
	}


	void ToggleSoundFix()
	{
		const static DWORD Addr = 0x004C665D;
		FixSound = !FixSound;

		if (FixSound)
			*(BYTE*)Addr = 0xEB;
		else
			*(BYTE*)Addr = 0x75;
	}

	void FixTele()
	{
		const static DWORD Addr = 0x004AE562;
		const static DWORD Addr2 = 0x004AE581;
		DWORD old;
		if (VirtualProtect((LPVOID)Addr, 4, PAGE_EXECUTE_READWRITE, &old))
		{
			*(DWORD*)Addr = 0x90909090;//84 c0 75 26
			if (VirtualProtect((LPVOID)Addr2, 1, PAGE_EXECUTE_READWRITE, &old))
				*(BYTE*)Addr2 = 0x75;//74
			else
				MessageBox(0, "couldn't fix protection for", "TeleFix", 0);
		}
		else
			MessageBox(0, "couldn't fix protection for", "TeleFix", 0);
	}
};

struct D3D_PARAMS
{
};

//HRESULT (*CreateDevice)(UINT Adapter,DWORD DeviceType,HWND hFocusWindow,DWORD BehaviorFlags,D3D_PARAMS* pPresentationParameters,void** ppReturnedDeviceInterface);
//HRESULT (*Present)(LPDIRECT3DDEVICE8 pDevice, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)=NULL;
bool Initialize(CStruct*, CScript*);
HRESULT __stdcall Present(LPDIRECT3DDEVICE8 pDevice, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);
typedef HRESULT(__stdcall* Present_t)(LPDIRECT3DDEVICE8 pDevice, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);
Present_t pPresent;




struct Hook
{
	void* func;
	BYTE backup[10];

	Hook()
	{
		func = NULL;
		auto backup = { 0 };
	}


	static DWORD_PTR* GetD3D8VTable(DWORD Base, DWORD Len)
	{
		/*BYTE* pBase = (BYTE*)Base;
		for (DWORD i = 0; i < Len; i++)
		{
		  if (*pBase == 0xC7 && *(pBase + 1) == 0x06 && *(pBase + 0x06) == 0x89 && *(pBase + 0x07) == 0x86 && *(pBase + 0x0C) == 0x89 && *(pBase + 0x0D) == 0x86)
			return (DWORD*)*(DWORD*)(pBase + 2);
		  pBase++;
		}
		return (0);*/
		BYTE* pBase = (BYTE*)Base;
		DWORD ptr = (DWORD)(pBase + 0x0001A4F3);
		return (DWORD*)*(DWORD*)(&ptr);//0x000012DC);
	}

	Hook(DWORD vTableIndex, void* Function, DWORD nopCount = 0)
	{
		func = NULL;
		DWORD addr = NULL;
		do {
			addr = (DWORD)GetModuleHandle("d3d8.dll");
			Sleep(20);
		} while (!addr);
		if (!addr)
			MessageBox(0, "no d3d8 found", "", 0);
		DWORD ptr = *(DWORD*)0x00970E48;
		DWORD_PTR* vTable = (DWORD*)*(DWORD*)(ptr);
		printf("vTable %X\n", vTable);
		//DWORD_PTR* vTable = GetD3D8VTable(addr, 0x0001A4F4);//0x128000);
		if (vTable == NULL)
		{
			MessageBox(0, "no vtabl found", "", 0);
			ExitProcess(0);
		}

		addr = (DWORD)vTable[vTableIndex];
		printf("addr %X", addr);
		/*BYTE jmp[6] = { 0xe9,			//jmp
		0x00, 0x00, 0x00, 0x00,		//address
		0xc3 };						//retn

		ReadProcessMemory((HANDLE)-1, (void*)addr, backup, 6, 0);
		*(DWORD*)&jmp[1] = ((DWORD)Function - addr - 5);
		WriteProcessMemory((HANDLE)-1, (void*)addr, jmp, 6, 0);*/

		ReadProcessMemory((HANDLE)-1, (void*)addr, backup, 10, 0);

		static BYTE jmpTable[11] = { 0 };
		BYTE* jmp = jmpTable;
		DWORD dwback;
		VirtualProtect(jmp, 11, PAGE_EXECUTE_READWRITE, &dwback);
		VirtualProtect((BYTE*)addr, (5 + nopCount), PAGE_READWRITE, &dwback);
		memcpy(jmp, (BYTE*)addr, (5 + nopCount)); jmp += (5 + nopCount);
		jmp[0] = 0xE9;
		*(DWORD*)(jmp + 1) = (DWORD)((BYTE*)addr + (5 + nopCount) - jmp) - (5 + nopCount);
		((BYTE*)addr)[0] = 0xE9;
		*(DWORD*)((BYTE*)addr + 1) = (DWORD)((BYTE*)Function - (BYTE*)addr) - 5;
		//VirtualProtect((BYTE*)addr, 5, dwback, &dwback);
		func = jmp - (5 + nopCount);
		DWORD idx = 0;
		while (idx < nopCount)
		{
			*(BYTE*)((BYTE*)addr + 5 + idx) = 0x90;
			idx++;
		}
		/*char addrs[25] = "";
		sprintf(addrs, "%X %X", addr, func);
		MessageBox(0, addrs, addrs, 0);*/
	}

	Hook(const char* Dll, const char* FuncName, void* Function)
	{
		func = NULL;
		DWORD addr = (DWORD)GetProcAddress(GetModuleHandle(Dll), FuncName);
		if (addr == 0)
		{
			addr = (DWORD)GetModuleHandle("d3d8.dll");
			if (addr == 0)
			{
				MessageBox(0, "fail", "fail", MB_OK);
				return;
			}
			DWORD_PTR* vTable = GetD3D8VTable(addr, 0x0001A4F4);//0x128000);
			if (vTable == NULL)
			{
				MessageBox(0, "fail", "fail", MB_OK);
				return;
			}
			addr = (DWORD)vTable[15];
		}
		BYTE jmp[6] = { 0xe9,			//jmp
		  0x00, 0x00, 0x00, 0x00,		//address
		  0xc3 };						//retn

		ReadProcessMemory((HANDLE)-1, (void*)addr, backup, 6, 0);
		*(DWORD*)&jmp[1] = ((DWORD)Function - addr - 5);
		WriteProcessMemory((HANDLE)-1, (void*)addr, jmp, 6, 0);
	}

	void Restore()
	{
		if (func)
			WriteProcessMemory((HANDLE)-1, (void*)func, backup, 6, 0);
	}

	void Rehook(void* Function)
	{
		if (func)
		{
			BYTE jmp[6] = { 0xe9,			//jmp
			  0x00, 0x00, 0x00, 0x00,		//address
			  0xc3 };						//retn

			ReadProcessMemory((HANDLE)-1, (void*)func, backup, 6, 0);
			*(DWORD*)&jmp[1] = ((DWORD)Function - (DWORD)func - 5);
			WriteProcessMemory((HANDLE)-1, (void*)func, jmp, 6, 0);
		}
	}
};

Hook CreateDeviceHook;


Hook PresentHook;

char tags[256] = "Tags: 0";
LPD3DXFONT m_font = NULL;
RECT rct;

void CommandKick(const char* message)
{

}
void CommandBan(const char* message)
{

}
void CommandShowCommands(const char* message);
void CommandGetInfo(const char* message)
{

}
inline void SendChatMsg(char* text);
void CommandConsole(const char* message);
void CommandTell(const char* message);
void CommandDebug(const char* message);

struct Command
{
	DWORD checksum;
	void(*function)(const char* message);
	char name[12];
};
Command commands[] = { { crc32f((unsigned char*)"commands"), &CommandShowCommands, "commands" }, { crc32f((unsigned char*)"tell"), &CommandTell, "tell" }, { crc32f((unsigned char*)"kick"), &CommandKick, "kick" }, { crc32f((unsigned char*)"ban"), &CommandBan, "ban" }, { crc32f((unsigned char*)"getinfo"), &CommandGetInfo, "getinfo" }, { crc32f((unsigned char*)"console"), &CommandConsole, "console" }, { crc32f((unsigned char*)"debug"), &CommandDebug, "debug" } };
void CommandShowCommands(const char* message)
{
	char msg[128];
	DWORD k = 0;
	for (DWORD i = 1; i < sizeof(commands) / sizeof(Command); i++)
	{
		DWORD j = 0;
		while (commands[i].name[j] != 0)
		{
			msg[k] = commands[i].name[j];
			j++;
			k++;
		}
		msg[k] = 0x20;
		k++;
	}
	msg[k] = '\n';
	msg[k] = 0;
	//SendChatMsg(msg);
	printf(msg);
}



void TestInterporlator(Matrix* result, float delta)
{
	/* Cap and floor the delta */
	/* If we are at one end the solution is easy */
	if (delta <= 0.0f)
	{
		// delta = 0.0f;
		*result = Slerp::start;
		return;
	}
	else if (delta >= 1.0f)
	{
		// delta = 1.0f;
		*result = Slerp::end;
		return;
	}

#if 0
	// GJ:  always lerp, used while slerp was being debugged
	//    m_useLerp = true;
#endif

  /* Do the lerp if we are, else... */
	if (Slerp::lerp)
	{
		/* Get the lerp matrix */
		Matrix	lerp;
		D3DXVECTOR4	lpos;
		D3DXVECTOR4	spos;
		D3DXVECTOR4	epos;
		D3DXVECTOR4	rpos;

		D3DXMatrixIdentity(&lerp);

		spos = *(D3DXVECTOR4*)Slerp::start.m[POS];
		epos = *(D3DXVECTOR4*)Slerp::end.m[POS];

		*(D3DXVECTOR3*)lerp.m[RIGHT] = *(D3DXVECTOR3*)Slerp::end.m[RIGHT] - *(D3DXVECTOR3*)Slerp::start.m[RIGHT];
		*(D3DXVECTOR3*)lerp.m[UP] = *(D3DXVECTOR3*)Slerp::end.m[UP] - *(D3DXVECTOR3*)Slerp::start.m[UP];
		*(D3DXVECTOR3*)lerp.m[AT] = *(D3DXVECTOR3*)Slerp::end.m[AT] - *(D3DXVECTOR3*)Slerp::start.m[AT];
		lpos = epos - spos;

		/* Do lerp */
		(*(Vector*)lerp.m[RIGHT]).Scale(delta);
		(*(Vector*)lerp.m[UP]).Scale(delta);
		(*(Vector*)lerp.m[AT]).Scale(delta);
		(*(Vector*)&lpos).Scale(delta);

		*(D3DXVECTOR3*)(*result).m[RIGHT] = *(D3DXVECTOR3*)Slerp::start.m[RIGHT] + *(D3DXVECTOR3*)lerp.m[RIGHT];
		*(D3DXVECTOR3*)(*result).m[UP] = *(D3DXVECTOR3*)Slerp::start.m[UP] + *(D3DXVECTOR3*)lerp.m[UP];
		*(D3DXVECTOR3*)(*result).m[AT] = *(D3DXVECTOR3*)Slerp::start.m[AT] + *(D3DXVECTOR3*)lerp.m[AT];
		rpos = spos + lpos;

		(*(Vertex*)(*result).m[RIGHT]).Normalize();
		(*(Vertex*)(*result).m[UP]).Normalize();
		(*(Vertex*)(*result).m[AT]).Normalize();

		*(D3DXVECTOR4*)(*result).m[POS] = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);
		*(D3DXVECTOR3*)(*result).m[POS] = *(D3DXVECTOR3*)&rpos;
	}
	else
	{
		D3DXVECTOR3	rpos;
		D3DXVECTOR3	spos;
		D3DXVECTOR3	epos;
		D3DXVECTOR3  lpos;

		spos = *(D3DXVECTOR3*)Slerp::start.m[POS];
		epos = *(D3DXVECTOR3*)Slerp::end.m[POS];

		/* Remove the translation for now */
		*result = Slerp::start;
		*(D3DXVECTOR4*)(*result).m[POS] = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);

		/* Rotate the new matrix */
		//      m_axis[W] = 0.0f;
		/*Slerp::axis.x = 0.0f;
		Slerp::axis.y = 0.0f;*/
		(*result).Rotate(Slerp::axis, Slerp::radians * delta);//*/D3DXMatrixRotationAxis((D3DXMATRIX*)result, &Slerp::axis, Slerp::radians * delta);//result->Rotate(Slerp::axis, Slerp::radians * delta);

		/* Do linear interpolation on position */
		lpos = epos - spos;
		(*(Vertex*)&lpos).Scale(delta);
		rpos = spos + lpos;

		*(D3DXVECTOR3*)(*result).m[POS] = rpos;
	}
}

void Interporlate(Matrix* result, float delta)
{
	if (delta <= 0.0f)
	{
		// delta = 0.0f;
		*result = Slerp::start;
		return;
	}
	else if (delta >= 1.0f)
	{
		// delta = 1.0f;
		*result = Slerp::end;
		return;
	}

	if (Slerp::lerp && false)
	{
		/* Get the lerp matrix */
		Matrix	lerp;
		Vertex	lpos;
		Vertex	spos;
		Vertex	epos;
		Vertex	rpos;

		D3DXMatrixIdentity(&lerp);

		spos = *(Vertex*)&Slerp::start.m[POS];
		epos = *(Vertex*)&Slerp::end.m[POS];

		*(D3DXVECTOR4*)&lerp[RIGHT] = *(D3DXVECTOR4*)&Slerp::end[RIGHT] - *(D3DXVECTOR4*)&Slerp::start[RIGHT];
		*(D3DXVECTOR4*)&lerp[UP] = *(D3DXVECTOR4*)&Slerp::end[UP] - *(D3DXVECTOR4*)&Slerp::start[UP];
		*(D3DXVECTOR4*)&lerp[AT] = *(D3DXVECTOR4*)&Slerp::end[AT] - *(D3DXVECTOR4*)&Slerp::start[AT];
		lpos = *(Vertex*)&(epos - spos);

		/* Do lerp */
		(*(Vector*)&(lerp[RIGHT])).Scale(delta);
		(*(Vector*)&(lerp[UP])).Scale(delta);
		(*(Vector*)&(lerp[AT])).Scale(delta);
		lpos.Scale(delta);

		*(D3DXVECTOR4*)&(*result)[RIGHT] = *(D3DXVECTOR4*)&Slerp::start[RIGHT] + *(D3DXVECTOR4*)&lerp[RIGHT];
		*(D3DXVECTOR4*)&(*result)[UP] = *(D3DXVECTOR4*)&Slerp::start[UP] + *(D3DXVECTOR4*)&lerp[UP];
		*(D3DXVECTOR4*)&(*result)[AT] = *(D3DXVECTOR4*)&Slerp::start[AT] + *(D3DXVECTOR4*)&lerp[AT];
		rpos = *(Vertex*)&(spos + lpos);

		(*(Vertex*)&((*result)[RIGHT])).Normalize();
		(*(Vertex*)&((*result)[UP])).Normalize();
		(*(Vertex*)&((*result)[AT])).Normalize();

		*(D3DXVECTOR4*)&(*result).m[POS] = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);
		*(Vertex*)&(*result)[POS] = rpos;
	}
	else
	{
		Matrix	lerp;
		D3DXVECTOR4	lpos;
		D3DXVECTOR4	spos;
		D3DXVECTOR4	epos;
		D3DXVECTOR4	rpos;

		D3DXMatrixIdentity(&lerp);
		spos = *(D3DXVECTOR4*)&Slerp::start.m[POS];
		epos = *(D3DXVECTOR4*)&Slerp::end.m[POS];
		*result = Slerp::start;

		//*(D3DXVECTOR4*)&(*result).m[POS] = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);

		result->Rotate(Slerp::axis, Slerp::radians * delta);
		lpos = epos - spos;
		(*(Vector*)&lpos).Scale(delta);
		rpos = spos + lpos;

		//*(D3DXVECTOR3*)&(*result).m[POS] = *(D3DXVECTOR3*)&rpos;
	}
}

inline	float	SmoothStep(float value)
{
	// interpolates from zero to one with a zero derivative at the end-points
	return -2.0f * value * value * (value - (3.0f / 2.0f));
}

Vertex CrossProduct(Vertex* vec1, Vertex* vec2)
{
	Vertex retVec;
	retVec.X = vec1->Y * vec2->Z - vec1->Z * vec2->Y;
	retVec.Y = vec1->Z * vec2->X - vec1->X * vec2->Z;
	retVec.Z = vec1->X * vec2->Y - vec1->Y * vec2->X;
	return retVec;
}

struct SAcidDropData
{
	Vertex					target_pos;
	Vertex					target_normal;
	float						true_target_height;
	bool						lerp;
};

inline	float	ClampMax(float v, float max)
{
	if (v > max)
	{
		return max;
	}
	else
	{
		return v;
	}
}

inline	float	ClampMin(float v, float min)
{
	if (v < min)
	{
		return min;
	}
	else
	{
		return v;
	}
}

inline	float	Clamp(float v, float min, float max)
{
	return ClampMin(ClampMax(v, max), min);
}

float GetAngleAbout(Vertex& v1, Vertex& v2, Vertex& Axis)
{
	Axis.Normalize();
	v1.ProjectToPlane(Axis);
	v2.ProjectToPlane(Axis);
	v1.Normalize();
	v2.Normalize();
	Vertex v3 = CrossProduct(&Axis, &v1);
	float v1_dot = D3DXVec3Dot((D3DXVECTOR3*)&v1, (D3DXVECTOR3*)&v2);
	float v3_dot = D3DXVec3Dot((D3DXVECTOR3*)&v3, (D3DXVECTOR3*)&v2);

	float angle = acosf(Clamp(v1_dot, -1.0f, 1.0f));

	if (v3_dot < 0.0f)
	{
		angle = -angle;
	}
	return angle;
}

inline	float	Sgn(float x)
{
	if (x < 0.f)
	{
		return -1.0f;
	}
	else
	{
		return 1.0f;
	}
}

bool m_began_frame_in_transfer = false;
static BYTE test = 0;

EXTERN void Skater::MultiplyMatrix(const D3DXMATRIX& delta)
{
	matrix *= delta;
	old = matrix;
	lerp = matrix;

}

void Skater::Slerping()
{
	//printf("Slerping\n");
	//MessageBox(0, "trying to slerp", "", 0);

	//KeyState* state = this->GetKeyState(KeyState::UP);

	Slerp::timer += this->GetFrameLength();

	/*if (Slerp::timer > 0.05f && Slerp::done == false)
	{
	Matrix slerp;
	Interporlate(&slerp, 0.5f);
	Matrix inv = Slerp::old;
	inv.InvertUniform();
	Matrix delta = *(Matrix*)&(*(D3DXMATRIX*)&inv**(D3DXMATRIX*)&slerp);
	this->MultiplyMatrix(delta);
	if (!Slerp::trying)
	Slerp::done = true;
	Slerp::trying = false;
	}*/


	/**(Vertex*)delta.m[X] = Vertex(0.0f, 0.0f, 0.0f);
	delta.m[Y][W]  = 1.0f;
	*(Vertex*)delta.m[Z] = Vertex(0.0f, 0.0f, 0.0f);
	delta.m[W][X] = 0.0f;
	delta.m[W][Y] = 0.0f;
	delta.m[W][Z] = 0.0f;
	delta.m[W][W] = 1.0f;*/
	/*delta.m[W][W] = 1.0f;
	delta.m[X][W] = 0.0f;
	delta.m[Y][W] = 0.0f;
	delta.m[Z][W] = 1.0f;*/
	/*if (test == 0)
	  test = 1;
	  else if (test == 1)
	  test = 2;
	  else if (test == 2)
	  {*/
	if (Slerp::slerping)
	{
		if (Slerp::timer <= Slerp::duration * 1.0f)
		{
			Matrix slerp;
			Interporlate(&slerp, SmoothStep(ClampMax(Slerp::timer / Slerp::duration, 1.0f)));
			Matrix inv = Slerp::old;
			inv.InvertUniform();
			Matrix delta = *(Matrix*)&(*(D3DXMATRIX*)&inv * *(D3DXMATRIX*)&slerp);
			this->MultiplyMatrix(delta);
			Slerp::old = slerp;
		}
		else
		{
			Slerp::slerping = false;
			/*QBKeyHeader* header = GetQBKeyHeader(crc32f((unsigned char*)"Normal_Lerp_Speed"));
			if (header)
			{

			  Slerp::value = header->value.f;
			  header->value.f = 0.01f;
			}*/
			Vector norm = *(Vector*)&Slerp::end.m[Y];
			SetNormal(norm);
			//SetPosition(*GetHitPoint());
		}
	}




	/*if (Slerp::timer < 0.08f)
	{
	/*Matrix slerp;
	D3DXMatrixIdentity(&slerp);
	slerp.Rotate(Slerp::axis, 3.0f*Slerp::timer);
	Matrix inv = Slerp::old;
	inv.InvertUniform();
	Matrix delta = *(Matrix*)&(*(D3DXMATRIX*)&inv**(D3DXMATRIX*)&slerp);
	this->MultiplyMatrix(delta);
	Slerp::trying = false;
	Slerp::old = slerp;/
	}
	else if (Slerp::done==false)
	{
	Matrix inv = Slerp::old;
	inv.InvertUniform();
	Matrix delta = *(Matrix*)&(*(D3DXMATRIX*)&inv**(D3DXMATRIX*)&Slerp::end);
	this->MultiplyMatrix(delta);
	Slerp::done = true;
	Slerp::trying = false;
	Slerp::old = Slerp::end;
	}*/


	/*if (Slerp::timer >= Slerp::duration)
	  matrix *= Slerp::end;
	  else
	  this->GetMatrix().Rotate(Slerp::axis, Slerp::timer/Slerp::duration);*/
	  /*if (GetMatrix().m[Y][Y] > -0.05f)
		this->GetMatrix().Rotate(Slerp::axis, -3.0f*framelength);
	  test = 0;
	  //}
	  /*_asm mov ecx, this
	  static DWORD adjustnormal = 0x0049EAC0;
	  _asm call adjustnormal*/
	Slerp::addedvel = true;
	Vertex* v = (Vertex*)this->GetVelocity();
	Vertex* p = (Vertex*)this->GetPosition();
	if (v->Y < 0.0f && p->Y < Slerp::height)	// just check if we have dropped below the target height		
	{
		Slerp::vel.Normalize(0.1f); // make spine velocity very small, but still there, so camera works
		Slerp::addedvel = false;
		/*if (Slerp::timer >= Slerp::duration)
		{
		  ResetLerpingMatrix();
		  Slerp::transfer = false;
		  Slerp::slerping = false;
		  SetLanded(Slerp::landed);
		}*/
	}
	else
		*v += Slerp::vel;
	Slerp::last = *v;
}


void Skater::Store()
{
	/*skpos = position;
	skopos = oldpos;
	skvel = velocity;*/
	scol = *(Vertex*)&startcol;
	ecol = *(Vertex*)&endcol;
}

void Skater::Restore()
{
	/*position = skpos;
	oldpos = skopos;
	skvel = velocity;^*/
	scol = *(Vertex*)&startcol;
	ecol = *(Vertex*)&endcol;
}

void Skater::ResetLerpingMatrix()
{
	old = matrix;
	currentnormal = *(Vector*)&GetMatrix()[Y];
	displaynormal = *(Vector*)&GetMatrix()[Y];
	lastdisplaynormal = *(Vector*)&GetMatrix()[Y];
	normallerp = 0.0f;
	//old = matrix;
}

void OnGround()
{
	Slerp::OnGround = true;

	if (Slerp::transfer)
	{
		Skater* skater = Skater::GetSkater();
		if (skater == NULL)
			return;
		printf("landed from transfer 2\n");
		fflush(stdout);
		QBKeyHeader* header = GetQBKeyHeader(crc32f((unsigned char*)"Normal_Lerp_Speed"));
		if (header)
			header->value.f = Slerp::value;
		Slerp::transfer = false;
		Slerp::slerping = false;

		skater->ResetLerpingMatrix();
		if (Slerp::landed == false)
		{

			/**skater->GetVelocity() = Slerp::last;
			skater->GetVelocity()->RotateToNormal(Slerp::goal);
			skater->RotateToPlane();
			Vertex test_vel = *skater->GetVelocity();
			skater->remove_sideways_velocity(&test_vel);
			if (test_vel.Y > 0.0f)
			{
			  // use a standard landing instead
			  *skater->GetVelocity() = Slerp::last;
			  skater->GetVelocity()->ProjectToPlane(*skater->GetNormal());
			}

			float minSpeed = 5000.0f;
			if (skater->GetVelocity()->LengthSqr() < minSpeed*minSpeed)
			{
			  skater->GetVelocity()->Normalize(minSpeed);
			}*/
			skater->SetLanded(Slerp::vert);
			Slerp::landed = true;
			/* MessageBox(0, "", "", 0);
			 //*skater->GetVelocity() = -*skater->GetVelocity();

			 CStruct CSpeed;
			 CStructHeader hSpeed;
			 hSpeed.Type = QBKeyHeader::QBKeyType::FLOAT;
			 hSpeed.QBkey = Checksum("Speed");
			 hSpeed.value.f = Slerp::speed;
			 CSpeed.head = &hSpeed;
			 CSpeed.tail = &hSpeed;
			 ExecuteQBScript("SetSpeedScripts", &CSpeed);
			 QBKeyHeader* header = GetQBKeyHeader(Checksum("SetSpeed"));



			 printf("Set Speed%p  Vel %f %f %f\n", header, skater->GetVelocity()->x, skater->GetVelocity()->y, skater->GetVelocity()->z);*/
		}
	}
}


__declspec(naked) void OnGround_naked()
{
	static const DWORD jmpBack = 0x004A21AF + 4;
	_asm pushad;
	//printf("inside onground\n");
	OnGround();
	_asm popad;
	static DWORD unk1 = 0x0040FA90;
	_asm call unk1
	_asm jmp[jmpBack];
}

__declspec(naked) void BeganTransferCheck_naked()
{
	static DWORD retaddr = 0;
	_asm pop retaddr;
	static const DWORD limit_speed = 0x0049E260;
	_asm call limit_speed
	m_began_frame_in_transfer = Slerp::transfer;
	_asm push retaddr;
	_asm ret
}



__declspec(naked) void handle_post_transfer_limit_overrides_naked()
{

}

__declspec(naked) void ResetTransfer_naked()
{
	static const DWORD jmpBack = 0x004A3097 + 4;
	static Skater* skater = NULL;
	_asm mov skater, ebp

	_asm pushad;
	_asm pushfd;
	//_asm pushfd;
	printf("inside resettransfer\n");
	fflush(stdout);
	if (Slerp::transfer && skater)
	{
		printf("landing from transfer\n");
		fflush(stdout);
		QBKeyHeader* header = GetQBKeyHeader(crc32f((unsigned char*)"Normal_Lerp_Speed"));
		if (header)
			header->value.f = Slerp::value;
		skater->ResetLerpingMatrix();
		*(Vertex*)skater->GetVelocity() = Slerp::last;
		((Vertex*)skater->GetVelocity())->RotateToNormal(Slerp::goal);
		((Vertex*)skater->GetVelocity())->RotateToPlane(*(Vertex*)skater->GetCollisionNormal());
		/**(Vertex*)skater->GetVelocity() = Slerp::last;
		//skater->GetMatrix() = Slerp::end;
		((Vertex*)skater->GetVelocity())->RotateToNormal(Slerp::goal);
		//skater->RotateToPlane();
		Vertex test_vel = *(Vertex*)skater->GetVelocity();
		//skater->remove_sideways_velocity(&test_vel);
		if (test_vel.Y > 0.0f)
		{
		  // use a standard landing instead
		  *((Vertex*)skater->GetVelocity()) = Slerp::last;
		  ((Vertex*)skater->GetVelocity())->ProjectToPlane(*(Vertex*)skater->GetCollisionNormal());
		}*/

		float minSpeed = 500.0f;//bankdropminspeed
		if (((Vertex*)skater->GetVelocity())->LengthSqr() < minSpeed * minSpeed)
		{
			((Vertex*)skater->GetVelocity())->Normalize(minSpeed);
		}
		//*skater->GetVelocity() = -*skater->GetVelocity();
		skater->SetLanded(Slerp::vert);
		Slerp::transfer = false;
		Slerp::slerping = false;
		//skater->ResetLerpingMatrix();
		Slerp::landed = true;

		/*CStruct params;

		CStructHeader* param = params.AddParam("Speed", QBKeyHeader::QBKeyType::FLOAT);
		param->value.f = Slerp::speed;

		ExecuteQBScript("SetSpeedScripts", &params);*/



		/*QBKeyHeader* header = GetQBKeyHeader(Checksum("SetSpeed"));
		if (header)
			printf("Set Speed%p  TYPE %s Vel %f %f %f\n", header->pFunction, QBTypes[header->type], skater->GetVelocity()->x, skater->GetVelocity()->y, skater->GetVelocity()->z);
		else
			MessageBox(0, "hmm", "", 0);
		CStruct CSpeed;
		CStructHeader hSpeed;
		hSpeed.Type = QBKeyHeader::QBKeyType::FLOAT;
		hSpeed.QBkey = 0;
		hSpeed.value.f = Slerp::speed;
		CSpeed.head = &hSpeed;
		CSpeed.tail = &hSpeed;
		header->pFunction(&CSpeed, NULL);



		printf("Set Speed%p  Vel %f %f %f\n", header->pFunction, skater->GetVelocity()->x, skater->GetVelocity()->y, skater->GetVelocity()->z);*/
	}
	_asm popfd;
	_asm popad;
	static DWORD unk1 = 0x0040FA20;
	_asm call unk1
	_asm jmp[jmpBack]
}

inline bool is_vert_for_transfers(const Vertex* normal)
{
	printf("Vert, %f %f %f\n", normal->X, normal->Y, normal->Z);
	// cull out non-vert vert polys when looking for spine transfer and acid drop triggers; allows designers to be a little sloppier
	return fabsf(normal->Y) < 0.707f;
}

bool look_for_transfer_target(const Vertex& search_dir, const Vertex& start_normal, bool& hip_transfer, Vertex& target, Vertex& target_normal)
{
	// take a bunch of steps forward until we find one		
	// This is not very good, as we have to do 80 collision checks....
	// we really need to optimize our collision detection to be able to select a set of "nearby" object
	// or to select a set that intersects a sphere, or a plane
	// (here, we could just get the set that intersects the plane)
	// this could be statically cached by the colision code, and have one set
	// or perhaps more flexibly, each "feeler" could have a set of objects
	// that it deals with (defaulting to the set of all objects)

	Skater* skater = Skater::GetSkater();


	for (float step = 10.0f; step < 650.0f; step += 5.0f)
	{
		// First find a VERT point a bit in front of us
		// can be some distance below us 
		// allowing us to transfer from high to low pools
		// (and low to high, proving you can jump up from the low point to the high point first)
		/*Vertex vel = *GetVelocity();
		Vertex vel_normal = ::GetNormal(&vel);*/
		Vertex start = *(Vertex*)skater->GetPosition() + search_dir * step;		// start at current height
		//start.Y += 100.0f;
		Vertex end = start;
		end.Y -= 4500.0f;									// long way below
		skater->SetRay(*(D3DXVECTOR3*)&start, *(D3DXVECTOR3*)&end);
		if (skater->CollisionCheck(0x8, false) && is_vert_for_transfers((Vertex*)skater->GetCollisionNormal()))
		{
			printf("Found Vert\n");
			Vertex horizontal_normal = *(Vertex*)skater->GetCollisionNormal();
			horizontal_normal.Y = 0.0f;
			horizontal_normal.Normalize();
			float dot = D3DXVec3Dot((D3DXVECTOR3*)&start_normal, (D3DXVECTOR3*)&horizontal_normal);
			if (dot <= 0.95f)
			{
				target = *(Vertex*)skater->GetHitPoint();
				target_normal = *(Vertex*)skater->GetCollisionNormal();
				printf("Target %f %f %f normal %f %f %f\n", target.X, target.Y, target.Z, target_normal.X, target_normal.Y, target_normal.Z);

				hip_transfer = dot > -0.866f;

				// feeler.m_end[Y] += 3960.0f;
				//AddRay(*(D3DXVECTOR3*)&start, *(D3DXVECTOR3*)&end, D3DXCOLOR(255.0f, 100.0f, 100.0f, 255.0f));
				// feeler.DebugLine(255, 100, 100, 0);

				return true;
			}
			else
			{
				target = *(Vertex*)skater->GetHitPoint();
				target_normal = *(Vertex*)skater->GetCollisionNormal();
				printf("FAlSE Target %f %f %f normal %f %f %f\n", target.X, target.Y, target.Z, target_normal.X, target_normal.Y, target_normal.Z);
				// feeler.m_end[Y] += 3960.0f;
				//AddRay(*(D3DXVECTOR3*)&start, *(D3DXVECTOR3*)&end, D3DXCOLOR(100.0f, 255.0f, 100.0f, 255.0f));
				// feeler.DebugLine(100, 255, 100, 0);
			}
		}
		else
		{
			// feeler.m_end[Y] += 3960.0f;
			//AddRay(*(D3DXVECTOR3*)&start, *(D3DXVECTOR3*)&end, D3DXCOLOR(100.0f, 100.0f, 255.0f, 255.0f));
			// feeler.DebugLine(100, 100, 255, 0);
		}
	}

	return false;
}


float GetAirGravity()
{
	return -1350.0f / 1.1f;
}


float CalculateDuration(float target_height, float pos_Y, float vel_Y)
{
	// s = ut - 1/2 * g * t^2			 (note, -g = a in the more traditional formula)
	// solve this using the quadratic equation, gives us the formula below
	// Note the sign of s is important.....
	float distance = pos_Y - target_height;
	float vel = vel_Y;
	float acceleration = -GetAirGravity();

	return (vel + sqrtf(vel * vel + 2.0f * acceleration * distance)) / acceleration;
}

bool Skater::CheckForSpine()
{
	// Break spin button is pressed, so try to break the spine

	// The line to check along is the skater's forward directinal vector, rotated onto the XZ plane
	// if you go straight up the wall, then this will be the same as the normal of the wall (in XY) as we previously calculated
	// however we also want to handle the cases where you approach the QP at an angle

	// Need to take the forward vector (Z) and rotate it "forward" 90 degrees
	// Rotate about an axis perpendicular to both the horizontal part of m_matrix[Y] and also the world up (0,1,0)

	Vertex skater_up = *(Vertex*)GetMatrix()->m[Y];	// skater_up is expected to be horizontal here, as we are "vert"
	skater_up.Y = 0.0f;
	skater_up.Normalize();

	// get a vector perpendicular to the plane containing m_matrix[Z] and the world up 	
#if 0 // old code - crossing by axis alined	vector bugs me
	Vertex world_up(0.0f, 1.0f, 0.0f);
	Vertex side = CrossProduct(&skater_up, &world_up);
#else
	Vertex side(-skater_up.Z, 0.0f, skater_up.X);
#endif

	// assuming we have not slowed down much, then our velocity should roughly be in the direction we took off from 
	Vertex forward = *(Vertex*)GetVelocity();
	if (forward.Y < 1.0f)
		return false;
	forward.Normalize();

	Vertex wall_out = forward; 							// forward facing vector
	wall_out.Rotate(side, D3DX_PI / 2.0f);					// rotate fowrad 90 degrees
	//wall_out = -wall_out;

	float speed;
	float dist = 12.0f;
	float time = 1.0f;
	bool hip_transfer = false;

	// Here the "wall" is what we are currently skating on, anything with "wall" in the name refers to that

	Vertex target;
	Vertex target_normal;
	bool target_found = false;

	// First find a point beneath our current position
	// Nice long line, higher than we can posibly jump
	Vertex start = *(Vertex*)GetPosition() + wall_out * -0.5f;
	Vertex end = *(Vertex*)GetPosition() + wall_out * -0.5f;
	end.Y -= 4500.0f;

	// ignore everything that is NOT vert
	// feeler.SetIgnore(0, mFD_VERT);

	Vertex	wall_pos;
	SetRay(*(D3DXVECTOR3*)&start, *(D3DXVECTOR3*)&end);
	if (CollisionCheck(0x8, false))
	{
		//AddRay(*(D3DXVECTOR3*)&start, *(D3DXVECTOR3*)&end, D3DXCOLOR(0.0f, 0.0f, 0.0f, 255.0f));
		printf("found target\n");
		fflush(stdout);
		//MessageBox(0, "found wall", "", 0);
		wall_pos = *(Vertex*)GetHitPoint();

		Vertex start_normal = *(Vertex*)&this->normal;
		start_normal.Y = 0.0f;
		start_normal.Normalize();

		target_found = look_for_transfer_target(wall_out, start_normal, hip_transfer, target, target_normal);

		if (!target_found)
		{
			Vertex left_along_vert(-start_normal.Z, 0.0f, start_normal.X);

			// no target was found in the forward direction, perhaps we should look slightly left or right; look in the horizontal direction which is
			// halfway between the previous search direction and the plane of the vert
			if (!GetKeyState(KeyState::LEFT)->IsPressed() && GetKeyState(KeyState::RIGHT)->IsPressed())
			{
				Vertex search_dir = -left_along_vert + -wall_out;
				search_dir.Normalize();
				target_found = look_for_transfer_target(search_dir, start_normal, hip_transfer, target, target_normal);
			}
			else if (!GetKeyState(KeyState::RIGHT)->IsPressed() && GetKeyState(KeyState::LEFT)->IsPressed())
			{
				Vertex search_dir = left_along_vert + wall_out;
				search_dir.Normalize();
				target_found = look_for_transfer_target(search_dir, start_normal, hip_transfer, target, target_normal);
			}
		}
	}
	/*else
	  AddRay(*(D3DXVECTOR3*)&start, *(D3DXVECTOR3*)&end, D3DXCOLOR(0.0f, 255.0f, 0.0f, 255.0f));*/

	if (!target_found) return false;

	Vertex XZ_to_target = target - wall_pos;
	XZ_to_target.Y = 0.0f;
	dist = XZ_to_target.Length();

	// We are only going to allow this later if the target point is the same level
	// as the takeoff point, and we have a clear line
	// so set it to this now, so we calculate the time correctly
	target.Y = ((Vertex*)GetPosition())->Y;

	// if the two faces are not really perpendicular or if the spine is wider than
	// then we determine that we are on a "hip" and we just want to go across it without drifting left or right
	// so we want to project all our velocity straight up

	Vertex	horizontal_target_normal = target_normal;
	horizontal_target_normal.Y = 0.0f;
	horizontal_target_normal.Normalize();

	Vertex cache_vel = *(Vertex*)GetVelocity();

	float face_dot = fabsf(D3DXVec3Dot((D3DXVECTOR3*)&skater_up, (D3DXVECTOR3*)&horizontal_target_normal));
	if (face_dot < 0.9f)
	{
		((Vertex*)GetVelocity())->Y = ((Vertex*)GetVelocity())->Length();
		((Vertex*)GetVelocity())->X = 0.0f;
		((Vertex*)GetVelocity())->Z = 0.0f;
	}
	else
	{
		// if spine more than two feet wide, then also don't allow drift
#define FEET(inch) ((inch)*12.0f)
		if (dist > FEET(2.0f))
		{
			((Vertex*)GetVelocity())->Y = ((Vertex*)GetVelocity())->Length();
			((Vertex*)GetVelocity())->X = 0.0f;
			((Vertex*)GetVelocity())->Z = 0.0f;
		}
	}

	// one inch out, to ensure miss the lip
	dist += 1.0f;

#if 0 // old transfer code
	// get angle to rotate about, being the vector perpendicular to the world up vector and the difference between the two face normals
	// (generally for a spine these normals will be opposite, however they might be up to 90 degrees or more off when doing a hip)

	Mth::Vector normal_diff = target_normal - skater_up;
	normal_diff[Y] = 0.0f;
	normal_diff.Normalize();

	m_spine_rotate_axis[X] = -normal_diff[Z];
	m_spine_rotate_axis[Y] = 0.0f;
	m_spine_rotate_axis[Z] = normal_diff[X];
	m_spine_rotate_axis[W] = 0.0f;;
#endif

	// for gravity calculations, temporarily pretend we are doing spine physics, so g is constant
	/*if (target.Y > GetPosition()->Y)
	  target.Y = GetPosition()->Y;*/
	time = CalculateDuration(target.Y, ((Vertex*)GetPosition())->Y, ((Vertex*)GetVelocity())->Y);

	// subtract some frames of time, to ensure we make it
	// time -= m_frame_length * 2.0f;

	if (time < 0.1f)
	{
		time = 0.1f;
	}

	speed = dist / time;

	// if spine more than two foot wide, then make sure that we have enough speed to get over it
	// otherwise, just do a little pop over, and allow them to recover						  
	if (dist > 24.0f && speed * speed > ((Vertex*)GetVelocity())->LengthSqr() + 50.0f)
	{
		printf("not enough speed\n");
		fflush(stdout);
		return false;
	}

	// we have found a target point, either by looking directly in front or by doing the drop-down method
	// but we don't want to go for it until there is a clear line from our current position to the target

	/*Vertex	target_XZ = target;
	target_XZ.Y = GetPosition()->Y;

	start = *GetPosition();
	end = target_XZ;
	SetRay(start, end, D3DXCOLOR(255.0f, 0.0f, 0.0f, 255.0f));
	if (CollisionCheck(0x10))
	{
	  printf("too early\n");
	  // don't do anything.  We have a valid transfer but we can wait until we get high enough before we try for it
	  return true;
	}*/

	// setup the transfer's matrix slerp

	Vertex land_facing;
	if (!hip_transfer)
	{
		land_facing = target - *(Vertex*)GetPosition();
		land_facing.Y = -(land_facing.X * target_normal.X + land_facing.Z * target_normal.Z) / target_normal.Y;
		land_facing.Normalize();
	}
	else
	{
		Vertex offset = target - *(Vertex*)GetPosition();
		offset.Normalize();
		float dot = D3DXVec3Dot((D3DXVECTOR3*)&offset, (D3DXVECTOR3*)&horizontal_target_normal);
		if (dot < 0.0f)
		{
			land_facing = Vertex(0.0f, 1.0f, 0.0f);
		}
		else
		{
			land_facing = Vertex(0.0f, -1.0f, 0.0f);
		}
	}

	Slerp::start = *(Matrix*)GetMatrix();

	// calculate the facing we want when we land; retain our horizontal direction and choose a vertical component which puts us parallel so the target
	// poly's plane

	// calculate goal matrix
	//land_facing = -land_facing;
	*(Vertex*)Slerp::end.m[Z] = land_facing;
	(*(Vertex*)Slerp::end.m[Z]).ProjectToPlane(target_normal);
	(*(Vertex*)Slerp::end.m[Z]).Normalize();
	Slerp::end.m[X][Z] = 1.0f;
	*(Vertex*)Slerp::end.m[Y] = target_normal;
	Slerp::end.m[Y][W] = 1.0f;
	*(Vertex*)Slerp::end.m[X] = CrossProduct((Vertex*)Slerp::end.m[Y], (Vertex*)Slerp::end.m[Z]);
	Slerp::end.m[X][W] = 1.0f;
	*(D3DXVECTOR4*)Slerp::end.m[W] = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);

	// store the goal facing for use in adjusting the velocity at land time
	Slerp::goal = (*(Vertex*)Slerp::end.m[Z]);

	Matrix	inv;


	// Calculate the inverse transformation.
	inv = Slerp::start;
	inv.Invert();
	(D3DXMATRIX)inv = inv * Slerp::end;

	// Get the axis and angle.
	inv.GetRotationAxisAndAngle((Vertex*)&Slerp::axis, &Slerp::radians);
	const float USE_LERP_INSTEAD_DEGREES = 2.0f;
	const float USE_LERP_INSTEAD_RADIANS = USE_LERP_INSTEAD_DEGREES * D3DX_PI / 180.0f;
	Slerp::lerp = Slerp::radians < USE_LERP_INSTEAD_RADIANS;
	// if the skater is entering the spine transfer with an odd facing due to rotation, we want to preserve that angle in the slerp's goal matrix

	// calculate the deviation between the skater's velocity and facing
	float angle = GetAngleAbout(*(Vertex*)&GetMatrix()->m[Z], cache_vel, *(Vertex*)&GetMatrix()->m[Y]);

	// be a bit forgiving for hip transfers, as you often have to hit left/right to trigger them, which causes rotation

	if (fabsf(angle) < D3DXToRadian(30.0f))
	{
		angle = 0.0f;
	}

	// rotate goal facing to reflect the deviation in the initial facing
	Slerp::end.RotateYLocal(-angle);

	// setup the slerp state
	Slerp::timer = 0.0f;
	Slerp::duration = ClampMin(time, 0.9f); // clamp the time to stop super fast rotations
	Slerp::old = Slerp::start;

	// insure that the slerp takes us over the top, and doesn't invert us
	Matrix slerp_test;
	TestInterporlator(&slerp_test, 0.5f);
	if (slerp_test.m[Y][Y] < 0.0f)
	{
		Slerp::axis = -Slerp::axis;
		Slerp::radians = (2.0f * D3DX_PI) - Slerp::radians;
	}

	// remember the height we are aiming for, so when we come down through this height
	// then we remove the non vert velocity (or make it very small....)
	Slerp::height = target.Y;

	// set velocity over the wall fast enough to land on the target point																	 
	Slerp::vel = (target - *(Vertex*)GetPosition()) / time;		// velocity from start to target
	Slerp::vel.Y = 0.0f;															// but ignore Y, as gravity handles that...

	// tell the code we are doing spine physics, so we lean quicker 
	if (!hip_transfer)
	{
		/*CStruct pParams;
		ExecuteQBScript("SkaterAwardTransfer",&pParams);*/	// award a trick (might want to do it as an exception later)
	}
	else
	{
		/*CStruct pParams;
		ExecuteQBScript("SkaterAwardHipTransfer", &pParams);*/	// award a trick (might want to do it as an exception later)
	}

	// no late jumps during a transfer
	//"ollied"....

	printf("spinning target %f vel %f %f duration %f\n", target.Z, Slerp::vel.X, Slerp::vel.Z, Slerp::duration);
	fflush(stdout);
	Slerp::transfer = true;	// flag in spin physics, to do the lean forward, and also allow downcoming lip tricks
	Slerp::speed = GetSpeed((Vertex*)GetVelocity());
	printf("\nVel %f %f %f Speed %f\n", GetVelocity()->x, GetVelocity()->y, GetVelocity()->z, Slerp::speed);
	inVert = true;
	static const DWORD timer = 0x00409AE0;
	_asm call timer
	static DWORD temp = 0;
	static DWORD temp2 = 0;
	_asm mov temp, eax
	_asm mov temp2, edx
	* (DWORD*)(&inVert + 8) = temp;
	*(DWORD*)(&inVert + 0xC) = temp2;
	tracking = false;	// we are still vert, but not tracking the vert
	canbreakvert = false;	// and as we "broke" vert, we don't want to do it again
	Slerp::done = false;
	Slerp::slerping = true;
	QBKeyHeader* header = GetQBKeyHeader(crc32f((unsigned char*)"Normal_Lerp_Speed"));
	if (header)
	{

		Slerp::value = header->value.f;
		header->value.f = 0.01f;
	}

	/*Vector norm;
	norm.x = target_normal.X;
	norm.y = target_normal.Y;
	norm.z = target_normal.Z;
	norm.w = 1.0f;
	new_normal(norm);*/

	return true;
}


bool TestForClearPath(Vertex& target, Vertex& vel, Vertex& pos, Skater* skater)
{
	float initial_horiz_speed = sqrtf(vel.X * vel.X + vel.Z * vel.Z);

	Vertex drop_direction = vel;
	drop_direction.Y = 0.0f;
	float length = drop_direction.Length();
	if (length < 0.01f) return false;
	drop_direction *= 1.0f / length;
	Vertex horizontal_offset = target - pos;
	horizontal_offset.Y = 0.0f;
	float distance = horizontal_offset.Length();
	float original_target_height = target.Y;
	if (D3DXVec3Dot((D3DXVECTOR3*)&horizontal_offset, (D3DXVECTOR3*)&drop_direction) < 0.0f)
	{
		distance = -distance;
	}
	drop_direction = horizontal_offset / distance;

	// give a slight upward pop
	/*if (skated_off_edge)
	{
	vel[Y] = Mth::Max(vel[Y], GetPhysicsFloat(CRCD(0x95a79c32, "Physics_Acid_Drop_Pop_Speed")));
	}*/

	// grab the acceleration we will have during our acid drop
	float acceleration = GetAirGravity();

	// calculate what height we would have if we used our current horizontal velocity to reach the target position
	float final_height;
	/*if (distance > 0.0f && initial_horiz_speed > 0.0001f)
	{*/
	float time_to_target = distance / initial_horiz_speed;
	final_height = pos.Y + vel.Y * time_to_target + 0.5f * acceleration * time_to_target * time_to_target;
	/*}
	else
	{
	// for backwards acid drops, just act as through we are directly over the target point
	final_height = pos.Y;
	}*/
	final_height += 2.0f;
	/*if (final_height > pos.Y)
	  final_height = pos.Y;*/

	  // if we can't reach the target with our current velocity, ditch the acid drop
	if (final_height < target.Y)
	{
		//MessageBox(0, "3", "", 0);
		return false;
	}

	// calculate the air time before the acid drop would hit its true target; prevent acid drops from occuring moments before landing
	float time_to_reach_target_height = CalculateDuration(original_target_height, pos.Y, vel.Y);
	/*if (time_to_reach_target_height < 0.5f)
	{
	  //MessageBox(0, "2", "", 0);
	  return false;
	}*/

	// ensure that we have a clear shot to the target

	bool clear_path = false;

	// keep shifting our target point up until we can get a clear shot to it, or we get to an unreachable height
	while (target.Y < final_height)
	{
		Vertex cstart = pos;

		// check a path constructed from two concatenated lines, with the midpoint halfway along the acid drop trajectory; this is an attempt
		// to allow most acid drop which might be disallowed by a ledge which would block a straight line

		// calculate the time span required to fall to the target height
		float half_time_to_reach_target_height = 0.5f * CalculateDuration(target.Y, pos.Y, vel.Y);

		// calculate the spine velocity which would be used for this target
		float required_speed = 0.5f * distance / half_time_to_reach_target_height;

		// calculate the height we will be at halfway through the acid drop
		float height_halfway = pos.Y + vel.Y * half_time_to_reach_target_height
			+ 0.5f * acceleration * half_time_to_reach_target_height * half_time_to_reach_target_height;//sqrtf(half_time_to_reach_target_height);

		  // calculate the point halfway through the acid drop
		Vertex halfway_point = pos;
		halfway_point.Y = height_halfway;
		halfway_point += drop_direction * required_speed * half_time_to_reach_target_height;

		// check for collisions alone the two-line path
		Vertex cend = halfway_point;
		skater->SetRay(*(D3DXVECTOR3*)&cstart, *(D3DXVECTOR3*)&cend);
		if (!skater->CollisionCheck(0x10))
		{
			// feeler.DebugLine(255, 255, 0);
			cstart = cend;
			cend = target;
			cend.Y += 2.0f;
			skater->SetRay(*(D3DXVECTOR3*)&cstart, *(D3DXVECTOR3*)&cend);
			if (!skater->CollisionCheck(0x10))
			{
				// feeler.DebugLine(255, 255, 0);
				return true;
			}
			else
			{
				// MessageBox(0, "hited", "", 0);
				 // feeler.DebugLine(0, 0, 0, 0);
			}
		}

		// feeler.DebugLine(0, 0, 0, 0);

		// try a higher target point
		target.Y += 24.0f;
	}

	return false;
}

bool maybe_acid(bool skated_off_edge, const Vertex& pos, const Vertex& old_pos, Vertex& vel, SAcidDropData& acid_drop_data)
{

	Skater* skater = Skater::GetSkater();
	// horizontal direction in which a drop would occur
	Vertex drop_direction = vel;
	drop_direction.Y = 0.0f;
	float length = drop_direction.Length();
	if (length < 0.01f) return false;
	drop_direction *= 1.0f / length;

	bool target_found = false;
	Vertex target;

	// in order not to miss vert polys with a thin horizontal projection, we check for them starting at this frame's initial position
	Vertex search_pos = old_pos;
	search_pos.Y = std::fmaxf(pos.Y, old_pos.Y);

	float scan_distance = 500.0f;
	float scan_height = 0.0f;


	Vertex target_normal;
	Vertex horizontal_target_normal;
	float distance;
	for (distance = 0.01f; distance < scan_distance; distance += 3.0f)
	{
		// look for a vert poly below us
		Vertex cstart = drop_direction * distance + search_pos;
		Vertex cend = cstart;
		cstart.Y += scan_height;
		cend.Y -= 4000.0f;
		skater->SetRay(*(D3DXVECTOR3*)&cstart, *(D3DXVECTOR3*)&cend);
		if (skater->CollisionCheck(0x8, false) && is_vert_for_transfers((Vertex*)skater->GetCollisionNormal()))
		{
			// the horizontal projection of the vert's normal just correspond somewhat to our direction			 
			target_normal = horizontal_target_normal = *(Vertex*)skater->GetCollisionNormal();
			horizontal_target_normal.Y = 0.0f;
			horizontal_target_normal.Normalize();


			target_found = D3DXVec3Dot((D3DXVECTOR3*)&drop_direction, (D3DXVECTOR3*)&horizontal_target_normal) >= 0.05f;

			if (target_found)
			{
				target = *(Vertex*)skater->GetHitPoint();
				// feeler.m_end[Y] += 3960.0f;
				// feeler.DebugLine(255, 100, 100, 0);
				break;
			}
			else
			{
				// feeler.m_end[Y] += 3960.0f;
				// feeler.DebugLine(100, 255, 100, 0);
			}
		}
		else
		{
			// feeler.m_end[Y] += 3960.0f;
			// feeler.DebugLine(100, 100, 255, 0);
		}

		// use a larger incrememt at larger distances, as we have several frames yet to find these polys
		if (distance > 100.0f)
		{
			distance += 24.0f;
		}
	}

	if (!target_found)
	{
		// no valid acid drop target found
		return false;
	}

	float original_target_height = target.Y;

	// because our search began behind us, the horizontal offset to the target may not be forward
	Vertex horizontal_offset = target - pos;
	horizontal_offset.Y = 0.0f;
	distance = horizontal_offset.Length();
	if (D3DXVec3Dot((D3DXVECTOR3*)&horizontal_offset, (D3DXVECTOR3*)&drop_direction) < 0.0f)
	{
		distance = -distance;
	}
	drop_direction = horizontal_offset / distance;

	// stash a copy of velocity so we can pretend it has an adjusted value
	Vertex hold_vel = vel;

	// calculate our effective horizontal velocity
	float initial_horiz_speed = sqrtf(vel.X * vel.X + vel.Z * vel.Z);

	// give a slight upward pop
	/*if (skated_off_edge)
	{
	vel[Y] = Mth::Max(vel[Y], GetPhysicsFloat(CRCD(0x95a79c32, "Physics_Acid_Drop_Pop_Speed")));
	}*/

	// grab the acceleration we will have during our acid drop
	float acceleration = GetAirGravity();

	// calculate what height we would have if we used our current horizontal velocity to reach the target position
	float final_height;
	if (distance > 0.0f && initial_horiz_speed > 0.0001f)
	{
		float time_to_target = distance / initial_horiz_speed;
		final_height = pos.Y + vel.Y * time_to_target + 0.5f * acceleration * time_to_target * time_to_target;
	}
	else
	{
		// for backwards acid drops, just act as through we are directly over the target point
		final_height = pos.Y;
	}

	// if we can't reach the target with our current velocity, ditch the acid drop
	if (final_height < target.Y)
	{
		vel = hold_vel;
		return false;
	}

	// calculate the air time before the acid drop would hit its true target; prevent acid drops from occuring moments before landing
	if (original_target_height > ((Vertex*)skater->GetPosition())->Y)
		original_target_height = ((Vertex*)skater->GetPosition())->Y;
	float time_to_reach_target_height = CalculateDuration(original_target_height, pos.Y, vel.Y);
	if (time_to_reach_target_height < 1.0f)
	{
		vel = hold_vel;
		return false;
	}

	// ensure that we have a clear shot to the target

	bool clear_path = false;

	// keep shifting our target point up until we can get a clear shot to it, or we get to an unreachable height
	while (target.Y < final_height)
	{
		Vertex cstart = pos;

		// check a path constructed from two concatenated lines, with the midpoint halfway along the acid drop trajectory; this is an attempt
		// to allow most acid drop which might be disallowed by a ledge which would block a straight line

		// calculate the time span required to fall to the target height
		if (target.Y > ((Vertex*)skater->GetPosition())->Y)
			target.Y = ((Vertex*)skater->GetPosition())->Y;
		float half_time_to_reach_target_height = 0.5f * CalculateDuration(target.Y, pos.Y, vel.Y);

		// calculate the spine velocity which would be used for this target
		float required_speed = 0.5f * distance / half_time_to_reach_target_height;

		// calculate the height we will be at halfway through the acid drop
		float height_halfway = pos.Y + vel.Y * half_time_to_reach_target_height
			+ 0.5f * acceleration * half_time_to_reach_target_height * half_time_to_reach_target_height;// sqrtf(half_time_to_reach_target_height);

		  // calculate the point halfway through the acid drop
		Vertex halfway_point = pos;
		halfway_point.Y = height_halfway;
		halfway_point += drop_direction * required_speed * half_time_to_reach_target_height;

		// check for collisions alone the two-line path
		Vertex cend = halfway_point;
		skater->SetRay(*(D3DXVECTOR3*)&cstart, *(D3DXVECTOR3*)&cend);
		if (!skater->CollisionCheck(0x10))
		{
			// feeler.DebugLine(255, 255, 0);
			cstart = cend;
			cend = target;
			cend.Y += 1.0f;
			skater->SetRay(*(D3DXVECTOR3*)&cstart, *(D3DXVECTOR3*)&cend);
			if (!skater->CollisionCheck(0x10))
			{
				// feeler.DebugLine(255, 255, 0);
				clear_path = true;
				break;
			}
			else
			{
				// feeler.DebugLine(0, 0, 0, 0);
			}
		}

		// feeler.DebugLine(0, 0, 0, 0);

		// try a higher target point
		target.Y += 24.0f;
	}

	// no clear path was found along the acid drop
	if (!clear_path)
	{
		vel = hold_vel;
		return false;
	}

	acid_drop_data.target_pos = target;
	acid_drop_data.target_normal = target_normal;
	acid_drop_data.true_target_height = original_target_height;
	return true;
}

typedef bool(__thiscall* const pCollisionCheck)(Skater* pThis, BYTE ignore, BYTE flags, BYTE ignore2);
EXTERN bool Skater::CollisionCheck(BYTE flag, bool ignore)
{
	if (ignore)
		return pCollisionCheck(0x0049FC80)(this, flag, 0, 0);
	else
		return pCollisionCheck(0x0049FC80)(this, 0, flag, 0);
}


EXTERN void Skater::SetRay(D3DXVECTOR3 start, D3DXVECTOR3 end)
{
	this->startcol = start;
	this->endcol = end;

	/*f_AddRay AddRayd = (f_AddRay)GetProcAddress(GetModuleHandle("d3d8.dll"), "AddRay");
	if (AddRayd == NULL)
	MessageBox(0, "WTF?", "", 0);
	AddRayd(start, end, c);*/
}

EXTERN void Skater::SetVertAir(bool value)
{
	inVert = value;
	tracking = false;
	//canbreakvert = false;
	autoturn = false;
	static const DWORD timer = 0x00409AE0;
	_asm call timer
	static DWORD temp = 0;
	static DWORD temp2 = 0;
	_asm mov temp, eax
	_asm mov temp2, edx

	* (DWORD*)(&inVert + 8) = temp;
	*(DWORD*)(&inVert + 0xC) = temp2;


}

void EnterAcid(const SAcidDropData& data)
{
	DEBUGSTART()
	{
		Skater* skater = Skater::GetSkater();
		printf("EnterAcid\n");
		fflush(stdout);
		//Slerp::done = false;
		//oldMatrix = this->matrix;
		//MessageBox(0, "EnterAcid", "", 0);
		const Vertex& target_pos = data.target_pos;
		const Vertex& target_normal = data.target_normal;
		const float& true_target_height = data.true_target_height;

		Vertex horizoffset = target_pos - *(Vertex*)skater->GetPosition();
		horizoffset.Y = 0.0f;
		float distance = horizoffset.Length();

		if (D3DXVec3Dot((D3DXVECTOR3*)&horizoffset, (D3DXVECTOR3*)skater->GetVelocity()) < 0.0f)
			distance = -distance;
		//distance += 2.0f;

		Vertex dropdirr = horizoffset / distance;

		//Slerp::transfer = true;
		/*float posY = target_pos.Y;
		if (target_pos.Y > this->GetPosition()->Y)
		  MessageBox(0, "WTF", "", 0);
		if (target_pos.Y > GetPosition()->Y)
		  posY = GetPosition()->Y;*/
		if (target_pos.Y > ((Vertex*)skater->GetPosition())->Y)
			Slerp::duration = CalculateDuration(true_target_height, ((Vertex*)skater->GetPosition())->Y, ((Vertex*)skater->GetVelocity())->Y);
		else
			Slerp::duration = CalculateDuration(target_pos.Y, ((Vertex*)skater->GetPosition())->Y, ((Vertex*)skater->GetVelocity())->Y);
		printf("duration %f target %f vel %f\n", Slerp::duration, target_pos.Y, ((Vertex*)skater->GetVelocity())->Y);
		fflush(stdout);
		//Slerp::transfer = false;
		float reqspeed = distance / Slerp::duration;

		Slerp::vel = Vertex(reqspeed * dropdirr.X, 0.0f, reqspeed * dropdirr.Z);
		Slerp::vel.Y = 0.0f;
		Slerp::height = target_pos.Y;

		//MessageBox(0, "VertTrue?", "", 0);
		skater->SetVertAir(true);
		//MessageBox(0, "Ya", "", 0);
		Slerp::transfer = true;
		Slerp::speed = GetSpeed((Vertex*)skater->GetVelocity());
		printf("\nVel %f %f %f Speed %f\n", skater->GetVelocity()->x, skater->GetVelocity()->y, skater->GetVelocity()->z, Slerp::speed);
		//MessageBox(0, "Ya2", "", 0);

		((Vertex*)skater->GetVelocity())->X = 0.0f;
		((Vertex*)skater->GetVelocity())->Z = 0.0f;

		Slerp::start = *(Matrix*)skater->GetMatrix();
		/*Slerp::facing = dropdirr;
		Slerp::facing.Y = -(Slerp::facing.X * Slerp::facing.X + Slerp::facing.Z * Slerp::facing.Z) / target_normal.Y;
		Slerp::facing.Normalize();*/
		Slerp::facing = target_pos - *(Vertex*)skater->GetPosition();
		Slerp::facing.Y = -(Slerp::facing.X * target_normal.X + Slerp::facing.Z * target_normal.Z) / target_normal.Y;
		Slerp::facing.Normalize();
		//Slerp::facing = -Slerp::facing;

		*(Vertex*)Slerp::end.m[Z] = Slerp::facing;
		(*(Vertex*)Slerp::end.m[Z]).ProjectToPlane(target_normal);
		(*(Vertex*)Slerp::end.m[Z]).Normalize();
		Slerp::end.m[Z][W] = 1.0f;
		*(Vertex*)Slerp::end.m[Y] = target_normal;
		Slerp::end.m[Y][W] = 1.0f;
		*(Vertex*)Slerp::end.m[X] = CrossProduct((Vertex*)Slerp::end.m[Y], (Vertex*)Slerp::end.m[Z]);
		Slerp::end.m[X][W] = 1.0f;
		*(D3DXVECTOR4*)Slerp::end.m[W] = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f);

		Slerp::goal = (*(Vertex*)&(Slerp::end.m[Z]));

		Matrix	inv;


		// Calculate the inverse transformation.
		inv = Slerp::start;
		inv.Invert();
		(D3DXMATRIX)inv = inv * Slerp::end;

		// Get the axis and angle.
		/*inv.GetRotationAxisAndAngle((Vertex*)&Slerp::axis, &Slerp::radians);
		const float USE_LERP_INSTEAD_DEGREES = 2.0f;
		const float USE_LERP_INSTEAD_RADIANS = USE_LERP_INSTEAD_DEGREES * D3DX_PI / 180.0f;
		Slerp::lerp = Slerp::radians < USE_LERP_INSTEAD_RADIANS;*/

		Vertex horizfacing = *(Vertex*)&(skater->GetMatrix()->m[Z]);
		horizfacing.Y = 0.0f;
		float angle = GetAngleAbout(horizfacing, dropdirr, *(Vertex*)&(skater->GetMatrix()[Y]));
		Slerp::end.RotateYLocal(-angle);
		Slerp::timer = 0.0f;
		Slerp::old = Slerp::start;


		QBKeyHeader* header = GetQBKeyHeader(Checksums::Normal_Lerp_Speed);
		if (header)
		{

			Slerp::value = header->value.f;
			header->value.f = 0.1f;
		}

		Vector norm;
		norm.x = target_normal.X;
		norm.y = target_normal.Y;
		norm.z = target_normal.Z;
		norm.w = 1.0f;
		skater->SetNormal(norm);
		//MessageBox(0, "done acid", "", 0);
		Slerp::slerping = false;
		Slerp::done = false;
	}
	DEBUGEND()
}

bool TestForAcid(CStruct* pParams, CScript* pScript)
{
	if (Slerp::inAcid || Slerp::transfer)
		return false;
	Slerp::landed = false;
	SAcidDropData acid_drop_data;
	Skater* skater = Skater::GetSkater();
	skater->Store();
	if (skater->InVert())
	{
		//MessageBox(0, "INVERT", "", MB_OK);
		if (skater->CheckForSpine())
		{
			Slerp::type = SPINE;
			Slerp::vert = true;
			skater->Restore();
			return Slerp::transfer;
		}
	}
	else
	{

		Vertex pos = *(Vertex*)skater->GetOldPosition();
		pos.X = max(pos.X, skater->GetPosition()->x);
		Vertex start = pos;
		Vertex end = start;

		Vertex velocity = *(Vertex*)skater->GetVelocity();


		Vertex vel_normal;// = GetNormal(&velocity);

		vel_normal = velocity;
		vel_normal.Y = 0.0f;
		float length = vel_normal.Length();
		if (length < 0.01f) return false;
		vel_normal *= 1.0f / length;

		for (float dist = 0.01f; dist < 550.0f; dist += 3.0f)
		{


			start.X = pos.X + vel_normal.X * dist;
			start.Z = pos.Z + vel_normal.Z * dist;

			end.X = pos.X + vel_normal.X * dist;
			end.Y = pos.Y - 4000.0f;
			end.Z = pos.Z + vel_normal.Z * dist;
			//velocity.Y -= 0.5f;
			skater->SetRay(*(D3DXVECTOR3*)&start, *(D3DXVECTOR3*)&end);


			if (skater->CollisionCheck(0x8, false) && is_vert_for_transfers((Vertex*)skater->GetCollisionNormal()))
			{
				Vertex target_normal = *(Vertex*)skater->GetCollisionNormal();
				Vertex vel = *(Vertex*)skater->GetVelocity();
				Vertex target = *(Vertex*)skater->GetHitPoint();
				Vertex truetarget = target;
				//printf("above\n");
				fflush(stdout);

				/*if (!TestForClearPath(target, vel, pos, skater))
				  continue;*/
				  //
				float height = ((Vertex*)skater->GetPosition())->Y - truetarget.Y;

				if (height < 20.0f)
					continue;

				/*float time = skater->CalculateDuration(skater->GetHitPoint()->Y, skater->GetPosition()->Y, skater->GetVelocity()->Y);

				// subtract some frames of time, to ensure we make it
				// time -= m_frame_length * 2.0f;

				if (time < 0.1f)
				{
				  time = 0.1f;
				}

				float speed = dist / time;

				// if spine more than two foot wide, then make sure that we have enough speed to get over it
				// otherwise, just do a little pop over, and allow them to recover
				if (dist > 24.0f && speed * speed > skater->GetVelocity()->LengthSqr())
				{
				  printf("not enough speed\n");
				  continue;
				}*/

				Vertex horizontal_target_normal = target_normal;
				horizontal_target_normal.Y = 0.0f;
				horizontal_target_normal.Normalize();

				if (D3DXVec3Dot((D3DXVECTOR3*)&vel_normal, (D3DXVECTOR3*)&horizontal_target_normal) >= 0.05f)
				{
					/*Vertex target_normal = *skater->GetCollisionNormal();
					Vertex vel = *skater->GetVelocity();
					Vertex target = *skater->GetHitPoint();
					Vertex truetarget = target;
					printf("above\n");
					fflush(stdout);*/

					if (TestForClearPath(target, vel, pos, skater))
					{
						printf("above\n");
						skater->Restore();
						Slerp::inAcid = true;
						Slerp::type = ACID;
						//skater->SetNormals();
						//skater->GetVelocity()->Y = velocity.Y;
						SAcidDropData acid_drop_data;
						acid_drop_data.target_pos = target;
						//acid_drop_data.target_pos.Y += 2.0f;
						acid_drop_data.target_normal = target_normal;

						acid_drop_data.true_target_height = truetarget.Y;
						Slerp::vert = true;
						EnterAcid(acid_drop_data);
						Slerp::inAcid = false;
						return true;
					}
					else
						printf("no clear path?\n");
				}
			}

			if (dist > 150.0f)
			{
				dist += 24.0f;
			}
		}

		/*Vertex vel = *skater->GetVelocity();
		if (skater->maybe_acid_drop(false, *skater->GetPosition(), *skater->GetOldPosition(), *skater->GetVelocity(), acid_drop_data))
		{
		  Slerp::type = ACID;
		  Slerp::inAcid = true;
		  Slerp::vert = true;
		  skater->EnterAcid(acid_drop_data);
		  Slerp::inAcid = false;
		  return true;
		}*/
		//*skater->GetVelocity() = vel;
		skater->Restore();
		int num_hits = 0;

		skater->Store();

		pos = *skater->GetOldPosition();
		start = pos;
		end = start;

		velocity = *skater->GetVelocity();


		for (float dist = 0.01f; dist < 550.0f; dist += 3.0f)
		{
			Vertex vel_normal = GetNormal(&velocity);
			vel_normal.Y = 0.0f;

			start.X = pos.X + vel_normal.X * dist;
			start.Z = pos.Z + vel_normal.Z * dist;

			end.X = pos.X + vel_normal.X * dist;
			end.Y = pos.Y - 4000.0f;
			end.Z = pos.Z + vel_normal.Z * dist;
			velocity.Y -= 0.5f;
			skater->SetRay(*(D3DXVECTOR3*)&start, *(D3DXVECTOR3*)&end);


			if (skater->CollisionCheck(0x10))
			{
				Vertex target_normal = *(Vertex*)skater->GetCollisionNormal();
				Vertex vel = *(Vertex*)skater->GetVelocity();
				Vertex target = *(Vertex*)skater->GetHitPoint();
				Vertex truetarget = target;
				//printf("above\n");
				fflush(stdout);

				/*if (!TestForClearPath(target, vel, pos, skater))
				  continue;*/
				float height = ((Vertex*)skater->GetPosition())->Y - truetarget.Y;

				if (height < 20.0f)
					continue;


				float absnorm = fabsf(target_normal.Y);
				if (absnorm > 0.48f && absnorm < 0.98f)
				{
					//printf("norm %f %f vel %f %f", vel_normal.X, vel_normal.Z, velocity.X, velocity.Z);
					float test = ((vel_normal.X - target_normal.X) + (vel_normal.Z - target_normal.Z));
					if (fabsf(test) < 0.82f)
					{
						printf("above\n");
						fflush(stdout);
						/*num_hits++;

						if (num_hits == 3)
						{*/

						/*Vertex target_normal = *skater->GetCollisionNormal();
						Vertex vel = *skater->GetVelocity();
						Vertex target = *skater->GetHitPoint();
						Vertex truetarget = target;
						printf("above\n");
						fflush(stdout);*/

						if (TestForClearPath(target, vel, pos, skater))
						{

							skater->Restore();
							Slerp::inAcid = true;
							Slerp::type = BANK;
							//skater->SetNormals();
							//skater->GetVelocity()->Y = velocity.Y;
							SAcidDropData acid_drop_data;
							acid_drop_data.target_pos = target;
							//acid_drop_data.target_pos.Y += 2.0f;
							acid_drop_data.target_normal = target_normal;

							acid_drop_data.true_target_height = truetarget.Y;
							Slerp::vert = false;
							EnterAcid(acid_drop_data);
							Slerp::inAcid = false;
							return true;
						}
						else
							printf("no clear path?\n");

						/*skater->Restore();
						Slerp::inAcid = true;
						Slerp::type = BANK;
						//skater->SetNormals();
						//skater->GetVelocity()->Y = velocity.Y;
						SAcidDropData acid_drop_data;
						acid_drop_data.target_pos = *skater->GetHitPoint();
						acid_drop_data.target_pos.Y += 1.0f;
						acid_drop_data.target_normal = *skater->GetCollisionNormal();

						acid_drop_data.true_target_height = acid_drop_data.target_pos.Y;
						Slerp::vert = false;
						skater->EnterAcid(acid_drop_data);
						Slerp::inAcid = false;
						return true;*/
						//}

						if (num_hits > 3)
						{
						}

						/*pos = *skater->GetPosition();
						Vertex hit = *skater->GetHitPoint();
						float oldY = pos.Y;
						Vertex transport;
						transport.X = hit.X - pos.X;
						transport.Z = hit.X - pos.Z;

						pos.X += transport.X*0.2f;
						pos.Z += transport.Z*0.2f;
						skater->SetPosition(pos);*/


					}
					else
						num_hits = 0;
				}
			}

			if (dist > 150.0f)
			{
				dist += 24.0f;
			}
		}
	}
	Slerp::inAcid = false;
	Slerp::transfer = false;
	skater->Restore();
	return false;
}


void MaybeAcid()
{
	if (Slerp::inAcid || Slerp::transfer)
		return;

	SAcidDropData acid_drop_data;
	Skater* skater = Skater::GetSkater();
	if (skater == NULL)
		return;
	skater->Store();
	if (skater && !Slerp::OnGround && skater->GetKeyState(SpineButton)->IsPressed())
	{
		printf("trying acid\n");
		fflush(stdout);
		/*if (skater->maybe_acid_drop(false, *skater->GetPosition(), *skater->GetOldPosition(), *skater->GetVelocity(), acid_drop_data))
		  skater->EnterAcid(acid_drop_data);
		  else
		  {*/
		if (TestForAcid(NULL, NULL))
		{
			printf("acid returned true\n");
			fflush(stdout);
			Slerp::trying = true;
			Slerp::done = false;
			switch (Slerp::type)
			{
			case ACID:
				skater->FlagException("AcidDrop");
				break;
			case BANK:
				skater->FlagException("BankDrop");
				break;
			case SPINE:
				skater->FlagException("SpineTransfer");
				break;
			}
		}
		//}
	}
	skater->Restore();
	Slerp::OnGround = false;
}

__declspec(naked) void MaybeAcid_naked()
{
	static const DWORD jmpBack = 0x004A8AE0 + 4;
	_asm pushad;
	_asm pushfd;
	/*printf("inside maybeacidd\n");
	fflush(stdout);*/
	MaybeAcid();
	_asm popfd;
	_asm popad;
	static const DWORD handle_air = 0x004A25A0;
	_asm call handle_air
	_asm jmp[jmpBack];
}

__declspec(naked) void CheckForTransfer_naked()
{
	static Skater* skater = NULL;
	_asm mov skater, ebp
	_asm pushad;
	static const DWORD jmpBack = 0x004A2DAB + 4;
	static bool ok = true;
	//_asm pop retaddr;

	if (Slerp::transfer == false || Slerp::slerping == false)
	{
		ok = false;
		_asm mov ecx, skater
		static DWORD adjustnormal = 0x0049EAC0;
		_asm call adjustnormal
		if (Slerp::type == SPINE && Slerp::transfer)
			ok = true;
	}
	else
	{
		//_asm pushad;
		//_asm pushfd;
		//printf("inside checkfortransfer\n");
		//skater->ResetLerpingMatrix();
		//_asm popfd;
		//_asm popad
	}
	//_asm push retaddr;
	_asm popad;
	_asm mov al, ok
	_asm jmp[jmpBack]
}

__declspec(naked) void ResetTransferVel_naked()
{
	static Vertex* velocity = NULL;
	_asm mov velocity, ebx
	static const DWORD jmpBack = 0x004A27B5 + 4;
	_asm pushad;
	_asm pushfd;
	//_asm pushfd;
	//printf("inside resettrasnferver\n");
	//_asm popfd;
	if (Slerp::transfer && Slerp::addedvel && velocity)
	{
		printf("Reset velocity %f %f %f\n", velocity->X, velocity->Y, velocity->Z);
		*velocity -= Slerp::vel;
	}
	_asm popfd;
	_asm popad;
	static DWORD vrtx = 0x0048A720;
	_asm call vrtx;
	_asm jmp[jmpBack];
}

__declspec(naked) void Slerp_naked()
{
	/* _asm pushad;
	 _asm pushfd;*/
	static Skater* skater = NULL;
	_asm mov skater, ebp
	static const DWORD jmpBack = 0x004A25D9 + 4;
	//_asm pushfd;
	//printf("inside slerp\n");
	if (!Slerp::transfer)// && skater->InVert())
	{
		static DWORD lean = 0x0049CB60;
		_asm call lean
	}
	else if (Slerp::transfer)
	{
		_asm pushad;
		if (skater)
			skater->Slerping();
		_asm popad;
	}
	//_asm popfd;
	/*_asm popfd;
	_asm popad;*/
	_asm jmp[jmpBack];
}



bool GetSpeedScript(CStruct* pParams, CScript* pScript)
{
	DEBUGSTART()
	{
		//ExecuteQBScript("DefaultGapScript", pParams, pScript);
		//ExecuteQBScript("DefaultGapScript", pParams, pScript);
		static const DWORD ptr = 0x005D06C0;
		VALIDATE_PTR((void*)ptr);
		DWORD pSpeed = *(DWORD*)ptr + 0x580;
		VALIDATE_PTR((void*)pSpeed);
		pSpeed = *(DWORD*)pSpeed + 0x4;
		VALIDATE_PTR((void*)pSpeed);
		pSpeed = *(DWORD*)pSpeed + 0x2C;
		VALIDATE_PTR((void*)pSpeed);
		pSpeed = *(DWORD*)pSpeed + 0x48;
		VALIDATE_PTR((void*)pSpeed);
		pSpeed = *(DWORD*)pSpeed + 0x334;
		VALIDATE_DATA((Vertex*)pSpeed, sizeof(Vertex));

		CStruct* const __restrict params = pScript->params;
		CStructHeader* const __restrict header = params->AllocateCStruct();
		params->tail->NextHeader = header;
		params->tail = header;
		header->Type = QBKeyHeader::FLOAT;
		header->QBkey = 0xF0D90109;
		const float speed = GetSpeed((const Vertex* const __restrict)pSpeed);
		if (speed > 0)
			header->value.f = speed;
		else
			header->value.f = 100.0f;
		header->NextHeader = NULL;
		//CStructHeader header = CStructHeader(QBKeyHeader::FLOAT, 0x3BD7B54E, speed != 0 ? speed : 100.0f, 0);

		/*typedef const int (__cdecl *const GetQBKeyHeaderFunc)(const unsigned long QBKey);
		const int addr = ((GetQBKeyHeaderFunc)(0x00426340))(0x3BD7B54E);

		if(addr!=0)
		{
		const float speed = GetSpeed((const Vertex* const __restrict)pSpeed);
		if(speed != 0)
		*(float*)(addr+12) = speed;
		else
		*(float*)(addr+12) = 100.0f;

		}
		else
		{
		QBKeyHeader* header = GetQBKeyHeader(0x3BD7B54E);
		if(header)
		{
		const float speed = GetSpeed((const Vertex* const __restrict)pSpeed);
		if(speed != 0)
		header->fVal = speed;
		else
		header->fVal = 100.0f;

		}
		MessageBox(0, "couldn't find script variable" ," LastSpeed", 0);
		return false;
		}*/
	}
	DEBUGEND();
	CScript::DumpScripts();
	return true;
}

Vertex oldSpeed;
float oldAngle1, oldAngle2;

bool PreWallplantScript(CStruct* pParams, CScript* pScript)
{
	static const DWORD ptr = 0x005D06C0;
	VALIDATE_PTR((void*)ptr);
	DWORD pSpeed = *(DWORD*)ptr + 0x580;
	VALIDATE_PTR((void*)pSpeed);
	pSpeed = *(DWORD*)pSpeed + 0x4;
	VALIDATE_PTR((void*)pSpeed);
	pSpeed = *(DWORD*)pSpeed + 0x2C;
	VALIDATE_PTR((void*)pSpeed);
	pSpeed = *(DWORD*)pSpeed + 0x48;
	VALIDATE_PTR((void*)pSpeed);
	pSpeed = *(DWORD*)pSpeed + 0x334;
	VALIDATE_DATA((Vertex*)pSpeed, sizeof(Vertex));

	oldSpeed = (*(const Vertex* const __restrict)pSpeed);

	static const DWORD ptr2 = 0x008E2498;
	VALIDATE_PTR((void*)ptr2);
	DWORD pPos = *(DWORD*)ptr2 + 0x13C;
	VALIDATE_PTR((void*)pPos);
	pPos = *(DWORD*)pPos + 0x14;
	VALIDATE_PTR((void*)pPos);
	const DWORD old = pPos;
	pPos = *(DWORD*)pPos + 0x4BC;
	if (InvalidReadPtr((void*)pPos))
	{
		pPos = old;
		pPos = *(DWORD*)pPos + 0x3B4;
		VALIDATE_PTR((void*)pPos);
	}
	pPos = *(DWORD*)pPos + 0x18;
	VALIDATE_DATA((Vertex*)pPos, sizeof(Vertex));
	pPos += 0x35C;
	oldAngle1 = *((float*)pPos);
	pPos += 8;
	oldAngle2 = *((float*)pPos);
	return true;
}

bool WallplantScript(CStruct* pParams, CScript* pScript)
{
	DEBUGSTART()
	{
		//ExecuteQBScript("DefaultGapScript", pParams, pScript);
		//ExecuteQBScript("DefaultGapScript", pParams, pScript);
		static const DWORD ptr = 0x005D06C0;
		VALIDATE_PTR((void*)ptr);
		DWORD pSpeed = *(DWORD*)ptr + 0x580;
		VALIDATE_PTR((void*)pSpeed);
		pSpeed = *(DWORD*)pSpeed + 0x4;
		VALIDATE_PTR((void*)pSpeed);
		pSpeed = *(DWORD*)pSpeed + 0x2C;
		VALIDATE_PTR((void*)pSpeed);
		pSpeed = *(DWORD*)pSpeed + 0x48;
		VALIDATE_PTR((void*)pSpeed);
		pSpeed = *(DWORD*)pSpeed + 0x334;
		VALIDATE_DATA((Vertex*)pSpeed, sizeof(Vertex));

		Vertex speed = (*(const Vertex* const __restrict)pSpeed);
		speed = oldSpeed;
		speed.X *= -1.0f;
		//speed.Y = 0.0f;
		speed.Z *= -1.0f;
		(*(Vertex*)pSpeed) = speed;

		/*static const DWORD ptr2 = 0x008E1E90;
		VALIDATE_PTR((void*)ptr2);
		DWORD pAngle = *(DWORD*)ptr2 + 0xC0;
		VALIDATE_PTR((void*)pAngle);
		pAngle = *(DWORD*)pAngle + 0x5C4;
		VALIDATE_PTR((void*)pAngle);
		pAngle = *(DWORD*)pAngle + 0x0C;
		VALIDATE_PTR((void*)pAngle);
		pAngle = *(DWORD*)pAngle + 0x10;
		VALIDATE_PTR((void*)pAngle);
		pAngle = *(DWORD*)pAngle + 0x40;
		VALIDATE_DATA((float*)pAngle, sizeof(float));
		(*(float*)pAngle) *= -1.0f;//float angle = *(float*)pAngle*(180.0f / 3.14159f);
		printf("speedx %f speedz %f anglez %f\n", speed.X, speed.Z, *(float*)pAngle);*/

		void* first = (void*)*(DWORD*)(*(DWORD*)0x008E2498 + 0x13C);
		void* last = (void*)*(DWORD*)(*(DWORD*)0x008E2498 + 0x140);
		void* current = first;
		Vertex* camera = ObserveMode::GetCamera();
		Vertex* pos = NULL;

		if (current && !InvalidReadPtr(current) && *(DWORD*)current == 0x0058E504 && *(DWORD*)((DWORD)current + 4) != 0xFFFFFFFF)
		{
			pos = (Vertex*)(*(DWORD*)((DWORD)current + 20) + 0x18);
			speed = *((Vertex*)(*(DWORD*)((DWORD)current + 20) + 0x18 + 16));
			printf("pos %f oldPos %f", pos->X, speed.X);
			fflush(stdout);
			/*speed.X -= pos->X;
			speed.Z -= pos->Z;*/
			/*char* c = (char*)((DWORD)current + 32);
			DWORD i = 0;
			while (*c != 0)
			{
			name[i] = *c;
			c++;
			i++;
			}
			name[i] = 0;*/
		}
		else
			return false;


		float cameraAngle = atan2f((pos->X - camera->X), (pos->Z - camera->Z));
		cameraAngle *= -1.0f;
		if (camera->X >= pos->X)
			pos->X += 10.0f;
		else
			pos->X -= 10.0f;
		if (camera->Z >= pos->Z)
			pos->Z += 10.0f;
		else
			pos->Z -= 10.0f;

		static const DWORD ptr2 = 0x008E2498;
		VALIDATE_PTR((void*)ptr2);
		DWORD pPos = *(DWORD*)ptr2 + 0x13C;
		VALIDATE_PTR((void*)pPos);
		pPos = *(DWORD*)pPos + 0x14;
		VALIDATE_PTR((void*)pPos);
		const DWORD old = pPos;
		pPos = *(DWORD*)pPos + 0x4BC;
		if (InvalidReadPtr((void*)pPos))
		{
			pPos = old;
			pPos = *(DWORD*)pPos + 0x3B4;
			VALIDATE_PTR((void*)pPos);
		}
		pPos = *(DWORD*)pPos + 0x18;
		VALIDATE_DATA((Vertex*)pPos, sizeof(Vertex));
		pPos += 0x35C;
		//*((float*)pPos) *= -1.0f;
		*((float*)pPos) = oldAngle1;
		pPos += 8;
		//float oldf = *(float*)pPos;
		//*((float*)pPos) = (cameraAngle / 0.5f / 3.14159f) - 1.0f;
		//printf("new %f camAngle%f old %f ", *((float*)pPos), cameraAngle, oldf);
		//*((float*)pPos) *= -1.0f;*/
		*((float*)pPos) = oldAngle2;
		//printf("newnew %f posX %f speedX %f\n", *((float*)pPos), pos->X, speed.X);
	}
	DEBUGEND();
	return true;
}

bool UsingNewMenu(CStruct* pStruct, CScript* pScript)
{
	return LevelModSettings::UseNewMenu;
}
bool UsingSoundFix(CStruct* pStruct, CScript* pScript)
{
	return LevelModSettings::FixSound;
}
bool UsingTeleFix(CStruct* pStruct, CScript* pScript)
{
	return LevelModSettings::TeleFix;
}

BYTE oldLimit = 32;

bool ToggleNewMenu(CStruct* pStruct, CScript* pScript)
{
	DWORD old;
	if (VirtualProtect((LPVOID)0x004404CE, sizeof(LevelModSettings::NewMenu), PAGE_EXECUTE_READWRITE, &old))
	{
		LevelModSettings::UseNewMenu = !LevelModSettings::UseNewMenu;
		if (LevelModSettings::UseNewMenu)
			memcpy((void*)0x004404CE, &LevelModSettings::NewMenu, sizeof(LevelModSettings::NewMenu));
		else
			memcpy((void*)0x004404CE, &LevelModSettings::OldMenu, sizeof(LevelModSettings::OldMenu));
		/*FILE*f = fopen("settings","w+b");
		if(f)
		{
		fwrite(&LevelModSettings::UseNewMenu, 1, 1, f);
		fwrite(&LevelModSettings::FixSound, 1, 1, f);
		fwrite(&LevelModSettings::TeleFix, 1, 1, f);
		fwrite(&oldLimit, 1, 1, f);
		bool disableGrass = false;
		QBKeyHeader* header = GetQBKeyHeader(crc32f((unsigned char*)"GrassLayersDisabled"));
		if(header)
		disableGrass = header->value.i;
		fwrite(&disableGrass,1,1,f);
		fclose(f);
		}*/
		return true;
	}
	else
	{
		MessageBox(0, "couldn't fix protection for", "NewMenu", 0);
		return false;
	}
}

bool CreatePair(CStruct* pStruct, CScript* pScript)
{
	const char* name = "";
	pStruct->GetString("name", &name);
	float x = 0;
	float y = 0;
	CStructHeader* value = pStruct->head;
	while (value)
	{
		if (value->QBkey == Checksum("x"))
		{
			x = value->value.f;
		}
		else if (value->QBkey == Checksum("y"))
			y = value->value.f;
		value = value->NextHeader;
	}

	CStructHeader* param = pScript->params->AddParam((char*)name, QBKeyHeader::PAIR);
	param->pVec = (D3DXVECTOR3*)mallocx(8);
	param->pVec->x = x;
	param->pVec->y = y;
	return true;
}

bool CounterIsOn(CStruct* pStruct, CScript* pScript)
{
	return LevelModSettings::grafCounter;
}


bool ToggleGrafCounter(CStruct* pStruct, CScript* pScript)
{
	DWORD addr = NULL;
	do {
		addr = (DWORD)GetModuleHandle("d3d8.dll");
		Sleep(20);
	} while (!addr);

	DWORD_PTR* vTable = Hook::GetD3D8VTable(addr, 0x0001A4F4);//0x128000);
	if (!vTable || !vTable[15])
		return false;

	addr = (DWORD)vTable[15];

	LevelModSettings::grafCounter = !LevelModSettings::grafCounter;
	if (LevelModSettings::grafCounter)
	{
		BYTE backup[5];
		memcpy(backup, (void*)addr, 5);
		memcpy((void*)addr, PresentHook.backup, 5);
		memcpy(PresentHook.backup, backup, 5);
	}
	else
	{
		BYTE backup[5];
		memcpy(backup, (void*)addr, 5);
		memcpy((void*)addr, PresentHook.backup, 5);
		memcpy(PresentHook.backup, backup, 5);
	}
	return true;
}

bool ToggleTeleFix(CStruct* pStruct, CScript* pScript)
{
	const static DWORD Addr = 0x004AE562;
	const static DWORD Addr2 = 0x004AE581;

	DWORD old;
	if (VirtualProtect((LPVOID)Addr, 4, PAGE_EXECUTE_READWRITE, &old))
	{
		if (VirtualProtect((LPVOID)Addr2, 1, PAGE_EXECUTE_READWRITE, &old))
		{
			LevelModSettings::TeleFix = !LevelModSettings::TeleFix;
			if (LevelModSettings::TeleFix)
			{
				*(DWORD*)Addr = 0x90909090;//84 c0 75 26
				*(BYTE*)Addr2 = 0x75;//74
			}
			else
			{
				*(DWORD*)Addr = 0x2675C084;
				*(BYTE*)Addr2 = 0x74;
			}
			/*FILE*f = fopen("settings","w+b");
			if(f)
			{
			fwrite(&LevelModSettings::UseNewMenu, 1, 1, f);
			fwrite(&LevelModSettings::FixSound, 1, 1, f);
			fwrite(&LevelModSettings::TeleFix, 1, 1, f);
			fwrite(&oldLimit, 1, 1, f);
			bool disableGrass = false;
			QBKeyHeader* header = GetQBKeyHeader(crc32f((unsigned char*)"GrassLayersDisabled"));
			if(header)
			disableGrass = header->value.i;
			fwrite(&disableGrass,1,1,f);
			fclose(f);
			}*/
			return true;
		}
		else
		{
			MessageBox(0, "couldn't fix protection for", "TeleFix", 0);
			return false;
		}
	}
	else
	{
		MessageBox(0, "couldn't fix protection for", "TeleFix", 0);
		return false;
	}
}

bool ToggleSoundFix(CStruct* pStruct, CScript* pScript)
{
	const static DWORD Addr = 0x004C665D;
	DWORD old;
	if (VirtualProtect((void*)Addr, 1, PAGE_EXECUTE_READWRITE, &old))
	{
		LevelModSettings::FixSound = !LevelModSettings::FixSound;

		if (LevelModSettings::FixSound)
			*(BYTE*)Addr = 0xEB;
		else
			*(BYTE*)Addr = 0x75;
		/*FILE*f = fopen("settings","w+b");
		if(f)
		{
		fwrite(&LevelModSettings::UseNewMenu, 1, 1, f);
		fwrite(&LevelModSettings::FixSound, 1, 1, f);
		fwrite(&LevelModSettings::TeleFix, 1, 1, f);
		fwrite(&oldLimit, 1, 1, f);
		bool disableGrass = false;
		QBKeyHeader* header = GetQBKeyHeader(crc32f((unsigned char*)"GrassLayersDisabled"));
		if(header)
		disableGrass = header->value.i;
		fwrite(&disableGrass,1,1,f);
		fclose(f);
		}*/
		return true;
	}
	else
	{
		MessageBox(0, "couldn't fix protection for", "SoundFix", 0);
		return false;
	}
}

bool GetMotd(CStruct* pStruct, CScript* pScript)
{
	/*WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	return false;
	SOCKET fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (fd < 0)
	{
	WSACleanup();
	return false;
	}
	SOCKADDR_IN service;
	service.sin_family = AF_INET;
	service.sin_port = htons(80);
	LPHOSTENT host = gethostbyname("vadru.chapter-3.net");
	if (!host)
	{
	closesocket(fd);
	WSACleanup();
	return false;
	}
	service.sin_addr = *((LPIN_ADDR)*host->h_addr_list);
	if (connect(fd, (SOCKADDR *)&service, sizeof(service)) < 0)
	{
	closesocket(fd);
	WSACleanup();
	return false;
	}
	static const char* GetStr = "GET /LevelModMotd HTTP/1.1\r\n"
	"Host: vadru.chapter-3.net\r\n"
	"User-Agent: Mozilla FireFox/4.0\r\n"
	"User-Agent: Mozilla Firefox/4.0\r\n"
	"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*//*;q=0.8\r\n"
	"Accept-Language: en-gb,en;q=0.5\r\n"
	"Accept-Encoding: gzip,deflate\r\n"
	"Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\n\r\n";

	send(fd, GetStr, strlen(GetStr)+1, 0);
	std::string response = "";
	int resp_leng= 1024;
	char buffer[1024];
	while (resp_leng > 0)
	{
	resp_leng= recv(fd, (char*)&buffer, 1024, 0);
	if (resp_leng>0)
	response += std::string(buffer).substr(0,resp_leng);
	}
	if(strstr(response.c_str(),"404 Not Found"))
	{
	closesocket(fd);
	WSACleanup();
	return false;
	}
	else
	{
	int pos = response.find("\r\n\r\n");
	pos += 4;
	char motd[1024] = "";
	if(sscanf(&response.c_str()[pos], "%s", motd))
	{
	int id = 0;
	pStruct->GetScript("id", &id);
	//SetElementText(18, "hey");
	/*QBKeyHeader* header = GetQBKeyHeader(crc32f((unsigned char*)"motd"));
	if(header)
	{
	if(header->str)
	{
	if(strlen(motd)>strlen(header->str))
	{
	free(header->str);
	header->str = (char*)malloc(strlen(motd)+1);
	strcpy(header->str, motd);
	}
	else
	{
	char* p = motd;
	char* d = header->str;
	while(*p != '\0')
	{
	*d = *p;
	d++;
	p++;
	}
	*d = '\0';
	}
	}
	else
	{
	header->str = (char*)malloc(strlen(motd)+1);
	strcpy(header->str, motd);
	}
	return true;
	}
	else
	{
	header = AddQBKeyHeader(crc32f((unsigned char*)"motd"), 2);
	if(header)
	{
	header->type = QBKeyHeader::STRING;
	header->str = (char*)malloc(strlen(motd)+1);
	strcpy(header->str, motd);
	return true;
	}

	}*/
	/*}
	}*/

	/* response.clear();
	closesocket(fd);
	WSACleanup();*/
	return true;
}
FILE* logFile;
int __cdecl add_log(const char* string, ...)
{
	static char buf[256] = "";
	static CScript* pScript = NULL;
	_asm mov pScript, esi
	/*if ((DWORD)string == 0 || (DWORD)string < 0x00420000 || (DWORD)string > 0x02000000)
	  return -1;
	  //MessageBox(0, string, string, 0);
	  va_list args;
	  va_start(args, string);
	  int k = -1;

	  if (logFile)
	  {
	  fseek(logFile, 0, SEEK_END);
	  k = vfprintf(logFile, string, args);
	  fputc('\n', logFile);
	  fflush(logFile);
	  }
	  va_end(args);
	  return k;*/

	if ((DWORD)string == 0 || (DWORD)string < 0x00420000 || (DWORD)string > 0x02000000 || (strstr(string, "Tried to")))//string[0] == '\n' && string[1] == 'T' && string[2] == 'r'))
		return -1;

	va_list args;
	va_start(args, string);
	int k = -1;
	k = vsprintf(buf, string, args);
	va_end(args);
	char* p = buf;
	if (pScript)
	{
		while (*p != 0x0)
		{
			if (*p == '?' && *(p + 1) == '?' && *(p + 2) == '?' && *(p + 3) == '?' && *(p + 4) == '?')
			{
				sprintf(buf, "%s:%s", buf, lastQB != NULL ? lastQB : FindChecksumName(pScript->scriptChecksum));
				break;
			}
			p++;
		}
	}
	printf(buf);
	printf("\n");
	return k;
}

/*void HookedPrintf(const char* string, ...)
{
va_list args;
va_start(args, string);
add_log(string, args);
}*/

void FixMemoryProtection()
{
	DWORD old;
	VirtualProtect((VOID*)0x0042C18D, 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((VOID*)0x0042C1C7, 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((VOID*)0x0042C1D5, 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((VOID*)0x0042C1E1, 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((VOID*)0x0042C1EC, 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((VOID*)0x00426088, 1, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((VOID*)0x004260E7, 1, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((VOID*)0x004288F7, 1, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((VOID*)0x00428B97, 1, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((VOID*)0x004288F7, 1, PAGE_EXECUTE_READWRITE, &old);
}

/*void LowMemoryMode()
{
*(DWORD*)0x0042C18D = 0x000FA000;
*(DWORD*)0x0042C1C7 = 0x000F9FF0;
*(DWORD*)0x0042C1D5 = 0x000F9FF0;
*(DWORD*)0x0042C1E1 = 0x000F9FF8;
*(DWORD*)0x0042C1EC = 0x000F9FFC;
*(BYTE*)0x00426088 = 0x75;
*(BYTE*)0x004260E7 = 0x75;
*(BYTE*)0x004288F7 = 0x75;
*(BYTE*)0x00428B97 = 0x75;
*(BYTE*)0x004288F7 = 0x75;
}*/

void NormalMemoryMode()
{
	/**(DWORD*)0x0042C18D = 0x000FA000;
	*(DWORD*)0x0042C1C7 = 0x000F9FF0;
	*(DWORD*)0x0042C1D5 = 0x000F9FF0;
	*(DWORD*)0x0042C1E1 = 0x000F9FF8;
	*(DWORD*)0x0042C1EC = 0x000F9FFC;
	*(BYTE*)0x00426088 = 0x74;
	*(BYTE*)0x004260E7 = 0x74;
	*(BYTE*)0x004288F7 = 0x74;
	*(BYTE*)0x00428B97 = 0x74;
	*(BYTE*)0x004288F7 = 0x74;*/

	/*00428B97

	  0042C280*/
}


void FixSpineFlag()
{
	DWORD old;
	VirtualProtect((void*)0x004A2DAA, 9, PAGE_EXECUTE_READWRITE, &old);
	//*(BYTE*)0x004A2DAA = 0xE9;
	DWORD addr = 0x004A2DAA;
	*(BYTE*)addr = 0xE9;
	addr++;
	DWORD offset = (PtrToUlong(CheckForTransfer_naked) - 0x004A2DAA) - 5;
	*(DWORD*)addr = offset;
	addr += 4;
	*(DWORD*)addr = 0x3F74C084;


	VirtualProtect((void*)0x004A25D8, 5, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)0x004A25D8 = 0xE9;
	addr = 0x004A25D9;
	offset = (PtrToUlong(Slerp_naked) - 0x004A25D8) - 5;
	*(DWORD*)addr = offset;

	VirtualProtect((void*)0x004A3096, 5, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)0x004A3096 = 0xE9;
	addr = 0x004A3097;
	offset = (PtrToUlong(ResetTransfer_naked) - 0x004A3096) - 5;
	*(DWORD*)addr = offset;

	VirtualProtect((void*)0x004A27B4, 5, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)0x004A27B4 = 0xE9;
	addr = 0x004A27B5;
	offset = (PtrToUlong(ResetTransferVel_naked) - 0x004A27B4) - 5;
	*(DWORD*)addr = offset;



	VirtualProtect((void*)0x004A21AE, 5, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)0x004A21AE = 0xE9;
	addr = 0x004A21AF;
	offset = (PtrToUlong(OnGround_naked) - 0x004A21AE) - 5;
	*(DWORD*)addr = offset;


	VirtualProtect((void*)0x004294C6, 5, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)0x0004294C6 = 0xE8;
	addr = 0x004294C7;


	/*Hooked_AddCStruct = (Hooked_AddCStruct)CStruct::MaybeAddCompressed;*/
	/*typedef BYTE* (__thiscall* CStruct::* pFunc)(DWORD, BYTE*, QBKeyInfoContainer*);
	pFunc p = (pFunc)&CStruct::MaybeAddCompressed;*/


	//BYTE* (__thiscall CStruct:: * pFunc)(DWORD, BYTE*, QBKeyInfoContainer*) = &CStruct::MaybeAddCompressed; <-this
	//void (MyClass:: * fptr) (void) = &MyClass::fun;


	//sprintf(msg, "%X %X", &pFunc, pFunc);
	//*(DWORD*)addr = (DWORD)(void*&)pFunc - 0x4294CB; <-this
	//MessageBox(0, msg, msg, 0);


	VirtualProtect((void*)0x004A8ADF, 5, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)0x004A8ADF = 0xE9;
	addr = 0x004A8AE0;
	offset = (PtrToUlong(MaybeAcid_naked) - 0x004A8ADF) - 5;
	*(DWORD*)addr = offset;


	/*VirtualProtect((void*)0x00403149, 5, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)0x00403149 = 0xE9;
	addr = 0x0040314A;
	offset = (PtrToUlong(Fopen_naked) - 0x00403149) - 5;
	*(DWORD*)addr = offset;*/
	if (debugMode)
	{
		DWORD fopen = *(DWORD*)0x0058D0B0;
		VirtualProtect((void*)fopen, 5, PAGE_EXECUTE_READWRITE, &old);
		*(BYTE*)fopen = 0xE9;
		addr = fopen + 1;
		offset = (PtrToUlong(Fopen_naked) - fopen) - 5;
		*(DWORD*)addr = offset;
	}
	/*VirtualProtect((void*)0x0058D0B0, 4, PAGE_EXECUTE_READWRITE, &old);
	 = (DWORD)_fopen;*/


	if (debugMode)
	{
		VirtualProtect((void*)0x004265F0, 5, PAGE_EXECUTE_READWRITE, &old);
		*(BYTE*)0x004265F0 = 0xE9;
		addr = 0x004265F1;
		offset = (PtrToUlong(Checksum_naked) - 0x004265F0) - 5;
	}
	char msg[128] = "";
	/*sprintf(msg, "OFFSET %X", offset);
	MessageBox(0, msg, msg, 0);*/
	*(DWORD*)addr = offset;




}

void FixQBParsing()
{
	DWORD old;
	VirtualProtect((void*)0x0042B2CC, 1, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)0x0042B2CC = 8;
	VirtualProtect((void*)0x0042B2D4, 5, PAGE_EXECUTE_READWRITE, &old);
	*(DWORD*)0x0042B2D4 = 0x04408BC3;
	*(BYTE*)0x0042B2D8 = 0xC3;
}

bool hooked = false;
static BYTE oldCustomPrint[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static BYTE oldPrint[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

static bool cr = false;

#undef CreateConsole
BOOL CreateConsole()
{
	if (cr)
		return FALSE;
	cr = true;
	int hConHandle = 0;
	HANDLE lStdHandle = 0;
	FILE* fp = 0;

	BOOL ret = AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	freopen("CONERR$", "w", stderr);
	/*lStdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	hConHandle = _open_osfhandle(PtrToUlong(lStdHandle), _O_TEXT);
	fp = _fdopen(hConHandle, "w");
	*stdout = *fp;
	setvbuf(stdout, NULL, _IONBF, 0);

	lStdHandle = GetStdHandle(STD_INPUT_HANDLE);
	hConHandle = _open_osfhandle(PtrToUlong(lStdHandle), _O_TEXT);
	fp = _fdopen(hConHandle, "r");
	*stdin = *fp;
	setvbuf(stdin, NULL, _IONBF, 0);

	lStdHandle = GetStdHandle(STD_ERROR_HANDLE);
	hConHandle = _open_osfhandle(PtrToUlong(lStdHandle), _O_TEXT);
	fp = _fdopen(hConHandle, "w");
	*stderr = *fp;
	setvbuf(stderr, NULL, _IONBF, 0);*/
	return ret;

}

bool UnhookDebugMessages(CStruct* pStruct, CScript* pScript)
{
	if (hooked)
	{
		hooked = false;
		memcpy((void*)0x00401960, oldCustomPrint, 6);

		static const DWORD addr = (DWORD)GetProcAddress(GetModuleHandle("msvcrt.dll"), "printf");
		if (addr)
		{
			memcpy((void*)addr, oldCustomPrint, 6);
		}
		fclose(logFile);
	}
	return true;
}

bool CreateConsole(CStruct* pStruct, CScript* pScript)
{
	return CreateConsole() == TRUE;
}
bool HideConsole(CStruct* pStruct, CScript* pScript)
{
	return FreeConsole() == TRUE;
}

bool HookDebugMessages(CStruct* pStruct, CScript* pScript)
{
	if (!hooked)
	{
		hooked = true;
		DWORD old;
		static BYTE callHooked[] = { 0xE9, 0x00, 0x00, 0x00, 0x00, 0xC3 };

		VirtualProtect((void*)0x00401960, 6, PAGE_EXECUTE_READWRITE, &old);
		memcpy(oldCustomPrint, (void*)0x00401960, 6);

		*(DWORD*)&callHooked[1] = ((DWORD)add_log - 0x00401960 - 5);

		memcpy((void*)0x00401960, callHooked, 6);


		static const DWORD addr = (DWORD)GetProcAddress(GetModuleHandle("msvcrt.dll"), "printf");
		if (addr)
		{
			VirtualProtect((void*)addr, 6, PAGE_EXECUTE_READWRITE, &old);
			memcpy(oldCustomPrint, (void*)addr, 6);
			DWORD hookedAddrs = ((DWORD)add_log - addr - 5);
			*(DWORD*)&callHooked[1] = hookedAddrs;
			memcpy((void*)addr, callHooked, 6);
		}
		//logFile = fopen("loggers.txt", "w+t");
	}
	return true;
}
bool hookedFopen = false;

DWORD HookGeneralFunction(const char* Dll, const char* FuncName, void* Function, unsigned char* backup, int pa)
{
	static BYTE caveFix[6];
	DWORD addr = (DWORD)GetProcAddress(GetModuleHandle(Dll), FuncName);
	if (addr == 0)
	{
		MessageBox(0, "fail", "fail", MB_OK);
		addr = pa - 3;
	}
	BYTE jmp[6] = { 0xe9,			//jmp
		0x00, 0x00, 0x00, 0x00,		//address
		0xc3 };						//retn

	//memcpy(backup, (void*)addr, 6);
	ReadProcessMemory((HANDLE)-1, (void*)addr, backup, 6, 0);
	DWORD calc = ((DWORD)Function - addr - 5);
	memcpy(&jmp[1], &calc, 4);
	//memcpy((void*)addr, &jmp, 6);
	WriteProcessMemory((HANDLE)-1, (void*)addr, jmp, 6, 0);

	return addr;
}



char* strduplow(char* str) {
	char* p;

	str = _strdup(str);
	for (p = str; *p; p++) {
		*p = tolower(*p);
	}
	return(str);
}
DWORD fopenAddr;
BYTE backupFOP[6];
FILE* __cdecl hookfopen(const char* filename, const char* mode)
{
	/*MessageBox(0, filename, mode, MB_OK);
	//char* filename2 = strduplow((char*)filename);
	if (strstr(filename, ".qb"))
		MessageBox(0, "QB FOUND", filename, MB_OK);
	//add_log("fopen(%s,%s)", filename, mode);*/
	WriteProcessMemory((HANDLE)-1, (void*)fopenAddr, backupFOP, 6, 0);
	//MessageBox(0, "REdone", "", MB_OK);
	FILE* ret = fopen(filename, mode);
	fopenAddr = HookGeneralFunction("msvcrt.dll", "fopen", hookfopen, backupFOP, 0);
	return ret;
}
bool HookFopen()
{
	if (!hookedFopen)
	{
		hookedFopen = true;
		fopenAddr = HookGeneralFunction("msvcrt.dll", "fopen", hookfopen, backupFOP, 0);
	}
	char msg[256] = "";
	sprintf(msg, "fopenaddr %x", fopenAddr);
	//MessageBox(0, msg, msg, MB_OK);
	return fopenAddr != 0;
}

#define TIME_TIMED 5000
#define TIME_STEP 5
bool(*IsHosting)(CStruct*, CScript*) = NULL;

struct Message
{
	SOCKET socket;
	const char* data;
	DWORD length;
	IN_ADDR ip;

	Message(SOCKET socket, const char* data, DWORD lenght, IN_ADDR ip)
	{
		this->socket = socket;
		this->data = data;
		this->length = lenght;
		this->ip = ip;
	}
};

void SendMessage(Message* msg)
{
	printf("sending msg!!\n");
	fflush(stdout);
	DWORD timeSpent = 0;
	while (timeSpent <= TIME_TIMED)
	{
		if (send(msg->socket, msg->data, msg->length, 0) >= 0)
			break;
		timeSpent += TIME_STEP;
		Sleep(TIME_STEP);
	}
	printf("disconnecting client: %d.%d.%d.%d!!\n", ((BYTE*)&msg->ip)[0], ((BYTE*)&msg->ip)[1], ((BYTE*)&msg->ip)[2], ((BYTE*)&msg->ip)[3]);
	fflush(stdout);
	closesocket(msg->socket);
}


struct StructScript
{
	CStruct* pStruct;
	CScript* pScript;
};

DWORD GetServerAddress()
{
	printf("getting server address!!\n");
	fflush(stdout);
	/*DWORD addr=0x05D0968;
	addr = *(DWORD*)addr+0xB8;
	addr = *(DWORD*)addr+0x10;
	addr = *(DWORD*)addr+0x4;
	addr = *(DWORD*)addr+0x1C;
	return *(DWORD*)addr+0x1F04;*/
	DWORD addr = 0x008E2498;
	addr = *(DWORD*)addr + 0x178;
	addr = *(DWORD*)addr + 0x44;

	return *(DWORD*)addr;
}

void StartedGraf(StructScript* pStructScript)
{
	if (!IsHosting)//Initialize game function pointer
	{
		//MessageBox(0,"IsHosting==NULL","",0);
		QBKeyHeader* header = GetQBKeyHeader(Checksums::OnServer);
		if (header)
			IsHosting = header->pFunction;
	}
	if (IsHosting)//has pointer to game fucntion?
	{
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2, 2), &wsaData);
		SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (serverSocket == -1)//error couldn't create valid socket
		{
			printf("socket failure\n");
			if (!IsHosting(pStructScript->pStruct, pStructScript->pScript))//not hosting, so limit graf for now
			{
				if (oldLimit != 32)
				{
					printf("temporarly limiting graf\n");//MessageBox(0,"limiting graf","",0);
					fflush(stdout);
					CStruct pStruct;//= new CStruct;
					CStructHeader head;
					pStruct.head = &head;//new CStructHeader;
					pStruct.tail = pStruct.head;
					pStruct.head->Data = 32;
					pStruct.head->Type = QBKeyHeader::QBKeyType::INT;
					pStruct.head->QBkey = 0;
					SetTagLimit(&pStruct, NULL);
					//delete pStruct.head;
				}
			}
			WSACleanup();
			return;
		}
		/*DWORD addr=0x05D0968;
		addr = *(DWORD*)addr+0xB8;
		addr = *(DWORD*)addr+0x10;
		addr = *(DWORD*)addr+0x4;
		addr = *(DWORD*)addr+0x1C;

		sockaddr_in server;
		server.sin_addr.s_addr =  *(DWORD*)addr+0x1F04;
		server.sin_family = AF_INET;
		server.sin_port = htons( 6500 );*/
		if (IsHosting(pStructScript->pStruct, pStructScript->pScript))//host trying to send info that graf is unlimited
		{
			printf("hosting!!\n");
			fflush(stdout);
			SOCKADDR_IN sin;
			sin.sin_family = AF_INET;
			sin.sin_port = htons(6500);
			sin.sin_addr.s_addr = INADDR_ANY;
			if (bind(serverSocket, (LPSOCKADDR)&sin, sizeof(sin)) < 0)
			{
				printf("bind error: ");
				fflush(stdout);
				char error[256];
				sprintf(error, "%d\n", WSAGetLastError());
				printf(error);
				fflush(stdout);
				closesocket(serverSocket);
				WSACleanup();
				return;
			}

			if (listen(serverSocket, 8) < 0)
			{
				printf("listen error: ");
				fflush(stdout);
				char error[256];
				sprintf(error, "%d\n", WSAGetLastError());
				printf(error);
				fflush(stdout);
				closesocket(serverSocket);
				WSACleanup();
				return;
			}
			printf("opening connection!!\n");
			fflush(stdout);
			FD_SET fdSet;
			//shutdown(serverSocket, SD_RECEIVE);
			if (oldLimit == 32)
			{
				printf("setting taglimit to 200");
				fflush(stdout);
				CStruct pStruct;//= new CStruct;
				CStructHeader head;
				pStruct.head = &head;//new CStructHeader;
				pStruct.tail = pStruct.head;
				pStruct.head->Data = 200;
				pStruct.head->Type = QBKeyHeader::QBKeyType::INT;
				pStruct.head->QBkey = 0;
				SetTagLimit(&pStruct, NULL);
			}
			//MessageBox(0,"your hosting","",0);
			DWORD timeSpent = 0;
			timeval timer;
			timer.tv_sec = 5;
			timer.tv_usec = 0;
			for (DWORD i = 0; i < 8; i++)//while(timeSpent<=TIME_TIMED)
			{
				printf("checking for connections %u/8\n", i + 1);
				fflush(stdout);
				FD_ZERO(&fdSet);
				FD_SET(serverSocket, &fdSet);
				if (select(0, &fdSet, NULL, NULL, &timer) == SOCKET_ERROR)
				{
					char error[256];
					sprintf(error, "%d\n", WSAGetLastError());
					printf("SelectError: ");
					printf(error);
					fflush(stdout);
				}
				if (FD_ISSET(serverSocket, &fdSet))
				{
					printf("trying to accept connection\n");
					fflush(stdout);
					sockaddr_in clientInfo;
					SOCKET connectionSocket = ::accept(serverSocket, (sockaddr*)&clientInfo, NULL);
					if (connectionSocket != INVALID_SOCKET)
					{
						printf("connected to client: %d.%d.%d.%d!!\n", ((BYTE*)&clientInfo.sin_addr)[0], ((BYTE*)&clientInfo.sin_addr)[1], ((BYTE*)&clientInfo.sin_addr)[2], ((BYTE*)&clientInfo.sin_addr)[3]);
						Message msg(connectionSocket, (const char*)&LevelModSettings::UnlimitedGraf, 1, clientInfo.sin_addr);
						HANDLE hRequestThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SendMessage, (LPVOID)&msg, 0/*CREATE_SUSPENDED*/, NULL);
					}
					else
					{
						char error[256];
						sprintf(error, "%d\n", WSAGetLastError());
						printf("Error: ");
						printf(error);
						fflush(stdout);
					}
				}
				/*timeSpent+=TIME_STEP;
				Sleep(TIME_STEP);*/
			}
			Sleep(TIME_TIMED);
		}
		else//client trying to get info from host, if no info received graf will remain limited
		{
			printf("client!!\n");
			fflush(stdout);
			//shutdown(serverSocket, SD_SEND);
			//MessageBox(0,"your client","",0);
			sockaddr_in server;
			DWORD address = GetServerAddress();
			printf("server address: %d.%d.%d.%d\n", ((BYTE*)&address)[0], ((BYTE*)&address)[1], ((BYTE*)&address)[2], ((BYTE*)&address)[3]);
			fflush(stdout);
			server.sin_addr.s_addr = address;
			server.sin_family = AF_INET;
			server.sin_port = htons(6500);
			bool unlimitedGraf = false;
			DWORD timeSpent = 0;
			for (DWORD i = 0; i < 2; i++)
			{
				int err = 0;
				printf("trying to connect\n");
				fflush(stdout);
				if (err = connect(serverSocket, (sockaddr*)&server, sizeof(sockaddr)) >= 0)
				{
					printf("connected!!\n");
					fflush(stdout);
					timeSpent = 0;
					while (timeSpent <= TIME_TIMED)
					{
						if (recv(serverSocket, (char*)&unlimitedGraf, 1, 0) >= 0)
						{
							printf("recieved!!\n");
							fflush(stdout);
							//shutdown(serverSocket, SD_RECEIVE);
							break;
						}
						timeSpent += TIME_STEP;
						Sleep(TIME_STEP);
					}
					break;
				}
				else
				{
					char error[256];
					sprintf(error, "%d\n", WSAGetLastError());
					printf("Error: ");
					printf(error);
					fflush(stdout);
				}
				/*timeSpent+=TIME_STEP;
				Sleep(TIME_STEP);*/
			}
			if (unlimitedGraf)
			{
				printf("unlimiting graf!!\n");
				fflush(stdout);
				if (oldLimit == 32)
				{
					CStruct pStruct;//= new CStruct;
					CStructHeader head;
					pStruct.head = &head;//new CStructHeader;
					pStruct.tail = pStruct.head;
					pStruct.head->Data = 200;
					pStruct.head->Type = QBKeyHeader::QBKeyType::INT;
					pStruct.head->QBkey = 0;
					SetTagLimit(&pStruct, NULL);
				}
			}
			else if (oldLimit != 32)
			{
				printf("limiting graf!!\n");
				fflush(stdout);
				//MessageBox(0,"limiting graf","",0);
				CStruct pStruct;//= new CStruct;
				CStructHeader head;
				pStruct.head = &head;//new CStructHeader;
				pStruct.tail = pStruct.head;
				pStruct.head->Data = 32;
				pStruct.head->Type = QBKeyHeader::QBKeyType::INT;
				pStruct.head->QBkey = 0;
				SetTagLimit(&pStruct, NULL);
				//delete pStruct.head;
			}
			else
			{
				printf("graf already limited!!\n");
				fflush(stdout);
			}
		}
		printf("closing connection!!\n");
		fflush(stdout);
		closesocket(serverSocket);
		WSACleanup();
	}
	else if (oldLimit != 32)//couldn't initialize game function pointer, so will need to limit graf....
	{
		printf("NO PTR TO OnServer!!\n");
		CStruct pStruct;//= new CStruct;
		CStructHeader head;
		pStruct.head = &head;//new CStructHeader;
		pStruct.tail = pStruct.head;
		pStruct.head->Data = 32;
		pStruct.head->Type = QBKeyHeader::QBKeyType::INT;
		pStruct.head->QBkey = 0;
		SetTagLimit(&pStruct, NULL);
		//delete pStruct.head;
	}
}

bool GrafStarted(CStruct* pStruct, CScript* pScript)
{
	DEBUGSTART()
	{
		StructScript structScript;
		structScript.pStruct = pStruct;
		structScript.pScript = pScript;
		if (LevelModSettings::UnlimitedGraf)
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)StartedGraf, &structScript, 0, 0);
		//MessageBox(0,"done","done",0);
	}
	DEBUGEND();
	return true;
}

bool Kill3DGrass(CStruct* params, CScript* pScript)
{
	DEBUGSTART()
	{
		CStructHeader* param = params->AddParam("Name", QBKeyHeader::LOCAL);

		char name[256] = "";

		for (DWORD i = 0; i < 1000; i++)
		{
			sprintf(name, "3DGrassMesh%u", i);
			param->SetChecksum(name);
			ExecuteQBScript("KillIfAlive", params);
		}
		params->DellocateCStruct();
	}
	DEBUGEND();
	return true;
}

bool UpdateLevelModSettings(CStruct* params, CScript* pScript)
{
	FILE* f = fopen("settings", "w+b");
	if (f)
	{
		printf("going to save settings\n");
		fwrite(&LevelModSettings::UseNewMenu, 1, 1, f);
		fwrite(&LevelModSettings::FixSound, 1, 1, f);
		fwrite(&LevelModSettings::TeleFix, 1, 1, f);
		fwrite(&oldLimit, 1, 1, f);
		bool disableGrass = false;
		QBKeyHeader* header = GetQBKeyHeader(Checksum("GrassLayersDisabled"));
		if (header)
			disableGrass = header->value.i != 0;
		fwrite(&disableGrass, 1, 1, f);
		bool slapOn = true;
		header = GetQBKeyHeader(Checksum("SlapIsOn"));
		if (header)
			slapOn = header->value.i != 0;
		fwrite(&slapOn, 1, 1, f);
		bool hudOn = true;
		header = GetQBKeyHeader(Checksum("HudIsOn"));
		if (header)
			hudOn = header->value.i != 0;
		fwrite(&hudOn, 1, 1, f);
		fwrite(&version, 4, 1, f);

		/*bool b = false;
		header = GetQBKeyHeader(Checksum("bShowConsole"));
		if (header)
			b = header->value.i != 0;
		fwrite(&b, 1, 1, f);
		header = GetQBKeyHeader(Checksum("bPrintDebug"));
		if (header)
			b = header->value.i != 0;
		fwrite(&b, 1, 1, f);*/
		fwrite(&debugMode, 1, 1, f);
		header = GetQBKeyHeader(Checksum("spine_button_text"));
		if (header)
		{
			if (strstr(header->pStr, "Revert"))
			{
				SpineButton = KeyState::REVERT;
			}
			else if (strstr(header->pStr, "Nollie"))
			{
				SpineButton = KeyState::NOLLIE;
			}
			else if (strstr(header->pStr, "Left"))
			{
				SpineButton = KeyState::SPINLEFT;
			}
			else if (strstr(header->pStr, "Right"))
			{
				SpineButton = KeyState::SPINRIGHT;
			}
			fwrite(&SpineButton, 1, 1, f);
		}
		fclose(f);
	}
	ExecuteQBScript("LoadLevelModSettings", params);
	return true;
}

bool ToggleHookDebugMessages(CStruct* pStruct, CScript* pScript)
{
	static const DWORD addr = (DWORD)GetProcAddress(GetModuleHandle("msvcrt.dll"), "printf");
	hooked = !hooked;
	if (hooked)
	{
		logFile = fopen("loggers.txt", "w+t");
		DWORD old;
		static BYTE callHooked[] = { 0xE9, 0x00, 0x00, 0x00, 0x00, 0xC3 };

		VirtualProtect((void*)0x00401960, 6, PAGE_EXECUTE_READWRITE, &old);
		memcpy(oldCustomPrint, (void*)0x00401960, 6);

		*(DWORD*)&callHooked[1] = ((DWORD)add_log - 0x00401960 - 5);

		memcpy((void*)0x00401960, callHooked, 6);

		if (addr)
		{
			VirtualProtect((void*)addr, 6, PAGE_EXECUTE_READWRITE, &old);
			memcpy(oldCustomPrint, (void*)addr, 6);
			DWORD hookedAddrs = ((DWORD)add_log - addr - 5);
			*(DWORD*)&callHooked[1] = hookedAddrs;
			memcpy((void*)addr, callHooked, 6);
		}
	}
	else
	{
		fclose(logFile);
		memcpy((void*)0x00401960, oldCustomPrint, 6);

		if (addr)
		{
			memcpy((void*)addr, oldCustomPrint, 6);
		}
	}
	return true;
}

void FixTagLimitProtections()
{
	DWORD old;
	VirtualProtect((void*)0x004359B7, 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x004359E1, 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x00435A07, 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x00435A3F, 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x00435A51, 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x00435A85, 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x00435A9D, 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x00435AB2, 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x00435AC2, 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x00435AD2, 4, PAGE_EXECUTE_READWRITE, &old);


	VirtualProtect((void*)0x00435AF7, 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x00435B21, 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x00435B4A, 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x00435B51, 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x00435B5A, 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x00435B7E, 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x00435B8C, 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x00435B9B, 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x00435BBA, 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x00435BD3, 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x00435BE8, 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x00435C29, 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x00435C6B, 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x00435C90, 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x00435C97, 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x00435CBA, 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x00435CFF, 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x00435D2D, 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x00435D50, 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x00435DAC, 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x00435E07, 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x00435E1C, 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x00435E2D, 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x00435E38, 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x00435E67, 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x00435E7C, 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x00435E8C, 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x00435E9D, 4, PAGE_EXECUTE_READWRITE, &old);

	VirtualProtect((void*)0x0043BA97, 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x0043BABC, 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x0043BB07, 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x0043BB27, 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x0043BBED, 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x0043BC0A, 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x0043BC1A, 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x0043BC7F, 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x0043BC94, 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x0043BCA4, 4, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x0043BCB4, 4, PAGE_EXECUTE_READWRITE, &old);

	VirtualProtect((void*)0x004BFCD3, 6, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x004BFD94, 6, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x004BFCEF, 12, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x0058C300, 12, PAGE_EXECUTE_READWRITE, &old);


	*(DWORD*)0x004BFCD3 = 0x0F39A2E9;
	*(WORD*)0x004BFCD7 = 0x9000;
	*(DWORD*)0x004BFCEF = 0xE9909090;
	*(DWORD*)0x004BFCF3 = 0x000CC615;
	*(DWORD*)0x004BFCF7 = 0x90909090;
	*(DWORD*)0x004BFD94 = 0x0F38F3E9;
	*(WORD*)0x004BFD98 = 0x9000;

	BYTE GrafFix1[] = { 0x85, 0xF6, 0x0F, 0x86, 0x63, 0xC6, 0xF0, 0xFF, 0xBA, 0x3C, 0x03, 0x40, 0x00, 0xE9, 0x4D, 0xC6, 0xF0, 0xFF, 0x89, 0x44, 0x24, 0x10, 0x0F, 0x86, 0x47, 0xC7, 0xF0, 0xFF, 0xBE, 0x3C, 0x03, 0x40, 0x00, 0xE9, 0xFA, 0xC6, 0xF0, 0xFF };

	VirtualProtect((void*)0x005B367A, sizeof(GrafFix1), PAGE_EXECUTE_READWRITE, &old);
	memcpy((void*)0x005B367A, GrafFix1, sizeof(GrafFix1));

	BYTE GrafFix2[] = { 0x89, 0x3D, 0x00, 0xC3, 0x58, 0x00, 0xBF, 0x3C, 0x03, 0x40, 0x00, 0x89, 0x04, 0x97, 0x8B, 0x3D, 0x00, 0xC3, 0x58, 0x00, 0x83, 0xBF, 0x80, 0x00, 0x00, 0x00, 0x00, 0x75, 0x0A, 0xC7, 0x05, 0x06, 0xC3, 0x58, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x05, 0x06, 0xC3, 0x58, 0x00, 0x83, 0xE2, 0x1F, 0x89, 0x04, 0x97, 0x8B, 0x87, 0x80, 0x00, 0x00, 0x00, 0xE9, 0xB1, 0x39, 0xF3, 0xFF };

	VirtualProtect((void*)0x0058C30C, sizeof(GrafFix2), PAGE_EXECUTE_READWRITE, &old);
	memcpy((void*)0x0058C30C, GrafFix2, sizeof(GrafFix2));

	VirtualProtect((void*)0x004BFD66, 1, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x004BFD6A, 1, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x004BFC85, 1, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x004BFD89, 1, PAGE_EXECUTE_READWRITE, &old);

	*(BYTE*)0x004BFC85 = 200;
	*(BYTE*)0x004BFC89 = 200;
}

bool SetTagLimit(CStruct* pStruct, CScript* pScript)
{
	DEBUGSTART()
	{
		//MessageBox(0,"inside limit", "", 0);
		int tagLimit = 0;
		/*if(pStruct==NULL)
		tagLimit=32;
		else
		{*/
		if (!pStruct->GetInt(&tagLimit) || tagLimit == 0 || tagLimit > 0xFF)
			return oldLimit == 32;
		//}
		oldLimit = (BYTE)tagLimit;
		BYTE limit = oldLimit;//Limit the counters
		*(BYTE*)0x004BFD66 = limit;
		*(BYTE*)0x004BFD6A = limit;
		/**(BYTE*)0x004BFC85 = limit;
		*(BYTE*)0x004BFC89 = limit;*/
		LevelModSettings::UnlimitedGraf = (limit != 32);//Set the new settings




		int numExtra = (tagLimit - 32) * 4;//Increase Stack Size by  4*(extra number of tags limited)
		*(DWORD*)0x004359B7 = 0xA0 + numExtra;
		*(DWORD*)0x004359E1 = 0xB8 + numExtra;
		*(DWORD*)0x00435A07 = 0xBC + numExtra;
		*(DWORD*)0x00435A3F = 0xB4 + numExtra;
		*(DWORD*)0x00435A51 = 0x80 + numExtra;
		*(DWORD*)0x00435A85 = 0x80 + numExtra;
		*(DWORD*)0x00435A9D = 0xB4 + numExtra;
		*(DWORD*)0x00435AB2 = 0xB4 + numExtra;
		*(DWORD*)0x00435AC2 = 0xAC + numExtra;
		*(DWORD*)0x00435AD2 = 0xAC + numExtra;


		*(DWORD*)0x00435AF7 = 0x158 + numExtra;
		*(DWORD*)0x00435B21 = 0x178 + numExtra;
		*(DWORD*)0x00435B4A = 0x188 + numExtra;
		*(DWORD*)0x00435B51 = 0x178 + numExtra;
		*(DWORD*)0x00435B5A = 0x170 + numExtra;
		*(DWORD*)0x00435B7E = 0x17C + numExtra;
		*(DWORD*)0x00435B8C = 0x184 + numExtra;
		*(DWORD*)0x00435B9B = 0x188 + numExtra;
		*(DWORD*)0x00435BBA = 0x180 + numExtra;
		*(DWORD*)0x00435BD3 = 0x170 + numExtra;
		*(DWORD*)0x00435BE8 = 0x170 + numExtra;
		*(DWORD*)0x00435C29 = 0x80 + numExtra;
		*(DWORD*)0x00435C6B = 0x80 + numExtra;
		*(DWORD*)0x00435C90 = 0x17C + numExtra;
		*(DWORD*)0x00435C97 = 0x180 + numExtra;
		*(DWORD*)0x00435CBA = 0x184 + numExtra;
		*(DWORD*)0x00435CFF = 0x80 + numExtra;
		*(DWORD*)0x00435D2D = 0x180 + numExtra;
		*(DWORD*)0x00435D50 = 0x184 + numExtra;
		*(DWORD*)0x00435DAC = 0x80 + numExtra;
		*(DWORD*)0x00435E07 = 0x170 + numExtra;
		*(DWORD*)0x00435E1C = 0x170 + numExtra;
		*(DWORD*)0x00435E2D = 0x180 + numExtra;
		*(DWORD*)0x00435E38 = 0x184 + numExtra;
		*(DWORD*)0x00435E67 = 0x170 + numExtra;
		*(DWORD*)0x00435E7C = 0x170 + numExtra;
		*(DWORD*)0x00435E8C = 0x168 + numExtra;
		*(DWORD*)0x00435E9D = 0x164 + numExtra;

		*(DWORD*)0x0043BA97 = 0x134 + numExtra;
		*(DWORD*)0x0043BABC = 0x150 + numExtra;
		*(DWORD*)0x0043BB07 = 0x150 + numExtra;
		*(DWORD*)0x0043BB27 = 0x154 + numExtra;
		*(DWORD*)0x0043BBED = 0x154 + numExtra;
		*(DWORD*)0x0043BC0A = 0x80 + numExtra;
		*(DWORD*)0x0043BC1A = 0x80 + numExtra;
		*(DWORD*)0x0043BC7F = 0x148 + numExtra;
		*(DWORD*)0x0043BC94 = 0x148 + numExtra;
		*(DWORD*)0x0043BCA4 = 0x140 + numExtra;
		*(DWORD*)0x0043BCB4 = 0x140 + numExtra;
		/*FILE*f = fopen("settings","w+b");
		if(f)
		{
		fwrite(&LevelModSettings::UseNewMenu, 1, 1, f);
		fwrite(&LevelModSettings::FixSound, 1, 1, f);
		fwrite(&LevelModSettings::TeleFix, 1, 1, f);
		fwrite(&oldLimit, 1, 1, f);
		bool disableGrass = false;
		QBKeyHeader* header = GetQBKeyHeader(crc32f((unsigned char*)"GrassLayersDisabled"));
		if(header)
		disableGrass = header->value.i;
		fwrite(&disableGrass,1,1,f);
		fclose(f);
		}*/
	}
	DEBUGEND();
	return true;
}

bool AddToGlobal(CStruct* pStruct, CScript* pScript)
{
	DEBUGSTART()
	{
		CStructHeader* header = pStruct->head;
		int qbKey = 0;
		int value = 0;
		while (header)
		{
			if (header->QBkey == 0xA1DC81F9)
			{
				qbKey = header->GetInt();
				if (value)
					break;
			}
			if (header->QBkey == 0xE288A7CB)
			{
				value = header->GetInt();
				if (qbKey)
					break;
			}
			header = header->NextHeader;
		}
		int oldValue;
		header = pScript->params->head;
		while (header)
		{
			if (header->QBkey == qbKey)
			{
				oldValue = header->GetInt();
				break;
			}
			header = header->NextHeader;
		}
		header = pScript->params->head;
		while (header)
		{
			if (header->QBkey == 0x34406F14)
			{
				header->value.i = oldValue + value;
				return true;
			}
			header = header->NextHeader;
		}
		CStruct* const __restrict params = pScript->params;
		CStructHeader* const __restrict pHeader = params->AllocateCStruct();
		if (!pHeader)
			return false;
		params->tail->NextHeader = pHeader;
		params->tail = pHeader;
		pHeader->Type = QBKeyHeader::LOCAL;
		pHeader->QBkey = 0x34406F14;
		pHeader->value.i = oldValue + value;
		pHeader->NextHeader = NULL;
	}
	DEBUGEND();
	return true;
}

bool SubToGlobal(CStruct* pStruct, CScript* pScript)
{
	DEBUGSTART()
	{
		CStructHeader* header = pStruct->head;
		int qbKey = 0;
		int value = 0;
		while (header)
		{
			if (header->QBkey == 0xA1DC81F9)
			{
				qbKey = header->GetInt();
				if (value)
					break;
			}
			if (header->QBkey == 0xE288A7CB)
			{
				value = header->GetInt();
				if (qbKey)
					break;
			}
			header = header->NextHeader;
		}
		int oldValue;
		header = pScript->params->head;
		while (header)
		{
			if (header->QBkey == qbKey)
			{
				oldValue = header->GetInt();
				break;
			}
			header = header->NextHeader;
		}
		header = pScript->params->head;
		while (header)
		{
			if (header->QBkey == 0x34406F14)
			{
				header->value.i = oldValue - value;
				return true;
			}
			header = header->NextHeader;
		}
		CStruct* const __restrict params = pScript->params;
		CStructHeader* const __restrict pHeader = params->AllocateCStruct();
		if (!pHeader)
			return false;
		params->tail->NextHeader = pHeader;
		params->tail = pHeader;
		pHeader->Type = QBKeyHeader::LOCAL;
		pHeader->QBkey = 0x34406F14;
		pHeader->value.i = oldValue + value;
		pHeader->NextHeader = NULL;
	}
	DEBUGEND();
	return true;
}

bool FreezeCamera(CStruct* pStruct, CScript* pScript)
{
	const static DWORD pFreeze = 0x004A7CAC;
	*(BYTE*)pFreeze = 0xEB;
	return true;
}

bool UnfreezeCamera(CStruct* pStruct, CScript* pScript)
{
	const static DWORD pFreeze = 0x004A7CAC;
	*(BYTE*)pFreeze = 0x74;
	return true;
}

bool NotScript(CStruct* pStruct, CScript* pScript)
{
	DEBUGSTART()
	{
		CStructHeader* header = pStruct->head;
		while (header)
		{
			if (header->Type == QBKeyHeader::LOCAL)
			{
				QBKeyHeader* Header = GetQBKeyHeader(header->Data);
				if (Header && Header->type == QBKeyHeader::COMPILED_FUNCTION)
				{
					CStruct params;
					params.head = header->NextHeader;
					params.tail = pStruct->tail;
					if (params.head == NULL)
						params.tail = NULL;
					/*pStruct->head = header->NextHeader;
					if(pStruct->head==NULL)
					pStruct->tail=NULL;*/
					return !Header->CallScript(&params, pScript);
				}
				break;
			}
			header = header->NextHeader;
		}
	}
	DEBUGEND();
	return true;
}

char PoolSize[128] = "";
char PoolSizeText[128] = "";

bool GetMemoryPoolSize(CStruct* pStruct, CScript* pScript)
{
	sprintf(PoolSize, "%u", LevelModSettings::MemorySize / 0x10);
	CStructHeader* header = pScript->params->head;
	while (header)
	{
		if (header->QBkey == 0x083FDB95)
		{
			header->pStr = PoolSize;
			return true;
		}
		header = header->NextHeader;
	}
	CStruct* const __restrict params = pScript->params;
	CStructHeader* const __restrict pHeader = params->AllocateCStruct();
	if (!pHeader)
		return false;
	params->tail->NextHeader = pHeader;
	params->tail = pHeader;
	pHeader->Type = QBKeyHeader::STRING;
	pHeader->QBkey = 0x083FDB95;
	pHeader->pStr = PoolSize;
	pHeader->NextHeader = NULL;
	return true;
}

bool GetMemoryPoolSizeText(CStruct* pStruct, CScript* pScript)
{
	sprintf(PoolSizeText, "MemoryPoolSize: %u", LevelModSettings::MemorySize / 0x10);
	CStructHeader* header = pScript->params->head;
	while (header)
	{
		if (header->QBkey == 0x083FDB95)
		{
			header->pStr = PoolSizeText;
			return true;
		}
		header = header->NextHeader;
	}
	CStruct* const __restrict params = pScript->params;
	CStructHeader* const __restrict pHeader = params->AllocateCStruct();
	if (!pHeader)
		return false;
	params->tail->NextHeader = pHeader;
	params->tail = pHeader;
	pHeader->Type = QBKeyHeader::STRING;
	pHeader->QBkey = 0x083FDB95;
	pHeader->pStr = PoolSizeText;
	pHeader->NextHeader = NULL;
	return true;
}

bool UpdateSpineText(CStruct* pStruct, CScript* pScript)
{
	int id = 0;
	if (pStruct->GetScript("menu_id", &id))
	{
		Element* container = AllocateElement(0);
		Element* element = container->GetElement(id);
		element = (Element*)CastPointer((void*)element, 0, 0x005B6344, 0x005B666C, FALSE);

		FreeElement();
	}
	return true;
}

bool SetMemoryPoolSize(CStruct* pStruct, CScript* pScript);

bool ChangeString(CStruct* pStruct, CScript* pScript)
{

	int s1;
	pStruct->GetScript("string", &s1);
	QBKeyHeader* header = GetQBKeyHeader(s1);
	const char* s2;
	pStruct->GetString("value", &s2);
	memcpy(header->pStr, s2, strlen(s2) + 1);
	return true;
}

bool strstr(CStruct* pStruct, CScript* pScript)
{

	const char* s1;
	pStruct->GetString("s1", &s1);

	const char* s2;
	pStruct->GetString("s2", &s2);

	return strstr(s1, s2);
}


void RemoveMovingObject(SuperSector* sector)
{
	for (DWORD i = 0; i < movingObjects.size(); i++)
	{
		if (movingObjects[i].sector == sector)
		{
			movingObjects.erase(movingObjects.begin() + i);
			return;
		}
	}
}


bool doneIt = false;

bool MoveObjectScript(CStruct* pStruct, CScript* pScript)
{
	int name = 0;
	printf("Maybe Move pScript %p\n", pScript);
	if (pStruct->GetScript("Name", &name) && name != 0)
	{
		SuperSector* sector = SuperSector::GetSuperSector(name);
		if (sector)
		{
			CStructHeader* relpos;
			if (pStruct->GetStruct(Checksums::Relpos, &relpos))
			{
				printf("Going to mov Object -> Relpos %f %f %f pScript %p\n", relpos->pVec->x, relpos->pVec->y, relpos->pVec->z, pScript);


				for (DWORD i = 0; i < movingObjects.size(); i++)
				{
					if (movingObjects[i].sector->name == name)
						return true;
				}

				D3DXVECTOR3 position = (sector->bboxMax + sector->bboxMin) / 2.0f;
				position += *relpos->pVec;
				movingObjects.push_back(MovingObject(sector, position, pScript));
			}
			else if (pStruct->ContainsFlag("FollowPathLinked"))
			{
				printf("Going to mov Object -> FollowPathLinked pScript %p\n", pScript);
				//pScript->node->GetNodeStruct()->GetArray()
				CArray* links = pScript->node->GetNodeStruct()->GetArray(Checksums::Links);
				if (links)
				{
					printf("Links %p", links);
					D3DXVECTOR3 position = (sector->bboxMax + sector->bboxMin) / 2.0f;
					CStructHeader* pos;
					CStructHeader* link = Node::GetNodeStructByIndex((*links)[0]);
					if (link->GetStruct(Checksums::Position, &pos))
					{
						position += *pos->pVec;
						movingObjects.push_back(MovingObject(sector, position, pScript, link));
					}
				}
			}
		}
		else
			printf("Couldn't find SuperSector(0x%X)", name);
	}
	return true;
}

const CompiledScript scripts[] =
{
	/*{"TestFunction",         TestFunc},
	{"Math",         Math},
	{"String",       String},
	{"GenChecksum",       GenChecksum},
	{"CallFuncByName",    CallScriptByName},
	{"CopyString", ScriptCopyString},
	{"ClearObserverState", ClearObserverState},*/
	{"ChangeString", ChangeString},
	{"strstr", strstr},
	{ "GetZAngle", GetZAngle },
	{ "GetSpeed", GetSpeedScript },
	{ "TestForAcid", TestForAcid },
	{ "Wallplant", WallplantScript },
	{ "PreWallplant", PreWallplantScript },
	{ "EnterObserveMode2", EnterObserveMode },
	{ "LeaveObserveMode2", LeaveObserveMode },
	{ "ObserveNext", ObserveNext },
	{ "GetSkaterPos", GetSkaterPos },
	{ "StoreSkaterPos", StoreSkaterPos },
	{ "GotoStoredPos", SetSkaterPos },
	{ "GetSkaterLook", GetSkaterLook },
	{ "UsingNewMenu", UsingNewMenu },
	{ "UsingTeleFix", UsingTeleFix },
	{ "UsingSoundFix", UsingSoundFix },
	{ "ToggleNewMenu", ToggleNewMenu },
	{ "ToggleTeleFix", ToggleTeleFix },
	{ "ToggleSoundFix", ToggleSoundFix },
	{ "Not", NotScript },
	{ "SubToGlobal", SubToGlobal },
	{ "AddToGlobal", AddToGlobal },
	{ "FreezeCamera", FreezeCamera },
	{ "UnfreezeCamera", UnfreezeCamera },
	{ "HookDebugMessages", HookDebugMessages },
	{ "UnHookDebugMessages", UnhookDebugMessages },
{     "CreateConsoleScript", CreateConsole},
{     "HideConsole", HideConsole},
	{ "ToggleLogDebugMessages", ToggleHookDebugMessages },
	{ "SetTagLimit", SetTagLimit },
	{ "GrafStarted", GrafStarted },
	//{"Kill3DGrass", Kill3DGrass},
	{ "UpdateLevelModSettings", UpdateLevelModSettings },
	{ "ToggleGrafCounter", ToggleGrafCounter },
	{ "CounterIsOn", CounterIsOn },
	{ "ChangeValues", ChangeValues },
	{ "CreatePair", CreatePair },
	{ "GetSliderValue", GetSliderValue },
	{ "InitLevelMod", Initialize },
	{ "MoveObject", MoveObjectScript   }
	/*{"SetMemoryPoolSize", SetMemoryPoolSize},
	{"GetMemoryPoolSize", GetMemoryPoolSize},
	{"GetMemoryPoolSizeText", GetMemoryPoolSizeText},*/
	//{"GetMotd", GetMotd},
};

std::vector<std::string> uninstalled;
static char download_message[1024] = "";

void PrintDownloadString(char* string)
{
	//return;
	/*QBKeyHeader* header = GetQBKeyHeader(crc32f((unsigned char*)"download_str"));

	if (header->pStr)
	freex(header->pStr);
	const DWORD lens = strlen(string) + 1;
	header->pStr = (char*)mallocx(lens);
	for (DWORD i = 0; i < lens; i++)
	{
	header->pStr[i] = string[i];
	}*/

	/*CStruct params;
	CStructHeader header;*/
	strncpy(download_message, string, strlen(string) + 1);
	//SetElementText(Checksum("helper_menu_1"), download_message);/**/



	/*header.Type = QBKeyHeader::STRING;
	header.QBkey = crc32f((unsigned char*)"text");
	header.pStr = string;
	params.head = &header;
	params.tail = &header;

	CStructHeader* param = params.AddParam("text", QBKeyHeader::STRING);
	param->pStr = string;

	ExecuteQBScript("SetHelperText", &params);
	/*params.DellocateCStruct();*/
}

unsigned long NumReadable(SOCKET sock) {
	unsigned long n = -1;
	if (ioctlsocket(sock, FIONREAD, &n) < 0) {
		/* look in WSAGetLastError() for the error code */
		return 0;
	}
	return n;
}


bool DownloadAndInstall(std::string& path, float build, SOCKADDR* service)
{
	//FILE* f = fopen(path.c_str(), "wb+");
	sprintf(download_message, "Dling: %s 0%% Version: %.1f", path.c_str(), build);

	SOCKET fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (fd == INVALID_SOCKET)
	{
		PrintDownloadString((char*)"Invalid Socket");
		return false;
	}

	u_long iMode = 0;//Set blocking mode
	ioctlsocket(fd, FIONBIO, &iMode);

	int retry = 0;

	while (connect(fd, service, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		if (retry == 5)
		{
			PrintDownloadString((char*)"can't connect");
			closesocket(fd);
			return false;
		}

		else
		{
			retry++;
		}
	}
	string request = "GET /";
	request.append(path);
	request += " HTTP/1.1\r\n"
		"Referer: http://levelmod.freetzi.com/\r\n"
		"Accept: */*\r\n"
		"User-Agent: Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 6.1; WOW64; Trident/7.0; SLCC2; .NET CLR 2.0.50727; .NET CLR 3.5.30729; .NET CLR 3.0.30729; .NET4.0C; .NET4.0E; Zoom 3.6.0)\r\n"
		"Host: levelmod.freetzi.com\r\n"
		"Cache-Control: no-cache\r\n"
		"\r\n";

	if (send(fd, request.c_str(), request.size(), 0) == SOCKET_ERROR)
	{
		PrintDownloadString((char*)"request timeout?");
		request.clear();
		closesocket(fd);
		return false;
	}

	request.clear();
	std::string response;

	float percent = 0.0f;
	DWORD size = MAXRECV * 1024;

	response.reserve(MAXRECV * 1024);
	char buffer[MAXRECV];
	int resp_leng = recv(fd, (char*)&buffer, MAXRECV, 0);
	if (resp_leng > 0)
	{
		response.append(buffer, resp_leng);
	}
	else
	{
		sprintf(download_message, "ERROR: %d", WSAGetLastError());
	}

	int length = response.find("Content-Length");// +16;

	if (length == string::npos)
	{
		PrintDownloadString((char*)"can't find Content-Length");
		request.clear();
		closesocket(fd);
		return false;
	}
	length += 16;

	sscanf(&response.c_str()[length], "%d\r\n", &size);

	int pos = response.find("\r\n\r\n");

	if (pos == string::npos)
	{
		PrintDownloadString((char*)"Can't find End of header");
		request.clear();
		closesocket(fd);
		return false;
	}

	pos += 4;
	std::string head = response;
	response.erase(0, pos);
	if (size > response.size());
	response.reserve(size);
	//fwrite(&response.begin(), response.size(), 1, f);

	DWORD readBytes = response.size();
	percent = (float)readBytes / (float)size * 100.0f;

	sprintf(&download_message[7], "%s %.2f%% Version: %.1f", path.c_str(), percent, build);

	while (resp_leng > 0)
	{
		resp_leng = recv(fd, (char*)&buffer, MAXRECV, 0);
		if (resp_leng > 0)
		{
			response.append(buffer, resp_leng);
			readBytes += resp_leng;
			percent = (float)readBytes / (float)size * (float)100.0f;
			sprintf(&download_message[7], "%s %.2f%% Version: %.1f", path.c_str(), percent, build);
		}
		else
		{
			sprintf(download_message, "ERROR: %d", WSAGetLastError());
		}
	}
	PrintDownloadString((char*)"Finished Downloading");
	//MessageBox(0, "Really finished?", "", 0);
	//fclose(f);

	while (readBytes < size)
	{
		PrintDownloadString((char*)"not ok");
		sprintf(download_message, "%d/%d bytes downloaded going to retry", readBytes, size);

		char requestString[256] = "";
		sprintf(requestString, "HTTP / 1.1\r\n"
			"Host: levelmod.freetzi.com\r\n"
			"Range: bytes=%d-\r\n"
			"Accept-Language: en-US\r\n"
			"Accept-Encoding: gzip, deflate\r\n"
			"User-Agent: Mozilla FireFox/4.0\r\n"
			//"Connection: Keep-Alive"
			"\r\n", readBytes, size);

		request = "GET /";
		request.append(path);
		request += requestString;

		if (send(fd, &request[0], request.size(), 0) == SOCKET_ERROR)
		{
			closesocket(fd);
			fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (fd == INVALID_SOCKET)
			{
				PrintDownloadString((char*)"Invalid Socket");
				return false;
			}

			u_long iMode = 0;//Set blocking mode
			ioctlsocket(fd, FIONBIO, &iMode);

			retry = 0;

			while (connect(fd, service, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
			{
				if (retry == 5)
				{
					PrintDownloadString((char*)"can't connect");
					closesocket(fd);
					return false;
				}

				else
				{
					retry++;
				}
			}
		}
		request.clear();
		DWORD oldSize = response.size();

		resp_leng = recv(fd, (char*)&buffer, 1024, 0);
		if (resp_leng > 0)
			response.append(buffer, buffer + resp_leng);

		pos = response.find("\r\n\r\n");
		while (pos == string::npos && resp_leng > 0)
		{
			resp_leng = recv(fd, (char*)&buffer, 1024, 0);
			if (resp_leng > 0)
				response.append(buffer, buffer + resp_leng);
			pos = response.find("\r\n\r\n");
		}
		if (pos == string::npos)
		{
			PrintDownloadString((char*)"not found2?");
			request.clear();
			closesocket(fd);
			return false;
		}
		pos += 4;
		readBytes += response.size() - oldSize - pos;
		response.erase(response.begin() + oldSize, response.end());

		//addedBytes = 0;
		percent = (float)readBytes / (float)size * 100.0f;
		sprintf(msg, "Dling: %s %.2f%%", path.c_str(), percent);
		PrintDownloadString(msg);

		while (resp_leng > 0)
		{
			resp_leng = recv(fd, (char*)&buffer, MAXRECV, 0);
			if (resp_leng > 0)
				response.append(buffer, buffer + resp_leng);
			readBytes += resp_leng;
			percent = (float)(readBytes) / (float)size * 100.0f;
			sprintf(msg, "Dling: %s %.2f%%", path.c_str(), percent);
			PrintDownloadString(msg);
		}
	}

	closesocket(fd);

	if (head.find("404 Not Found") == string::npos)
	{
		FILE* f = fopen(path.c_str(), "w+b");
		fwrite(&response[0], response.size(), 1, f);
		fclose(f);
		char installPath[MAX_PATH + 1];
		GetCurrentDirectory(MAX_PATH, installPath);
		char final[MAX_PATH * 2 + 100];
		sprintf(final, "%s/LevelMod-Installer.exe", installPath);
		printf("%s", final);
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		// set the size of the structures
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));
		char cmdArgs[256] = "";
		sprintf(cmdArgs, "LevelMod-Installer.exe %s", path.c_str());


		// start the program up
		CreateProcess(final,   // the path
			cmdArgs,        // Command line
			NULL,           // Process handle not inheritable
			NULL,           // Thread handle not inheritable
			FALSE,          // Set handle inheritance to FALSE
			0,              // No creation flags
			NULL,           // Use parent's environment block
			NULL,           // Use parent's starting directory 
			&si,            // Pointer to STARTUPINFO structure
			&pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
		);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

		CStruct params;
		ExecuteQBScript("MenuQuitGame", &params);
	}
	else
	{
		PrintDownloadString((char*)"404");
		uninstalled.clear();
		response.clear();
		return false;
	}
	//response.clear();
	uninstalled.clear();
	return uninstalled.size() == 0;
}



void DownloadAndInstall(char* address, char* filename)
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return;
	SOCKET fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (fd == INVALID_SOCKET)
	{
		WSACleanup();
		return;
	}
	SOCKADDR_IN service;
	service.sin_family = AF_INET;
	service.sin_port = htons(80);
	LPHOSTENT host = gethostbyname(address);
	if (!host)
	{

		PrintDownloadString((char*)"no host");
		closesocket(fd);
		WSACleanup();
		return;
	}
	service.sin_addr = *((LPIN_ADDR)*host->h_addr_list);
	//DownloadAndInstall(filename, (SOCKADDR*)&service);
}

void GetLevelModVersion()
{
	downloading = true;
	CStruct params;
	ExecuteQBScript("ShowDownloadInfo", &params);
	ExecuteQBScript("SetHelperPos", &params);
	/*ChilkatUnlock();
	ChilkatCheckVersion();*/


	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return;
	SOCKET fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (fd == INVALID_SOCKET)
	{
		WSACleanup();
		return;
	}
	u_long iMode = 0;
	ioctlsocket(fd, FIONBIO, &iMode);
	SOCKADDR_IN service;
	service.sin_family = AF_INET;
	service.sin_port = htons(80);
	LPHOSTENT host = gethostbyname("levelmod.freetzi.com");
	if (!host)
	{

		PrintDownloadString((char*)"no host");
		closesocket(fd);
		WSACleanup();
		return;
	}
	service.sin_addr = *((LPIN_ADDR)*host->h_addr_list);
	if (connect(fd, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)
	{

		PrintDownloadString((char*)"not connected");
		closesocket(fd);
		WSACleanup();
		return;
	}
	PrintDownloadString((char*)"Checking version...");
	static const char* GetStr = "GET /Version.txt HTTP/1.1\r\n"
		"Host: levelmod.freetzi.com\r\n"
		"User-Agent: Mozilla FireFox/4.0\r\n" //"User-Agent: Mozilla / 5.0 (Windows NT 6.1; WOW64; rv:36.0) Gecko / 20100101 Firefox / 36.0\r\n"
		"Accept: text/html, application/xhtml+xml, *//**\r\n"
		"Accept-Language: en-gb,en;q=0.5\r\n"
		"Connection: Keep-Alive\r\n"
		"Accept-Encoding: gzip, deflate\r\n\r\n"
		/*"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*//*;q=0.8\r\n"
		"Accept-Language: en-gb,en;q=0.5\r\n"
		"Accept-Encoding: gzip,deflate\r\n"
		"Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\n\r\n"*/;

	if (send(fd, GetStr, strlen(GetStr) + 1, 0) == SOCKET_ERROR)
	{

		closesocket(fd);
		WSACleanup();
		PrintDownloadString((char*)"error");
		return;
	}
	std::string response;// = "";
	int resp_leng = 1024;
	char buffer[1024];
	while (resp_leng > 0)
	{
		resp_leng = recv(fd, (char*)&buffer, 1024, 0);
		if (resp_leng > 0)
			response += std::string(buffer).substr(0, resp_leng);
	}

	if (strstr(response.c_str(), "404 Not Found"))
	{

		response.clear();

		WSACleanup();
		PrintDownloadString((char*)"404");
		return;
	}
	else
	{
		int pos = response.find("\r\n\r\n");
		pos += 4;
		float latest = 0.0f;
		float build = 0.0f;

		char buildpath[128] = "";
		char buildname[128] = "";

		std::istringstream f(&response.c_str()[pos]);
		std::string line;// , line2;
		std::getline(f, line);

		sscanf(line.c_str(), "[%f]", &build);
		std::getline(f, line);
		bool asked = false;
		//MessageBox(0, "dling", "", 0);
		bool sucess = false;
		if (build > version)
		{
			PrintDownloadString((char*)"Found new Version");

			//std::getline(f, line2);
			/*CStruct params;
			ExecuteQBScript("AskDownload", &params);
			/*QBKeyHeader* header = GetQBKeyHeader(crc32f((unsigned char*)"Answear"));
			while ((DWORD)header->value.i == crc32f((unsigned char*)"None"))
			{
			header = GetQBKeyHeader(crc32f((unsigned char*)"Answear"));
			Sleep(200);
			}*/

			/*if ((DWORD)header->value.i == crc32f((unsigned char*)"No"))
			{
			response.clear();
			line.clear();
			f.clear();
			closesocket(fd);
			WSACleanup();
			downloading = false;
			return;
			}*/
			/*asked = true;
			latest = build;
			//sscanf(line.c_str(), "%s %s", &buildpath, &buildname);
			//DownloadAndInstall(buildpath, buildname);
			sucess = DownloadAndInstall((char*)line.c_str(), (SOCKADDR*)&service);
			/*using namespace System::Net;
			WebClient^ client = gcnew WebClient;
			Uri ^uri = gcnew Uri(address);

			// Specify that the DownloadFileCallback method gets called
			// when the download completes.
			client->DownloadFileCompleted += gcnew AsyncCompletedEventHandler(DownloadFileCallback2);

			// Specify a progress notification handler.
			client->DownloadProgressChanged += gcnew DownloadProgressChangedEventHandler(DownloadProgressCallback);
			client->DownloadDataAsync()
			response.clear();
			closesocket(fd);
			WSACleanup();
			downloading = false;
			return;*/
			closesocket(fd);
			sucess = DownloadAndInstall(line, build, (SOCKADDR*)&service);
			/*std::getline(f, line);
			while (std::getline(f, line))
			{
				if (line[0] == ';')
					break;
				else if (line[0] == '[')
					sscanf(line.c_str(), "[%f]", &latest);
				else
				{
					sucess = DownloadAndInstall((char*)line.c_str(), (SOCKADDR*)&service);
				}
			}*/
		}
		else if (build == version)
			PrintDownloadString((char*)"You have latest version :)");

		//if (sscanf(&response.c_str()[pos], "%f"/* %s %f %s"*///, &latest/*, patchname, &build, buildname*/))
		if (latest > version&& sucess)
		{
			/*FILE* fp = fopen("settings", "r+b");
			fseek(fp, 7, SEEK_SET);
			fwrite(&latest, 4, 1, fp);
			fclose(fp);*/

			/*if(build>version && build != latest)
			{
			DownloadAndInstall(buildname, (SOCKADDR *)&service);
			}
			DownloadAndInstall(patchname, (SOCKADDR *)&service);

			if(uninstalled.size() != 0)
			{
			for(DWORD i=0; i<uninstalled.size(); i++)
			{
			MessageBox(0, uninstalled[i].c_str(), "ERROR couldn't install file", 0);
			}
			uninstalled.clear();
			//DeleteFile("temp.zip");
			}*/
			/*HWND hwnd = ::GetActiveWindow();
			SetFocus(HWND_DESKTOP);
			ShellExecute(NULL, "open", "http://www.chapter-3.net/thps3/site/distribution/viewtopic.php?f=19&t=2677",
			NULL, NULL, SW_SHOWNORMAL);
			if (MessageBox(hwnd, "There is a new version of LevelMod do you want to close thps3?", "LevelMod update checker", MB_YESNO) == IDYES)
			{
			response.clear();
			closesocket(fd);
			WSACleanup();
			WSACleanup();
			ExitProcess(0);
			return;
			}
			else
			SetFocus(hwnd);*/
		}
		line.clear();
		//line2.clear();
		f.clear();
	}

	response.clear();
	//closesocket(fd);
	WSACleanup();
	Sleep(10000);
	downloading = false;
}

bool shouldfix = false;

void FixClampBugs()
{
	if (shouldfix)
	{
		MessageBox(0, clamp, "YEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEAH", 0);
		const BYTE vertfix1[116] = {
		  0xD9, 0x85, 0x60, 0x87, 0x00, 0x00, 0xD8, 0x5C, 0x24, 0x14, 0xDF, 0xE0, 0x25, 0x00, 0x41, 0x00, 0x00, 0x75, 0x0A, 0x8B, 0x85, 0x60, 0x87, 0x00, 0x00, 0x89, 0x44, 0x24, 0x14, 0x8B, 0x44, 0x24, 0x10, 0x8B, 0x4C, 0x24, 0x14, 0xE9, 0x4F, 0x29, 0x0a, 0x00,
		  0x31, 0xFF, 0xD9, 0x85, 0x60, 0x86, 0x00, 0x00, 0xD8, 0x25, 0x38, 0x03, 0x40, 0x00, 0x6A, 0x00, 0x6A, 0x08, 0x6A, 0x00, 0x8B, 0xCD, 0xD9, 0x9D, 0x60, 0x86, 0x00, 0x00, 0xD9, 0x85, 0x70, 0x86, 0x00, 0x00, 0xD8, 0x25, 0x38, 0x03, 0x40, 0x00, 0xD9, 0x9D, 0x70, 0x86, 0x00, 0x00, 0xE8, 0x5B, 0xF9, 0x09, 0x00, 0x84, 0xC0, 0x0F, 0x85, 0xFC, 0x28, 0x0A, 0x00, 0x47, 0x83, 0xFF, 0x0C, 0x7C, 0xC1, 0xE9, 0x5B, 0x2A, 0x0A, 0x00, 0x00, 0x00, 0x40, 0x40
		};
		DWORD old;
		VirtualProtect((void*)0x004A2C39, sizeof(vertfix1), PAGE_EXECUTE_READWRITE, &old);
		memcpy((void*)0x004A2C39, vertfix1, sizeof(vertfix1));
		VirtualProtect((void*)0x004A2C23, sizeof(clampfunc), PAGE_EXECUTE_READWRITE, &old);
		memcpy((void*)0x004A2C39, clampfunc, sizeof(clampfunc));
	}
}

void TestForAcid()
{
	Skater* skater = Skater::GetSkater();
	skater->Store();

	Vertex pos = *(Vertex*)skater->GetPosition();
	Vertex start = pos;
	Vertex end = start;

	Vertex velocity = *(Vertex*)skater->GetVelocity();


	for (float dist = 0.0f; dist < 600.0f; dist += 20.0f)
	{
		Vertex vel_normal = GetNormal(&velocity);

		start.X = pos.X + vel_normal.X * dist;
		start.Z = pos.Z + vel_normal.Z * dist;

		end.X = pos.X + vel_normal.X * dist;
		end.Y = pos.Y - 4000.0f;
		end.Z = pos.Z + vel_normal.Z * dist;
		velocity.Y -= 0.5f;
		skater->SetRay(*(D3DXVECTOR3*)&start, *(D3DXVECTOR3*)&end);


		if (skater->CollisionCheck(0x8, false))
		{
			//
			float height = skater->GetPosition()->y - skater->GetHitPoint()->y;

			if (height < 20.0f)
				break;


			float absnorm = fabsf(skater->GetCollisionNormal()->y);
			if (absnorm > 0.48f && absnorm < 0.98f)
			{
				//printf("norm %f %f vel %f %f", vel_normal.X, vel_normal.Z, velocity.X, velocity.Z);
				float test = ((vel_normal.X - skater->GetCollisionNormal()->x) + (vel_normal.Z - skater->GetCollisionNormal()->z));
				if (fabsf(test) < 0.82f)
				{
					printf("above\n");
					skater->FlagException("AcidDrop");
					break;
				}
			}
		}
		skater->Restore();
	}
}
char* path;
bool game = true;

vector<CStructHeader> compressedStructs;
DWORD numCompressed = 0;

void AddCompressedNodes()
{

	FILE* f = fopen(qbPath, "r+b");
	if (f)
	{
		printf("Opened: %s\n", qbPath);
		fseek(f, 0, SEEK_END);
		DWORD size = ftell(f);

		//ZeroMemory(&compressedStructs.front(), compressedStructs.size() * sizeof(CStructHeader));

		BYTE* pFile = new BYTE[size];

		fseek(f, 0, SEEK_SET);
		fread(pFile, size, 1, f);
		if (*pFile != 0xAA)
			return;
		compNodes.clear();
		compressedStructs.clear();
		pFile++;


		compressedStructs.reserve(size / 4);
		while (*pFile != 0x0)
		{
			switch (*pFile)
			{
			default:
				pFile++;
				break;
			case 0x16:
				pFile++;
				/*sprintf(msg, "P %X", opcode);
				MessageBox(0, msg, msg, 0);*/
				DWORD name = *(DWORD*)pFile;
				printf("Name %X\n", name);
				pFile += 4;


				compNodes.push_back(CompressedNode());
				CompressedNode* comp = &compNodes.back();

				comp->checksum = name;

				/*CStructHeader* pMap = *(CStructHeader**)0x008E1E04;
				pMap = pMap->NextHeader;
				*(CStructHeader**)0x008E1E04 = pMap;*/
				compressedStructs.push_back(CStructHeader());
				comp->first = &compressedStructs.back();



				CStructHeader* header = comp->first;
				comp->last = header;

				printf("chc %X comp %X\n", name, comp);
				while (*pFile != 0 && *pFile != 1)
				{
					pFile++;

					DWORD name = *(DWORD*)pFile;
					pFile += 4;
					header->Type = QBKeyHeader::QBKeyType::LOCAL;

					if (*(pFile) == 0x7)
					{
						pFile += 2;
						DWORD script = *(DWORD*)pFile;
						pFile += 4;
						header->QBkey = name;
						header->Data = script;

						if (*(pFile) != 1 && *pFile != 0)
						{
							/*CStructHeader* pMap = *(CStructHeader**)0x008E1E04;
							pMap = pMap->NextHeader;
							*(CStructHeader**)0x008E1E04 = pMap;*/
							compressedStructs.push_back(CStructHeader());
							header->NextHeader = &compressedStructs.back();
							header = header->NextHeader;
						}
						else
						{
							header->NextHeader = 0;
							comp->last = header;
							break;
						}
					}
					else
					{
						header->Data = name;

						if (*(pFile) != 1 && *pFile != 0)
						{
							/*CStructHeader* pMap = *(CStructHeader**)0x008E1E04;
							pMap = pMap->NextHeader;
							*(CStructHeader**)0x008E1E04 = pMap;*/
							compressedStructs.push_back(CStructHeader());
							header->NextHeader = &compressedStructs.back();
							/*pMap = pMap->NextHeader;
							*(CStructHeader**)0x008E1E04 = pMap;
							printf("pMap %X\n", pMap);*/
							header = header->NextHeader;
						}
						else
						{
							header->NextHeader = 0;
							comp->last = header;
							break;
						}

					}
				}

				header->NextHeader = 0;
				comp->last = header;

				break;
			}
		}



		delete[] pFile;
		fclose(f);

	}
}
bool hookDone = true;

#define HASH_SIZE 0x2FFD0
#define OLD_HASH_SIZE 0xBFF4
BYTE hashTable[HASH_SIZE] = { 0 };//the node hash names, each item is 12 bytes
#define OTHER_SIZE 0x140000
#define OLD_OTHER_SIZE 0x14000
//BYTE otherTable[0x50000];//unknown table, each item is 20 bytes

static DWORD pOld = 0x008A8B48;
static DWORD pNew = (DWORD)&hashTable;

#pragma pack(1)
struct TriggerScript
{
	DWORD qbKey;
	DWORD unk1;
	DWORD test;//should be 2
	//BYTE padding[3];
	void* script;
	void* link;
};
#pragma pop(pack)
#define MAX_TRIGGERS OTHER_SIZE / 0x14
#define OLD_MAX_TRIGGERS OLD_OTHER_SIZE / 0x14
TriggerScript triggers[MAX_TRIGGERS]{ 0 };
static DWORD pOld2 = 0x0087D8F8;
static DWORD pNew2 = (DWORD)&triggers;
TriggerScript* oldTriggers = (TriggerScript*)pOld2;

void HookedFopen(char* p)
{
	if (p && strlen(p))
	{
		if (p[strlen(p) - 1] == 'b' && p[strlen(p) - 2] == 'q' && p[strlen(p) - 3] == '.' && game)
		{
			printf("Fopen: %s\n", p);
			memcpy(qbPath, p, strlen(p) + 1);
			qbPath[strlen(qbPath) - 1] = 0x0;
			if (!hookDone)
			{
				MessageBox(0, "going to hook", "", 0);
				hookDone = true;

				for (DWORD i = 0; i < OTHER_SIZE; i += 4)
				{
					*(DWORD*)(pNew2 + i) = 0;
				}

				for (DWORD i = 0; i < OLD_OTHER_SIZE; i += 4)
				{
					*(DWORD*)(pNew2 + i) = *(DWORD*)(pOld2 + i);
				}


				/*for (DWORD i = 0; i < OLD_MAX_TRIGGERS; i++)
				{
					DWORD q = oldTriggers[i].qbKey;
					q &= 0x0000FFFF;
					q = q * 4 + q;

					TriggerScript* t = (TriggerScript*)(q*4 + pNew2);
					if((BYTE)t->test!=2)
					{
						t->qbKey = oldTriggers[i].qbKey;
						t->unk1 = oldTriggers[i].unk1;
						t->test = oldTriggers[i].test;
						//t->padding = oldTriggers[i].padding;
						t->script = oldTriggers[i].script;
						t->link = oldTriggers[i].link;

					}
					else
					{
						while (t->qbKey)
						{
							t++;
						}
						t->qbKey = oldTriggers[i].qbKey;
						t->unk1 = oldTriggers[i].unk1;
						t->test = oldTriggers[i].test;
						//t->padding = oldTriggers[i].padding;
						t->script = oldTriggers[i].script;
						t->link = oldTriggers[i].link;
					}
				}*/

				* (DWORD*)0x00426123 = (DWORD)&triggers;
				*(DWORD*)0x0042635A = (DWORD)&triggers;
				*(DWORD*)0x0042B91C = (DWORD)&triggers;
				*(DWORD*)0x0042C231 = (DWORD)&triggers;
				*(DWORD*)0x0042612A = (DWORD)&triggers + 8;
				*(DWORD*)0x00426353 = (DWORD)&triggers + 8;
				*(DWORD*)0x0042B923 = (DWORD)&triggers + 8;
				*(DWORD*)0x0042C0F2 = (DWORD)&triggers + 16;
				*(DWORD*)0x0042C10D = (DWORD)&triggers + OTHER_SIZE + 0x10;

				*(DWORD*)0x0042C24D = (DWORD)&triggers + OTHER_SIZE;

				/**(DWORD*)0x004282B1 = (DWORD)&otherTable + OTHER_SIZE;
				*(DWORD*)0x004282BD = (DWORD)&otherTable + OTHER_SIZE;*/
				/**(BYTE*)0x0042611A = 0xFF;
				*(BYTE*)0x0042634A = 0xFF;
				*(BYTE*)0x0042B913 = 0xFF;*/
				MessageBox(0, "HOOKED3", "", 0);
			}
			/*MessageBox(0, p, qbPath, 0);
			AddCompressedNodes();*/
			if (debugMode)
				qbTable->OpenScript(p);
		}
	}
}

/*__declspec(naked) void Fopen_naked()
{
	static const DWORD jmpBack = 0x0040314A + 4;
	_asm pushad;
	_asm pushfd;
	_asm mov path, ecx
	HookedFopen(path);
	_asm popfd;
	_asm popad;
	_asm add esp, 0x0C;
	_asm mov al, [ecx];
	_asm jmp[jmpBack];
}*/


void AddChecksum(int key, char* name)
{
	lastQB = name;
	/*if (qbTable->qbTable.find(key) == qbTable->qbTable.end())
	{
		printf("AddChecksum %s 0x%X\n", name, key);
		qbTable->qbTable.insert(std::pair<int, char*>(key, name));
	}*/
}

__declspec(naked) void Checksum_naked()
{
	static const DWORD jmpBack = 0x004110E4;
	static DWORD chc = 0;

	_asm mov ecx, [esp + 4];
	_asm mov path, ecx;

	_asm call[jmpBack];
	_asm mov chc, eax
	_asm pushad;
	_asm pushfd;
	AddChecksum(chc, path);
	_asm popfd;
	_asm popad;
	_asm mov eax, chc
	_asm ret;
}

/*CStructHeader* HookAddCStruct(int ParamName, int Data, int qbType)
{
	if (ParamName == 0xD)
	{

	}
}*/

/*struct Node
{
	CStructHeader* first;
	CStructHeader* last;
};
*/






void CStruct::AddCompressedNode(DWORD checksum, QBKeyInfoContainer* container)
{
	for (int i = 0; i < compNodes.size(); i++)
	{
		if (compNodes[i].checksum == checksum)
		{
			if (!compNodes[i].added)
			{

				CStructHeader* header = compNodes[i].first;
				/*CStructHeader* pMap = *(CStructHeader**)0x008E1E04;

				compNodes[i].first = pMap->NextHeader;*/

				CStructHeader* added = this->AddCStruct(header->QBkey, header->Type, header->Data);
				compNodes[i].first = added;
				header = header->NextHeader;

				while (header)
				{

					added = this->AddCStruct(header->QBkey, header->Type, header->Data);
					header = header->NextHeader;
				}
				/*pMap->NextHeader = 0;*/
				compNodes[i].added = true;
				compNodes[i].last = added;
			}
			else
			{
				if (this->tail)
					this->tail->NextHeader = compNodes[i].first;

				if (!this->head)
					this->head = compNodes[i].first;
				this->tail = compNodes[i].last;

				if (container)
				{
					CStructHeader* header = this->head;
					while (header != this->tail && header)
					{

						printf("CONTAINER:.:..\n");
						typedef DWORD(__cdecl* const pShiftQBKey)(DWORD qbKey);
						DWORD shifted = pShiftQBKey(0x00426DB0)(header->Data);


						typedef QBKeyInfo* (__thiscall* const pAddInfo)(QBKeyInfoContainer* pThis, DWORD qbKey);

						QBKeyInfo* info = pAddInfo(0x00428A30)(container, shifted);

						typedef void(__thiscall* const pAddContainer)(CStruct* pThis, QBKeyInfo* info);
						pAddContainer(0x004292B0)(this, info);

						header = header->NextHeader;
					}
				}
			}
			printf("Added Comp Node: %X %X COMP %X\n", checksum, this, &compNodes[i]);


			break;
		}
	}

}

BYTE* CStruct::MaybeAddCompressed(DWORD qbKey, BYTE* pFile, QBKeyInfoContainer* container)
{
	typedef BYTE* (__thiscall* const pAddCStruct)(CStruct* pThis, DWORD qbKey, BYTE* pFile, QBKeyInfoContainer* other);
	if (qbKey != 0x68A9B4E1)
		return pAddCStruct(0x00428DF0)(this, qbKey, pFile, container);

	pFile++;
	DWORD checksum = *(DWORD*)pFile;
	pFile += 4;
	AddCompressedNode(checksum, container);
	return pFile;
}

/*__declspec(naked) void Ncomp_naked()
{
	static CStruct* node = NULL;
	static DWORD checksum = 0;
	static const DWORD jmpBack = 0x004294C3;
	static const DWORD jmpBack2 = 0x0042945F;
	static const DWORD oldESP = 0;
	static BYTE* pFile = NULL;
	_asm push esp
	_asm pushad;
	_asm pushfd;
	_asm cmp ebx, 0x68A9B4E1;
	_asm je hi;
	_asm popfd;
	_asm popad;
	_asm pop esp;
	_asm add esp, 8;
	_asm mov ecx, ebp;
	_asm jmp[jmpBack];
hi:

	_asm mov node, ebp;
	_asm mov pFile, eax;
	game = false;
	pFile++;
	checksum = *(DWORD*)pFile;
	pFile += 4;

	AddCompressedNode(checksum, node);

	game = true;
	_asm popfd;
	_asm popad;
	_asm pop esp
	_asm add esp, 8
	//_asm mov byte ptr[esp - 0x15], 00;
	_asm mov eax, pFile;
	_asm jmp[jmpBack2];
}*/

__declspec(naked) void Fopen_naked()
{
	static DWORD fopen = *(DWORD*)0x0058D0B0;
	static const DWORD jmpBack = fopen + 5;
	_asm mov path, esp;
	_asm pushad;
	_asm pushfd;

	path = path + 0x4;
	path = (char*)*(DWORD*)path;
	HookedFopen(path);
	_asm popfd;
	_asm popad;
	_asm mov edi, edi;
	_asm push ebp;
	_asm mov ebp, esp;
	_asm jmp[jmpBack];
}

FILE* __cdecl _fopen(const char* p, const char* b)
{
	printf("Fopen: %s\n");
	if (p[strlen(p) - 1] == 'b' && p[strlen(p) - 2] == 'q' && p[strlen(p) - 3] == '.' && game)
	{

		memcpy(qbPath, p, strlen(p) + 1);
		qbPath[strlen(qbPath) - 1] = 0x0;
		//MessageBox(0, p, qbPath, 0);
		AddCompressedNodes();
		qbTable->OpenScript((char*)p);
	}

	return fopen(p, b);
}

__declspec(naked) void TestForAcid_naked()
{
	static DWORD retaddr = 0;
	_asm pop retaddr;
	_asm pushad;
	_asm pushfd;
	TestForAcid();
	_asm popfd;
	_asm popad;
	_asm sub esp, 0x000000B4
	_asm push retaddr;
	_asm ret
}

struct BBox
{
	Vertex max;
	Vertex min;
};

struct CollisionObject;

struct UnkStruct
{
	UnkStruct* prev;
	UnkStruct* next;
	DWORD flags;//??0x34?
	CollisionObject* object;
	void* unk1;
	bool(*pFunction)(UnkStruct*, UnkStruct*);//some collision testing maybe??
	WORD numUnk2;//0x2E?
	WORD flag;//0x1
	DWORD unk2;//0x0
	DWORD numVerts;
	DWORD numTris;
};

struct CollisionObject//+0x64 collisionflags*
{
	DWORD transform;//0xFFFFFFFF
	DWORD checksum;
	WORD* indices;
	Vertex* verticse;
	DWORD zero;//could be transform?
	void* unk1;//never accessed?
	void* unk2;//never accessed?
	void* unk3;//dunno
	UnkStruct* unk;
	void* unkptr[6];//ptrs to itself?? like if on address 0050000 then it 
	//ptrs to itself and 0050000+4 ptrs to 0050000 and 0050000+8 
	//ptrs to 0050000+8 and 0050000+12 ptrs to 0050000+8 etc
	BBox bbox;
	DWORD padding;//never accessed
	WORD numTris;//??
	DWORD numTris2;//??
	void* unkptr2[2];
	WORD* collFlags;//vert, skatable, etc
};

void FixVertBugs()
{
	const BYTE vertjmp1[] = { 0xE9, 0x8A, 0xD6, 0xF5, 0xFF, 0x90, 0x90, 0x90 };
	DWORD old;
	VirtualProtect((void*)0x004A2C39, sizeof(vertjmp1), PAGE_EXECUTE_READWRITE, &old);
	memcpy((void*)0x004A2C39, vertjmp1, sizeof(vertjmp1));
	const BYTE vertfix1[116] = {
	  0xD9, 0x85, 0x60, 0x87, 0x00, 0x00, 0xD8, 0x5C, 0x24, 0x14, 0xDF, 0xE0, 0x25, 0x00, 0x41, 0x00, 0x00, 0x75, 0x0A, 0x8B, 0x85, 0x60, 0x87, 0x00, 0x00, 0x89, 0x44, 0x24, 0x14, 0x8B, 0x44, 0x24, 0x10, 0x8B, 0x4C, 0x24, 0x14, 0xE9, 0x4F, 0x29, 0x0a, 0x00,
	  0x31, 0xFF, 0xD9, 0x85, 0x60, 0x86, 0x00, 0x00, 0xD8, 0x25, 0x38, 0x03, 0x40, 0x00, 0x6A, 0x00, 0x6A, 0x08, 0x6A, 0x00, 0x8B, 0xCD, 0xD9, 0x9D, 0x60, 0x86, 0x00, 0x00, 0xD9, 0x85, 0x70, 0x86, 0x00, 0x00, 0xD8, 0x25, 0x38, 0x03, 0x40, 0x00, 0xD9, 0x9D, 0x70, 0x86, 0x00, 0x00, 0xE8, 0x5B, 0xF9, 0x09, 0x00, 0x84, 0xC0, 0x0F, 0x85, 0xFC, 0x28, 0x0A, 0x00, 0x47, 0x83, 0xFF, 0x0C, 0x7C, 0xC1, 0xE9, 0x5B, 0x2A, 0x0A, 0x00, 0x00, 0x00, 0x40, 0x40
	};
	VirtualProtect((void*)0x004002c8, sizeof(vertfix1), PAGE_EXECUTE_READWRITE, &old);
	memcpy((void*)0x004002c8, vertfix1, sizeof(vertfix1));
	const BYTE vertjmp2[] = { 0x0F, 0x84, 0xC9, 0xD6, 0xF5, 0xFF };
	VirtualProtect((void*)0x004A2C23, sizeof(vertjmp2), PAGE_EXECUTE_READWRITE, &old);
	memcpy((void*)0x004A2C23, vertjmp2, sizeof(vertjmp2));
	/*const BYTE vertfix2 [] = {0x31, 0xFF, 0xD9, 0x85, 0x60, 0x86, 0x00, 0x00, 0xD8, 0x25, 0x38, 0x03, 0x40, 0x00, 0x6A, 0x00, 0x6A, 0x08, 0x6A, 0x00, 0x8B, 0xCD, 0xD9, 0x9D, 0x60, 0x86, 0x00, 0x00, 0xD9, 0x85, 0x70, 0x86, 0x00, 0x00, 0xD8, 0x25, 0x38, 0x03, 0x40, 0x00, 0xD9, 0x9D, 0x70, 0x86, 0x00, 0x00, 0xE8, 0x5B, 0xF9, 0x09, 0x00, 0x84, 0xC0, 0x0F, 0x85, 0xFC, 0x28, 0x0A, 0x00, 0x47, 0x83, 0xFF, 0x0C, 0x7C, 0xC1, 0xE9, 0x5B, 0x2A, 0x0A, 0x00, 0x00, 0x00, 0x40, 0x40};
	VirtualProtect((void*)0x004002F2, sizeof(vertfix2), PAGE_EXECUTE_READWRITE, &old);
	memcpy((void*)0x004002F2, vertfix2, sizeof(vertfix2));*/
	const DWORD b = 0x0058EA10;
	VirtualProtect((void*)b, 4, PAGE_EXECUTE_READWRITE, &old);
	*(float*)b = 36.0f;
	/*const DWORD c = 0x004A2BDD;
	VirtualProtect((void*)c, 1, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)c = 0x4B;*/
}

//BYTE RenderFix[4096] = { 0 };

void FixRenderBugs()
{
	//const DWORD renderjmpAddr1 = 0x004f4ba9;
	const BYTE renderjmp1[] = { 0xE9, 0x10, 0xB6, 0xF0, 0xFF };
	DWORD old;
	VirtualProtect((void*)0x004f4ba9, sizeof(renderjmp1), PAGE_EXECUTE_READWRITE, &old);
	memcpy((void*)0x004f4ba9, renderjmp1, sizeof(renderjmp1));
	const BYTE renderFix1[] = { 0x85, 0xD2, 0x0F, 0x84, 0x10, 0x4A, 0x0F, 0x00, 0x89, 0x0A, 0x8B, 0x48, 0x10, 0xE9, 0xDE, 0x49, 0x0F, 0x00 };
	VirtualProtect((void*)0x004001be, sizeof(renderFix1), PAGE_EXECUTE_READWRITE, &old);
	memcpy((void*)0x004001be, renderFix1, sizeof(renderFix1));

	VirtualProtect((void*)0x004FEE77, 4, PAGE_EXECUTE_READWRITE, &old);
	*(DWORD*)0x004FEE77 = 0x008A9000;// (DWORD)&RenderFix;

	VirtualProtect((void*)0x004FEEA4, 4, PAGE_EXECUTE_READWRITE, &old);
	*(DWORD*)0x004FEEA4 = 0x008A9000;//(DWORD)&RenderFix;


	VirtualProtect((void*)0x004FEEAF, 4, PAGE_EXECUTE_READWRITE, &old);
	*(DWORD*)0x004FEEAF = 0x008A9000;//(DWORD)&RenderFix;

	VirtualProtect((void*)0x004FEEC6, 4, PAGE_EXECUTE_READWRITE, &old);
	*(DWORD*)0x004FEEC6 = 0x008A9000;//(DWORD)&RenderFix;

	VirtualProtect((void*)0x004FF050, 4, PAGE_EXECUTE_READWRITE, &old);
	*(DWORD*)0x004FF050 = 0x008A9000;//(DWORD)&RenderFix;

	/*DWORD Old = 0x0090b048;
	for (DWORD i = 0; i < 1024; i++)
	{
		*(DWORD*)(RenderFix + i * 4) = *(DWORD*)(old + i * 4);
	}*/

	/*for (DWORD i = 0; i < 4096; i++)
	{
		RenderFix[i] = 0;
	}*/

}

FILE* Dump;

void AddDump(const char* dump, ...)
{
	Dump = fopen("dump.txt", "r+t");
	fseek(Dump, 0, SEEK_END);
	va_list va_alist;
	char logbuf[512] = { 0 };

	va_start(va_alist, dump);
	_vsnprintf(logbuf + strlen(logbuf), sizeof(logbuf) - strlen(logbuf), dump, va_alist);
	va_end(va_alist);
	fprintf(Dump, logbuf);
	fclose(Dump);
}

char* ChatText = NULL;

const Vertex* const __restrict GetSkaterPos()
{
	static const DWORD ptr = 0x008E2498;
	VALIDATE_PTR((void*)ptr);
	DWORD pPos = *(DWORD*)ptr + 0x13C;
	VALIDATE_PTR((void*)pPos);
	pPos = *(DWORD*)pPos + 0x14;
	VALIDATE_PTR((void*)pPos);
	const DWORD old = pPos;
	pPos = *(DWORD*)pPos + 0x4BC;
	if (InvalidReadPtr((void*)pPos))
	{
		pPos = old;
		pPos = *(DWORD*)pPos + 0x3B4;
		VALIDATE_PTR((void*)pPos);
	}
	pPos = *(DWORD*)pPos + 0x18;
	VALIDATE_DATA((Vertex*)pPos, sizeof(Vertex));
	return (const Vertex* const __restrict)pPos;
}

float GetSkaterYAngle()
{
	/*angle on ground
	atan(maybe abs(new.z - old.z / maybe abs(new.x - old.x)
	angle up/down
	atan(maybe abs(new.y - old.y / maybe abs(new.x - old.x)
	or maube 100% not working mot-algo unless you use p4*/
	static const DWORD ptr = 0x008E1E90;
	VALIDATE_PTR((void*)ptr);
	DWORD pAngle = *(DWORD*)ptr + 0xC0;
	VALIDATE_PTR((void*)pAngle);
	pAngle = *(DWORD*)pAngle + 0x5C4;
	VALIDATE_PTR((void*)pAngle);
	pAngle = *(DWORD*)pAngle + 0x0C;//--
	VALIDATE_PTR((void*)pAngle);
	pAngle = *(DWORD*)pAngle + 0x10;
	VALIDATE_PTR((void*)pAngle);
	pAngle = *(DWORD*)pAngle + 0x40;//--
	VALIDATE_DATA((float*)pAngle, sizeof(float));
	float checker = *(float*)pAngle;
	pAngle += 8;
	VALIDATE_DATA((float*)pAngle, sizeof(float));
	float angle = *(float*)pAngle;
	if (checker >= 0.0f)
	{
		return (angle + 1.0f) * 3.14159f * 0.5f;
	}
	else
	{
		return ((angle + 1.0f) * 3.14159f * 0.5f) * -1.0f;
	}
}


inline void ReadChat()
{
	DWORD chatLen = strlen(ChatText);
	if (ChatText && chatLen > 150)
		ChatText[149] = 0x0;
	for (DWORD i = 0; i < chatLen; i++)
	{
		if (ChatText[i] == '%')
			ChatText[i] = '_';
	}
	static DWORD CrownCount = 0, RestartCount = 0, ZoneCount = 0, numSpawns = 0;

	register const char* ptr = &ChatText[0];
	/*if(*ptr++ == '/')
	{*/
	if (*ptr == '/')
	{
		const char* message = ptr + 1;

		char command[12];
		DWORD j = 0;
		while (message[j] != 0x20 && message[j] != 0x0)
		{
			command[j] = message[j];
			j++;
			if (j > 12)
				j = 0;
		}
		if (j)
		{
			command[j] = 0x00;

			DWORD cmd = crc32f((unsigned char*)command);
			for (DWORD i = 0; i < sizeof(commands) / sizeof(Command); i++)
			{
				if (cmd == commands[i].checksum)
				{
					commands[i].function(message);
					break;
				}
			}
		}
		/*char* commands[] = {
		  "tell", "kick", "ban", "getinfo", "console", "debug", NULL
		  };

		  DWORD i = 0;
		  char* command = commands[i];
		  while (command)
		  {
		  DWORD j = 0;
		  while (*command != 0)
		  {
		  if (*command++ != message[j])
		  goto nextCommand;
		  j++;
		  }
		  switch (i)
		  {
		  case 4:
		  CreateConsole();
		  printf("Welcome to Console!!!!\n\n");
		  *(BYTE*)0x00478983 = 0x75;
		  break;
		  case 5:
		  HookDebugMessages(NULL, NULL);
		  *(BYTE*)0x00478983 = 0x75;
		  break;
		  case 0:
		  message += 5;
		  char name[128];
		  DWORD j = 0;

		  while (*message != 0x20 && *message != 0)
		  {
		  name[j] = *message++;
		  j++;
		  }
		  name[j] = 0;
		  message++;

		  SendPm(name, message);
		  *(BYTE*)0x00478983 = 0x75;
		  break;
		  }
		  break;
		  nextCommand:
		  i++;
		  command = commands[i];
		  }*/
	}
	if (*ptr++ == 'a' && *ptr++ == 'd' && *ptr++ == 'd')
	{
		static const char* commands[] = { "flag", "zone", "key", "crown", "spawn" };
		DWORD i = 0;
	NextCommand:

		const char* pCommand = commands[i];
		//char* ptr = &ChatText[4];
		while (*pCommand)
		{
			if (*pCommand != *ptr)
			{
				if (i < sizeof(commands) / sizeof(char*))
				{
					i++;
					ptr = &ChatText[3];
					goto NextCommand;
				}
				else
					return;
			}
			ptr++;
			pCommand++;
		}
		if (i == 0)
		{
			Vertex vertex = *GetSkaterPos();
			if (*ptr == '\0')
			{
				AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = TRG_CTF_Team1\nClass = GenericNode\nType = CTF_1\nCreatedAtStart }\n \r\n", vertex.X, vertex.Y, vertex.Z * -1);
				AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = team1\nClass = Restart\nType = UserDefined\nCreatedAtStart RestartName = \"Team: CTF\"\nrestart_types = ARRAY(\n\nCTF_1\n)\nTriggerScript = TRG_SpawnSkater\n}\n \r\n", vertex.X, vertex.Y, vertex.Z * -1);
			}
			else if (strstr(ptr, "blue"))
			{
				AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = TRG_CTF_Team1\nClass = GenericNode\nType = CTF_1\nCreatedAtStart }\n \r\n", vertex.X, vertex.Y, vertex.Z * -1);
				AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = team1\nClass = Restart\nType = UserDefined\nCreatedAtStart RestartName = \"Team: CTF\"\nrestart_types = ARRAY(\n\nCTF_1\n)\nTriggerScript = TRG_SpawnSkater\n}\n \r\n", vertex.X, vertex.Y, vertex.Z * -1);
			}
			else if (strstr(ptr, "red"))
			{
				AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = TRG_CTF_Team02\nClass = GenericNode\nType = CTF_2\nCreatedAtStart }\n \r\n", vertex.X, vertex.Y, vertex.Z * -1);
				AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = team2\nClass = Restart\nType = UserDefined\nCreatedAtStart RestartName = \"Team: CTF\"\nrestart_types = ARRAY(\n\nCTF_2\n)\nTriggerScript = TRG_SpawnSkater\n}\n \r\n", vertex.X, vertex.Y, vertex.Z * -1);
			}
			else
			{
				AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = TRG_CTF_Team1\nClass = GenericNode\nType = CTF_1\nCreatedAtStart }\n \r\n", vertex.X, vertex.Y, vertex.Z * -1);
				AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = team1\nClass = Restart\nType = UserDefined\nCreatedAtStart RestartName = \"Team: CTF\"\nrestart_types = ARRAY(\n\nCTF_1\n)\nTriggerScript = TRG_SpawnSkater\n}\n \r\n", vertex.X, vertex.Y, vertex.Z * -1);
			}
			*(BYTE*)0x00478983 = 0x75;
		}
		else if (i == 1)
		{
			Vertex vertex = *GetSkaterPos();
			unsigned int zone_multiplier = 1;
			sscanf(ChatText, "addzone %d", &zone_multiplier);
			AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = TRG_CONTROL_ZONE%d\nClass = GenericNode\nType = ZONE\nCreatedAtStart zone_multiplier = %d\n}\n \r\n", vertex.X, vertex.Y + 50, vertex.Z * -1, ZoneCount, zone_multiplier);
			ZoneCount++;
			CrownCount++;
			AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = TRG_NewCrown%d\nClass = GenericNode\nType = Crown\nCreatedAtStart }\n \r\n", vertex.X, vertex.Y, vertex.Z * -1, CrownCount);
			*(BYTE*)0x00478983 = 0x75;
		}
		else if (i == 2)
		{
			Vertex vertex = *GetSkaterPos();
			AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = TRG_ZONE_KEY\nClass = GenericNode\nType = ZONE_KEY\nCreatedAtStart zone_multiplier = 1\n}\n \r\n", vertex.X, vertex.Y, vertex.Z * -1);
			*(BYTE*)0x00478983 = 0x75;
		}
		else if (i == 3)
		{
			Vertex vertex = *GetSkaterPos();
			CrownCount++;
			AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = TRG_NewCrown%d\nClass = GenericNode\nType = Crown\nCreatedAtStart }\n \r\n", vertex.X, vertex.Y, vertex.Z * -1, CrownCount);
			*(BYTE*)0x00478983 = 0x75;
		}
		else if (i == 4)
		{
			numSpawns++;
			Vertex vertex = *GetSkaterPos();
			float angle = GetSkaterYAngle();
			AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; %f; 0.0]\nName = TRG_Restart%u\nClass = Restart\nType = UserDefined\nCreatedAtStart RestartName = \"Restart %u\"\nrestart_types = ARRAY(\nMultiPlayer\n)\nTriggerScript = TRG_SpawnSkater\n}\n\r\n", vertex.X, vertex.Y, vertex.Z * -1, angle, numSpawns, numSpawns);
			*(BYTE*)0x00478983 = 0x75;
		}
	}
	// }
	/*if(strstr(ChatText,"addflag"))
	{

	Vertex vertex = *GetSkaterPos();

	if(strstr(ChatText,"blue"))
	{
	AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = TRG_CTF_Team1\nClass = GenericNode\nType = CTF_1\nCreatedAtStart }\n \r\n",vertex.X,vertex.Y,vertex.Z*-1);
	AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = team1\nClass = Restart\nType = UserDefined\nCreatedAtStart RestartName = Team: CTF\nrestart_types = ARRAY(\n\nCTF_1\n)\nTriggerScript = TRG_SpawnSkater\n}\n \r\n",vertex.X,vertex.Y,vertex.Z*-1);
	}
	else if(strstr(ChatText,"red"))
	{
	AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = TRG_CTF_Team02\nClass = GenericNode\nType = CTF_2\nCreatedAtStart }\n \r\n",vertex.X,vertex.Y,vertex.Z*-1);
	AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = team2\nClass = Restart\nType = UserDefined\nCreatedAtStart RestartName = Team: CTF\nrestart_types = ARRAY(\n\nCTF_2\n)\nTriggerScript = TRG_SpawnSkater\n}\n \r\n",vertex.X,vertex.Y,vertex.Z*-1);
	}
	else
	{
	AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = TRG_CTF_Team1\nClass = GenericNode\nType = CTF_1\nCreatedAtStart }\n \r\n",vertex.X,vertex.Y,vertex.Z*-1);
	AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = team1\nClass = Restart\nType = UserDefined\nCreatedAtStart RestartName = Team: CTF\nrestart_types = ARRAY(\n\nCTF_1\n)\nTriggerScript = TRG_SpawnSkater\n}\n \r\n",vertex.X,vertex.Y,vertex.Z*-1);
	}

	}
	else if(!strcmp(ChatText,"addzone"))
	{
	Vertex vertex = *GetSkaterPos();
	AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = TRG_CONTROL_ZONE%d\nClass = GenericNode\nType = ZONE\nCreatedAtStart zone_multiplier = 1\n}\n \r\n",vertex.X,vertex.Y+50,vertex.Z*-1,ZoneCount);
	ZoneCount++;
	}

	else if(strstr(ChatText,"addzone "))
	{
	unsigned int zone_multiplier = 1;
	Vertex vertex = *GetSkaterPos();
	sscanf(ChatText,"addzone %d",&zone_multiplier);
	AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = TRG_CONTROL_ZONE%d\nClass = GenericNode\nType = ZONE\nCreatedAtStart zone_multiplier = %d\n}\n \r\n",vertex.X,vertex.Y+50,vertex.Z*-1,ZoneCount,zone_multiplier);
	ZoneCount++;
	}

	else if(!strcmp(ChatText,"addkey"))
	{
	Vertex vertex = *GetSkaterPos();
	AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = TRG_ZONE_KEY\nClass = GenericNode\nType = ZONE_KEY\nCreatedAtStart zone_multiplier = 1\n}\n \r\n",vertex.X, vertex.Y, vertex.Z*-1);
	}

	else if(!strcmp(ChatText,"addcrown"))
	{
	Vertex vertex = *GetSkaterPos();
	CrownCount++;
	AddDump("STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = TRG_NewCrown%d\nClass = GenericNode\nType = Crown\nCreatedAtStart }",vertex.X,vertex.Y,vertex.Z*-1,CrownCount);
	}

	/*else if(strstr(ChatText,"addspawn "))
	{
	Vertex vertex = *GetSkaterPos();
	RestartCount++;
	char Chat[MAX_PATH];
	sprintf(Chat,"STRUCT{\nPosition = VECTOR[%f; %f; %f]\nAngles = VECTOR[0.0; 0.0; 0.0]\nName = TRG_NewRestart_%d\nClass = Restart\nType = MultiPlayer\nCreatedAtStart RestartName = \x22NewRestart%d\x22\nrestart_types = ARRAY(\nMultiPlayer ",vertex.X, vertex.Y, vertex.Z*-1,RestartCount,RestartCount);
	if(strstr(ChatText,"horse"))
	strcat(Chat,"Horse ");
	if(strstr(ChatText,"blue"))
	strcat(Chat,"CTF_1");
	if(strstr(ChatText,"red"))
	strcat(Chat,"CTF_2");
	AddDump(Chat);
	AddDump("\n)\nTriggerScript = TRG_Spawn\n}\n \r\n");

	if(!SpawnFunc)
	{
	SpawnFunc=true;
	AddDump("\n#15801  FUNCTION SpawnFunc\n#15808 MakeSkaterGoto StartSkating1\n#15809 END FUNCTION\n \r\n");
	}
	}*/
}

void CommandConsole(const char* message)
{
	CreateConsole();
	printf("Welcome to Console!!!!\n\n");
	*(BYTE*)0x00478983 = 0x75;
}
void CommandDebug(const char* message)
{
	HookDebugMessages(NULL, NULL);
	*(BYTE*)0x00478983 = 0x75;
}
void CommandTell(const char* message)
{
	message += 5;
	char name[128];
	DWORD j = 0;

	while (*message != 0x20 && *message != 0)
	{
		name[j] = *message++;
		j++;
	}
	if (message != 0)
	{
		name[j] = 0;
		message++;

		SendPm(name, message);
		*(BYTE*)0x00478983 = 0x75;
	}
}

char* ChatText2 = NULL;

void FixChatHook(char* chatText)
{
	/*chatText[0] = 0x00;
	chatText[1] = 0x00;*/
	if (chatText && strlen(chatText) > 149)
	{
		chatText[0] = 0x00;
		chatText[1] = 0x00;
		chatText[148] = 0x00;
		chatText[149] = 0x00;
	}
}


__declspec(naked) void ReadChatHook(void)
{
	static DWORD ReadChatRetAddr = 0;
	__asm
	{

		pop ReadChatRetAddr

		mov ChatText, edx

		pushad
		pushfd
	}
	*(BYTE*)0x00478983 = 0x74;
	ReadChat();
	__asm
	{
		popfd
		popad

		sub esp, 0x000001E8

		push ReadChatRetAddr

		/*_asm mov eax, 0x004788CA
		_asm mov [esp], eax*/
		ret
	}
}

void ShowPm(char* message)
{
	CStruct params;
	CStructHeader* param = params.AddParam("text", QBKeyHeader::STRING);
	param->pStr = message;
	while (*message != 0x0)
	{
		if (*message == '%')
			*message = '_';
		message++;
	}
	ExecuteQBScript("LaunchGrafCounter", &params);
	params.DellocateCStruct();
}

char* GetPlayerName()
{
	return (*(char**)0x00970158) + 0x4;
}

void GotMessage(char* message)
{
	char* playerName = GetPlayerName();
	char* c = message;
	while (*c != 0x3A)
	{
		if (*c++ != *playerName++)
		{
			ShowPm(message);
			return;
		}
	}
}

__declspec(naked) void AddMessageHook(void)
{
	static DWORD retAddr = 0;
	_asm pop retAddr;
	static char* message = NULL;
	static DWORD old = 0;
	_asm
	{
		test edx, edx;
		mov esi, [esp + 0x00000820];
		je skip;
		test esi, esi;
		jne skip;
		mov old, eax
			lea eax, [esp + 0x0C];
		test eax, eax;
		je skip;
		mov message, eax;
		mov eax, old
			pushad;
	}
	GotMessage(++message);
	_asm
	{
		popad;
	skip:
		push retAddr;
		ret;
	}



	/*static DWORD AddMessageRetAddr = 0;
	char* message;
	__asm
	{

	pop AddMessageRetAddr;

	test edx, edx;
	je skip;
	test esi, esi;
	jne skip;
	pushad;
	pushfd;
	lea eax, [esp+0x2C];
	test eax, eax;
	je skip;
	mov message, eax;
	}
	GotMessage(message++);
	__asm
	{

	popfd;
	popad;
	skip:
	mov esi,[esp+0x00000820];

	push AddMessageRetAddr;

	ret;
	}*/
}

VOID WriteBytesASM(DWORD destAddress, LPVOID patch, DWORD numBytes)
{
	// Store old protection of the memory page
	DWORD oldProtect = 0;

	// Store the source address
	DWORD srcAddress = PtrToUlong(patch);

	// Make sure page is writeable
	VirtualProtect((void*)(destAddress), numBytes, PAGE_EXECUTE_READWRITE, &oldProtect);

	// Do the patch (oldschool style to avoid memcpy)
	__asm
	{
		nop						// Filler
		nop						// Filler
		nop						// Filler

		mov esi, srcAddress		// Save the address
		mov edi, destAddress	// Save the destination address
		mov ecx, numBytes		// Save the size of the patch
		Start :
		cmp ecx, 0				// Are we done yet?
			jz Exit					// If so, go to end of function

			mov al, [esi]			// Move the byte at the patch into AL
			mov[edi], al			// Move AL into the destination byte
			dec ecx					// 1 less byte to patch
			inc esi					// Next source byte
			inc edi					// Next destination byte
			jmp Start				// Repeat the process
			Exit :
		nop						// Filler
			nop						// Filler
			nop						// Filler
	}

	// Restore old page protection
	VirtualProtect((void*)(destAddress), numBytes, oldProtect, &oldProtect);
}

VOID HookFunc(DWORD destAddress, VOID(*func)(VOID), BYTE nopCount)
{
	DWORD offset = (PtrToUlong(func) - destAddress) - 5;

	BYTE nopPatch[0xFF] = { 0 };

	BYTE patch[5] = { 0xE8, 0x00, 0x00, 0x00, 0x00 };
	memcpy(patch + 1, &offset, sizeof(DWORD));
	WriteBytesASM(destAddress, patch, 5);

	if (nopCount == 0)
		return;

	memset(nopPatch, 0x90, nopCount);

	WriteBytesASM(destAddress + 5, nopPatch, nopCount);
}

static char motd[128] = "";


void GetMotd()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return;
	SOCKET fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (fd == INVALID_SOCKET)
	{
		WSACleanup();
		return;
	}
	SOCKADDR_IN service;
	service.sin_family = AF_INET;
	service.sin_port = htons(80);
	LPHOSTENT host = gethostbyname("vadru.chapter-3.net");
	if (!host)
	{
		closesocket(fd);
		WSACleanup();
		return;
	}
	service.sin_addr = *((LPIN_ADDR)*host->h_addr_list);
	if (connect(fd, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)
	{
		closesocket(fd);
		WSACleanup();
		return;
	}
	static const char* GetStr = "GET /LevelModMotd HTTP/1.1\r\n"
		"Host: vadru.chapter-3.net\r\n"
		"User-Agent: Mozilla FireFox/4.0\r\n"
		"User-Agent: Mozilla Firefox/4.0\r\n"
		"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*//*;q=0.8\r\n"
		"Accept-Language: en-gb,en;q=0.5\r\n"
		"Accept-Encoding: gzip,deflate\r\n"
		"Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\n\r\n";

	if (send(fd, GetStr, strlen(GetStr) + 1, 0) == SOCKET_ERROR)
	{
		closesocket(fd);
		WSACleanup();
		return;
	}
	std::string response;// = "";
	int resp_leng = 1024;
	char buffer[1024];
	while (resp_leng > 0)
	{
		resp_leng = recv(fd, (char*)&buffer, 1024, 0);
		if (resp_leng > 0)
			response += std::string(buffer).substr(0, resp_leng);
	}
	if (strstr(response.c_str(), "404 Not Found"))
	{
		response.clear();
		closesocket(fd);
		WSACleanup();
		return;
	}
	else
	{
		int pos = response.find("\r\n\r\n");
		if (pos != string::npos)
		{
			pos += 4;
			response.erase(0, pos);
			for (DWORD i = 0, len = response.size(); i < len; i++)
			{
				motd[i] = response[i];
			}
		}
	}

	response.clear();
	closesocket(fd);
	WSACleanup();
}

bool init = true;
bool first = true;

bool Initialize(CStruct*, CScript*)
{
	if (first)
	{
		first = false;
		return false;
	}
	if (init)
	{
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)GetLevelModVersion, 0, 0, 0);
		init = false;
		return true;
	}
	return false;
}

void FixMessage()
{
	Sleep(500);
	QBKeyHeader* header = GetQBKeyHeader(Checksum("ver"));
	DWORD times = 0;
	while (header == 0 && times < 100)
	{
		header = GetQBKeyHeader(Checksum("ver"));
		times++;
		Sleep(50);
	}
	if (header)
	{
		std::ostringstream ss;
		ss << VERSION;
		if (header->pStr)
			freex(header->pStr);
		static const std::string message = "2011-2013, LevelMod " + std::string(ss.str());

		const DWORD len = message.length() + 1;
		header->pStr = (char*)mallocx(len);
		for (DWORD i = 0; i < len; i++)
		{
			header->pStr[i] = message[i];
		}

		header = GetQBKeyHeader(Checksum("motd"));

		if (header->pStr)
			freex(header->pStr);
		const DWORD lens = strlen(motd) + 1;
		header->pStr = (char*)mallocx(lens);
		for (DWORD i = 0; i < lens; i++)
		{
			header->pStr[i] = motd[i];
		}
	}
}

/**(DWORD*)0x0042C18D = 0x000FFFFF;
*(DWORD*)0x0042C1C7 = 0x000FFFEF;
*(DWORD*)0x0042C1D5 = 0x000FFFEF;
*(DWORD*)0x0042C1E1 = 0x000FFFF7;
*(DWORD*)0x0042C1EC = 0x000FFFFB;
*(BYTE*)0x00426088 = 0x74;
*(BYTE*)0x004260E7 = 0x74;
*(BYTE*)0x004288F7 = 0x74;
*(BYTE*)0x00428B97 = 0x74;
*(BYTE*)0x004288F7 = 0x74;*;*/

bool DefaultMemoryMode()
{
	FILE* temp = fopen("temp", "w+b");
	if (temp)
	{
		LevelModSettings::MemorySize = 0xFA000;

		CStructHeader* backup = *(CStructHeader**)0x008E1E00;
		for (DWORD i = 0; i < 0xFA00; i++)
		{
			fwrite(backup, sizeof(CStructHeader), 1, temp);
			backup++;
		}
		freex((void*)*(CStructHeader**)0x008E1E00);
		CStructHeader* head = (CStructHeader*)mallocx(0xFA000);
		CStructHeader** ptr = *(CStructHeader***)0x008E1E2C + 64000;
		CStructHeader** end = *(CStructHeader***)0x008E1E30;

		CStructHeader* old = *(CStructHeader**)0x008E1E00;
		CStructHeader* oldEnd = old + 0xFA00;

		while (ptr < end)
		{
			if (*ptr > old&&* ptr < oldEnd)
			{
				CStructHeader* parser = *ptr;
				DWORD index = ((DWORD)parser - (DWORD)old) / 0x10;
				parser = head + index;
				if (parser > head&& parser < (head + 0xFA00))
					*ptr = parser;
			}
			ptr++;//;= (CStructHeader**)((DWORD)ptr+1);
		}
		//CStructHeader* old = *(CStructHeader**)0x008E1E00;
		*(CStructHeader**)0x008E1E00 = head;
		CStructHeader* parser = *(CStructHeader**)0x008E1E04;
		DWORD index = ((DWORD)parser - (DWORD)old) / 0x10;
		*(CStructHeader**)0x008E1E04 = head + index;//*(CStructHeader**)0x008E1E04+(head-old);
		if (fflush(temp) == 0)
		{
			fseek(temp, 0, SEEK_SET);
			backup = head;
			for (DWORD i = 0; i < 0xFA00; i++)
			{
				fread(backup, sizeof(CStructHeader), 1, temp);
				backup++;
			}
			fclose(temp);
			CStructHeader* header = head;
			DWORD last = (DWORD)head + 0xFA000;
			while ((DWORD)header < last)
			{
				header->NextHeader = header + 1;
				header++;
			}

			header = head + 0xF9FF;
			header->Type = QBKeyHeader::QBKeyType::UNDEFINED;
			header->value.i = 0;
			header->NextHeader = 0;
			FILE* f = fopen("settings", "w+b");
			if (f)
			{
				fwrite(&LevelModSettings::UseNewMenu, 1, 1, f);
				fwrite(&LevelModSettings::FixSound, 1, 1, f);
				fwrite(&LevelModSettings::TeleFix, 1, 1, f);
				fwrite(&oldLimit, 1, 1, f);
				bool disableGrass = false;
				QBKeyHeader* header = GetQBKeyHeader(Checksum("GrassLayersDisabled"));
				if (header)
					disableGrass = header->value.i != 0;
				fwrite(&disableGrass, 1, 1, f);
				fclose(f);
				_fcloseall();
			}
		}
		else
			fclose(temp);
		DeleteFile("temp");
		return true;
	}
	return false;
}

/*VirtualProtect((void*)0x004260E7, 7, PAGE_EXECUTE_READWRITE, &old);
*(DWORD*)0x004260E7 = 0x9090F775;
*(WORD*)0x004260EB = 0x9090;
*(BYTE*)0x004260ED = 0x90;*/

void AddLock()
{
	DWORD old;

	VirtualProtect((void*)0x004288F1, 1, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x00428B91, 1, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x004260E1, 1, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((void*)0x00426081, 1, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)0x004288F1 = 0xF9;
	*(BYTE*)0x00428B91 = 0xF9;
	*(BYTE*)0x004260E1 = 0xF9;
	*(BYTE*)0x00426081 = 0xF9;

	VirtualProtect((void*)0x004260E7, 7, PAGE_EXECUTE_READWRITE, &old);
	*(DWORD*)0x004260E7 = 0x9090F775;
	*(WORD*)0x004260EB = 0x9090;
	*(BYTE*)0x004260ED = 0x90;

	VirtualProtect((void*)0x004288F7, 7, PAGE_EXECUTE_READWRITE, &old);
	*(DWORD*)0x004288F7 = 0x9090F775;
	*(WORD*)0x004288FB = 0x9090;
	*(BYTE*)0x004288FD = 0x90;

	VirtualProtect((void*)0x00428B97, 4, PAGE_EXECUTE_READWRITE, &old);
	*(DWORD*)0x00428B97 = 0x3DEBF775;

	VirtualProtect((void*)0x00426088, 4, PAGE_EXECUTE_READWRITE, &old);
	*(DWORD*)0x00426088 = 0x2DEBF675;

	/*VirtualProtect((void*)0x004260E7, 4, PAGE_EXECUTE_READWRITE, &old);
	*(DWORD*)0x004260E7 = 0x3DEBF775;*/
}

bool HighMemoryMode(DWORD newSize)
{
	if (newSize == LevelModSettings::MemorySize)
		return true;
	if (newSize <= 0xFA000)
	{
		if (LevelModSettings::MemorySize != 0xFA000)
			return DefaultMemoryMode();
		return false;
	}
	DWORD newCount = newSize / 0x10;
	DWORD oldSize = LevelModSettings::MemorySize;
	if (newSize < oldSize)
	{
		//MessageBox(0, "new size needs to be bigger than old", "...", 0);
		//return false;
		oldSize = newSize;
	}
	FILE* temp = fopen("temp", "w+b");
	if (temp)
	{
		LevelModSettings::MemorySize = newSize;
		DWORD oldCount = oldSize / 0x10;

		CStructHeader* backup = *(CStructHeader**)0x008E1E00;
		for (DWORD i = 0; i < oldCount; i++)
		{
			fwrite(backup, sizeof(CStructHeader), 1, temp);
			backup++;
		}
		ZeroMemory((void*)*(CStructHeader**)0x008E1E00, oldSize);
		freex((void*)*(CStructHeader**)0x008E1E00);
		CStructHeader* head = (CStructHeader*)mallocx(newSize);
		CStructHeader** ptr = *(CStructHeader***)0x008E1E2C + 30000 - 3804;
		//*(ptr-5)=head+newCount;
		CStructHeader** end = *(CStructHeader***)0x008E1E30 + ((DWORD) * (CStructHeader***)0x008E1E34);//120000

		CStructHeader* old = *(CStructHeader**)0x008E1E00;
		CStructHeader* oldEnd = old + oldCount;

		while (ptr < end)
		{
			if (*ptr >= old && *ptr < oldEnd)
			{
				CStructHeader* parser = *ptr;
				DWORD index = ((DWORD)parser - (DWORD)old) / 0x10;
				parser = head + index;
				if (parser >= head && parser < (head + newCount))
					*ptr = parser;
			}
			ptr++;//;= (CStructHeader**)((DWORD)ptr+1);
		}

		if (head == NULL)
		{
			newSize = oldSize;
			newCount = oldCount;
			head = (CStructHeader*)mallocx(oldSize);
			if (head == NULL)
			{
				newSize = 0xFA000;
				newCount = 0xFA00;
				head = (CStructHeader*)mallocx(0xFA000);
				if (head == NULL)
					return false;
			}
		}
		//CStructHeader* old = *(CStructHeader**)0x008E1E00;
		*(CStructHeader**)0x008E1E00 = head;
		CStructHeader* parser = *(CStructHeader**)0x008E1E04;
		if (parser >= old && parser < oldEnd)
		{
			DWORD index = ((DWORD)parser - (DWORD)old) / 0x10;
			*(CStructHeader**)0x008E1E04 = head + index;//*(CStructHeader**)0x008E1E04+(head-old);
		}
		if (fflush(temp) == 0)
		{
			fseek(temp, 0, SEEK_SET);
			backup = head;
			for (DWORD i = 0; i < oldCount; i++)
			{
				fread(backup, sizeof(CStructHeader), 1, temp);
				CStructHeader* parser = (CStructHeader*)backup->pData;
				if (parser >= old && parser < oldEnd)
				{
					DWORD index = ((DWORD)parser - (DWORD)old) / 0x10;
					parser = head + index;
					if (parser >= head && parser < (head + newCount))
						backup->pData = (void*)parser;
				}
				if (backup->NextHeader == NULL || (backup->NextHeader >= old && backup->NextHeader < oldEnd))
					backup->NextHeader = backup + 1;
				backup++;
			}
			fclose(temp);


			CStructHeader* header = head + (oldCount - 1);//-1
			DWORD last = (DWORD)head + newSize;
			while ((DWORD)header < last)
			{
				header->Type = QBKeyHeader::QBKeyType::UNDEFINED;
				header->QBkey = 0;
				header->Data = 0;
				header->NextHeader = header + 1;
				header++;
			}
			header = head + (newCount - 1);
			header->Type = QBKeyHeader::QBKeyType::UNDEFINED;
			header->QBkey = 0;
			header->Data = 0;
			header->NextHeader = NULL;
		}
		else
			fclose(temp);
		DeleteFile("temp");
		return true;
	}
	return false;
}

bool SetMemoryPoolSize(CStruct* pStruct, CScript* pScript)
{
	void** puEBP = NULL;
	__asm { mov puEBP, ebp };
	char ok[25];
	sprintf(ok, "%X %p", pStruct, pScript->params);
	MessageBox(0, ok, ok, 0);
	/*void * pvReturn = puEBP[2];
	char ok[25];
	sprintf(ok, "%X %p", pvReturn, pvReturn);
	MessageBox(0,ok,"",0);
	puEBP = (void**)puEBP[0];
	sprintf(ok, "%X %p", puEBP, puEBP);
	MessageBox(0,ok,"",0);*/
	Sleep(1000);
	const char* string;
	if (pStruct->GetString("string", &string))
	{
		const  BYTE* it = (const BYTE*)string;
		DWORD size = 0;
		while (*it >= '0' && *it <= '9')
		{
			size = size * 10 + *it - '0';
			it++;
		}
		if (size)
		{
			ExecuteQBScript("StopScripts", NULL);
			*(bool*)0x008E1DF9 = true;
			CStructHeader* old = *(CStructHeader**)0x008E1E00;

			DWORD oldSize = LevelModSettings::MemorySize;
			DWORD oldCount = oldSize / 0x10;

			bool ret = HighMemoryMode(size * 0x10);



			CStructHeader* head = *(CStructHeader**)0x008E1E00;
			CStructHeader* oldEnd = old + oldCount;

			CStructHeader* header = pStruct->head;
			if (header)
			{
				if (header >= old && header <= oldEnd)
				{
					DWORD index = ((DWORD)pStruct->head - (DWORD)old) / 0x10;
					pStruct->head = head + index;
				}
				if (pStruct->tail >= old && pStruct->tail <= oldEnd)
				{
					DWORD index = ((DWORD)pStruct->tail - (DWORD)old) / 0x10;
					pStruct->tail = head + index;
				}
				while (header && header->NextHeader)
				{
					if (header->NextHeader >= old && header->NextHeader <= oldEnd)
					{
						DWORD index = ((DWORD)header->NextHeader - (DWORD)old) / 0x10;
						header->NextHeader = head + index;
					}
					header = header->NextHeader;
				}
			}
			if (pScript->params)
			{
				header = pScript->params->head;
				if (header)
				{
					if (header >= old && header <= oldEnd)
					{
						DWORD index = ((DWORD)pScript->params->head - (DWORD)old) / 0x10;
						pScript->params->head = head + index;
					}
					if (pScript->params->tail >= old && pScript->params->tail <= oldEnd)
					{
						DWORD index = ((DWORD)pScript->params->tail - (DWORD)old) / 0x10;
						pScript->params->tail = head + index;
					}
					while (header && header->NextHeader)
					{
						if (header->NextHeader >= old && header->NextHeader <= oldEnd)
						{
							DWORD index = ((DWORD)header->NextHeader - (DWORD)old) / 0x10;
							header->NextHeader = head + index;
						}
						header = header->NextHeader;
					}
				}
			}

			*(bool*)0x008E1DF9 = false;
			//MessageBox(0,"done",0,0);
			return ret;
		}

		return false;
	}

	if (LevelModSettings::MemorySize != 0xFA000)
	{
		*(bool*)0x008E1DF9 = true;
		bool ret = DefaultMemoryMode();
		*(bool*)0x008E1DF9 = false;
		return ret;
	}
	return false;
}

DWORD newSize;

void HighMemMode()
{

}

void FixChat()
{
	DWORD old;
	VirtualProtect((VOID*)0x004558FB, 5, PAGE_EXECUTE_READWRITE, &old);
	VirtualProtect((VOID*)0x00455912, 4, PAGE_EXECUTE_READWRITE, &old);
	*(DWORD*)0x004558FB = 0x5011468D;
	*(BYTE*)0x004558FF = 0x90;


	*(DWORD*)0x00455912 = (DWORD)FixChatHook - 0x00455916;
}

void InitSkater()
{
	Slerp::transfer = false;
	Slerp::slerping = false;
	D3DXMatrixIdentity(&Slerp::start);
	D3DXMatrixIdentity(&Slerp::end);
	D3DXMatrixIdentity(&Slerp::old);
	Slerp::duration = 0.0f;
	Slerp::radians = 0.0f;
	Slerp::timer = 0.0f;
	Slerp::axis = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

void D3D8BASE(void)
{
	Sleep(10000);
	//CreateConsole();
	/*HookDebugMessages(NULL, NULL);
	*(BYTE*)0x00478983 = 0x75;
	PresentHook = Hook(15, HookedPresent);
	pPresent = (Present_t)PresentHook.func;*/
}


DWORD oldTagCount = 0;

int LineCount = 0;
struct Point {
	float x, y, z, rhw;
	DWORD color;


}; //lineList[1000], drawList[1000];

struct Line
{
	D3DXVECTOR3 v;
	float rhw;
	D3DXCOLOR color;

	Line(D3DXVECTOR3 _v, D3DXCOLOR _c)
	{
		this->v = _v;
		rhw = 1.0f;
		this->color = _c;
	}
};


std::vector<Line> lineList;

HRESULT __stdcall HookedPresent(LPDIRECT3DDEVICE8 pDevice, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
{
	if (!m_font)
	{
		//MessageBox(0,"init font","",0);
		rct.left = 200;
		rct.right = 200 + 1000;
		rct.top = 10;
		rct.bottom = 10 + 80;

		D3DXCreateFont(pDevice, CreateFont(45, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial")), &m_font);
	}

	DWORD numMovingObjects = movingObjects.size();
	if (numMovingObjects)
	{
		Skater* skater = Skater::GetSkater();
		if (skater)
		{
			for (DWORD i = 0; i < movingObjects.size(); i++)
			{
				movingObjects[i].Update(skater->GetFrameLength());
			}
		}
	}
	if (Modulating())
	{
		//TestForAcid();
		DWORD tagCount = GetTagCount();
		if (tagCount != oldTagCount)
		{
			if (tagCount && tagCount < 1000)
			{
				oldTagCount = tagCount;
				sprintf(&tags[6], "%u %X", tagCount, *(DWORD*)(0x0040033C + ((tagCount - 1) * 4)));
				CStruct params;
				CStructHeader* param = params.AddParam("text", QBKeyHeader::STRING);
				param->pStr = tags;
				ExecuteQBScript("LaunchGrafCounter", &params);
				params.DellocateCStruct();
			}
			else
			{
				oldTagCount = 0;
				CStruct params;
				ExecuteQBScript("LaunchGrafCounter", &params);
			}
		}
	}
	else if (downloading)
	{
		m_font->DrawText(download_message, -1, &rct, 0, D3DCOLOR_ARGB(255, 0, 0, 255));
	}
	if (false)
	{
		pDevice->BeginScene();

		DWORD  D3DCMP, pShared, vShared, Cull, StencileEnable, Clipping, Clipplane, COLOROP1, COLORARG1, COLORARG2, COLOROP2 = 0;
		UINT oldStride = 0;

		DWORD oldRenderState;
		pDevice->GetRenderState(D3DRS_ZENABLE, &oldRenderState);
		pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		pDevice->GetPixelShader(&pShared);
		pDevice->SetPixelShader(NULL);
		/* IDirect3DVertexBuffer8* oldBuffer = NULL;
		 IDirect3DBaseTexture8* oldTexture1, * oldTexture2 = NULL;
		 pDevice->GetStreamSource(0, &oldBuffer, &oldStride);
		 pDevice->GetPixelShader(&pShared);
		 pDevice->GetVertexShader(&vShared);*/
		pDevice->GetRenderState(D3DRS_CULLMODE, &Cull);

		//m_pIDirect3DDevice8->SetStreamSource(0,0,0);

		//m_pIDirect3DDevice8->SetViewport(&pViewport);
		//m_pIDirect3DDevice8->SetTexture(0,NULL);
		/*pDevice->SetPixelShader(NULL);


		pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
		//m_pIDirect3DDevice8->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);
		pDevice->GetRenderState(D3DRS_ZFUNC, &D3DCMP);
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		//m_pIDirect3DDevice8->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );
		//m_pIDirect3DDevice8->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		pDevice->GetRenderState(D3DRS_STENCILENABLE, &StencileEnable);
		pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
		pDevice->GetRenderState(D3DRS_CLIPPING, &Clipping);
		pDevice->SetRenderState(D3DRS_CLIPPING, FALSE);
		pDevice->GetRenderState(D3DRS_CLIPPLANEENABLE, &Clipplane);
		pDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, FALSE);

		pDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);
		pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN |
			D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);

		pDevice->GetTextureStageState(0, D3DTSS_COLOROP, &COLOROP1);
		pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTSS_COLOROP);
		pDevice->GetTextureStageState(0, D3DTSS_COLORARG1, &COLORARG1);
		pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		pDevice->GetTextureStageState(0, D3DTSS_COLORARG2, &COLORARG2);
		pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

		pDevice->GetTextureStageState(1, D3DTSS_COLOROP, &COLOROP2);
		pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
		pDevice->GetTexture(0, &oldTexture1);
		pDevice->GetTexture(1, &oldTexture2);
		pDevice->SetTexture(0, NULL);
		pDevice->SetTexture(1, NULL);*/


		pDevice->SetVertexShader(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);


		pDevice->DrawPrimitiveUP(D3DPT_LINELIST, lineList.size() / 2, &lineList.front(), sizeof(Line));
		pDevice->EndScene();

		pDevice->SetRenderState(D3DRS_ZENABLE, oldRenderState);
		pDevice->SetPixelShader(pShared);
		/*pDevice->SetTexture(0, oldTexture1);
		pDevice->SetTexture(1, oldTexture2);
		pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
		//m_pIDirect3DDevice8->SetRenderState( D3DRS_ZENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_STENCILENABLE, StencileEnable);
		pDevice->SetRenderState(D3DRS_CLIPPING, Clipping);
		pDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, Clipplane);*/


		pDevice->SetRenderState(D3DRS_CULLMODE, Cull);
		/*pDevice->SetPixelShader(pShared);
		pDevice->SetVertexShader(vShared);
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP);
		pDevice->SetStreamSource(0, oldBuffer, oldStride);*/



	}
	/*else if (downloading)
	{
	m_font->DrawText(msg, -1, &rct, 0, D3DCOLOR_ARGB(255, 0, 0, 255));
	}*/
	/*else
	{
	DWORD tagCount = GetTagCount();
	if (tagCount && tagCount < 1000)
	{
	oldTagCount = tagCount;
	DWORD crc = *(DWORD*)(0x0040033C + (tagCount * 4));
	if (crc)
	{
	sprintf(&tags[6], "%X", crc);
	CStruct params;
	CStructHeader* param = params.AddParam("text", QBKeyHeader::STRING);
	param->pStr = tags;
	ExecuteQBScript("LaunchGrafCounter", &params);
	params.DellocateCStruct();
	}
	}
	else if(oldTagCount != 0)
	{
	oldTagCount = 0;
	CStruct params;
	ExecuteQBScript("LaunchGrafCounter", &params);
	}
	}*/
	if (observing && observe)
	{
		observe->Update();
	}

	/*DWORD  D3DCMP, pShared, vShared, Cull, StencileEnable, Clipping, Clipplane, COLOROP1, COLORARG1, COLORARG2, COLOROP2;
	UINT oldStride;
	IDirect3DVertexBuffer8* oldBuffer;
	IDirect3DBaseTexture8* oldTexture1, *oldTexture2;
	m_pIDirect3DDevice8->GetStreamSource(0, &oldBuffer, &oldStride);
	m_pIDirect3DDevice8->GetPixelShader(&pShared);
	m_pIDirect3DDevice8->GetVertexShader(&vShared);
	m_pIDirect3DDevice8->GetRenderState(D3DRS_CULLMODE, &Cull);

	//m_pIDirect3DDevice8->SetStreamSource(0,0,0);

	//m_pIDirect3DDevice8->SetViewport(&pViewport);
	//m_pIDirect3DDevice8->SetTexture(0,NULL);
	m_pIDirect3DDevice8->SetPixelShader(NULL);


	m_pIDirect3DDevice8->SetRenderState(D3DRS_ZENABLE, TRUE);
	//m_pIDirect3DDevice8->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);
	m_pIDirect3DDevice8->GetRenderState(D3DRS_ZFUNC, &D3DCMP);
	m_pIDirect3DDevice8->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	//m_pIDirect3DDevice8->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );
	//m_pIDirect3DDevice8->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
	m_pIDirect3DDevice8->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pIDirect3DDevice8->GetRenderState(D3DRS_STENCILENABLE, &StencileEnable);
	m_pIDirect3DDevice8->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	m_pIDirect3DDevice8->GetRenderState(D3DRS_CLIPPING, &Clipping);
	m_pIDirect3DDevice8->SetRenderState(D3DRS_CLIPPING, FALSE);
	m_pIDirect3DDevice8->GetRenderState(D3DRS_CLIPPLANEENABLE, &Clipplane);
	m_pIDirect3DDevice8->SetRenderState(D3DRS_CLIPPLANEENABLE, FALSE);

	m_pIDirect3DDevice8->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);
	m_pIDirect3DDevice8->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN |
	D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);

	m_pIDirect3DDevice8->GetTextureStageState(0, D3DTSS_COLOROP, &COLOROP1);
	m_pIDirect3DDevice8->SetTextureStageState(0, D3DTSS_COLOROP, D3DTSS_COLOROP);
	m_pIDirect3DDevice8->GetTextureStageState(0, D3DTSS_COLORARG1, &COLORARG1);
	m_pIDirect3DDevice8->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pIDirect3DDevice8->GetTextureStageState(0, D3DTSS_COLORARG2, &COLORARG2);
	m_pIDirect3DDevice8->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	m_pIDirect3DDevice8->GetTextureStageState(1, D3DTSS_COLOROP, &COLOROP2);
	m_pIDirect3DDevice8->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	m_pIDirect3DDevice8->GetTexture(0, &oldTexture1);
	m_pIDirect3DDevice8->GetTexture(1, &oldTexture2);
	m_pIDirect3DDevice8->SetTexture(0, NULL);
	m_pIDirect3DDevice8->SetTexture(1, NULL);


	m_pIDirect3DDevice8->SetVertexShader(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	for (DWORD i = 0; i < pointList.size(); i++)
	m_pIDirect3DDevice8->DrawPrimitiveUP(D3DPT_LINELIST, (pointList[i].numNodes - 1) / 2, &pointList[i].v[0], sizeof(pointList[i].v[0]));*/


	//}
	/*m_pIDirect3DDevice8->SetTexture(0, oldTexture1);
	m_pIDirect3DDevice8->SetTexture(1, oldTexture2);
	m_pIDirect3DDevice8->SetRenderState(D3DRS_ZENABLE, FALSE);
	//m_pIDirect3DDevice8->SetRenderState( D3DRS_ZENABLE, TRUE);
	m_pIDirect3DDevice8->SetRenderState(D3DRS_STENCILENABLE, StencileEnable);
	m_pIDirect3DDevice8->SetRenderState(D3DRS_CLIPPING, Clipping);
	m_pIDirect3DDevice8->SetRenderState(D3DRS_CLIPPLANEENABLE, Clipplane);


	m_pIDirect3DDevice8->SetRenderState(D3DRS_CULLMODE, Cull);
	m_pIDirect3DDevice8->SetPixelShader(pShared);
	m_pIDirect3DDevice8->SetVertexShader(vShared);
	m_pIDirect3DDevice8->SetRenderState(D3DRS_ZFUNC, D3DCMP);
	m_pIDirect3DDevice8->SetStreamSource(0, oldBuffer, oldStride);*/
	if (pHookedPresent != NULL)
		pHookedPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
	return pPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}
/*
void Test()
{
	Sleep(10000);
	MessageBox(0, "", "", 0);
	DWORD old, addr, offset = 0;
	VirtualProtect((void*)0x004294BE, 5, PAGE_EXECUTE_READWRITE, &old);
	*(BYTE*)0x004294BE = 0xE9;
	addr = 0x004294BF;
	offset = (PtrToUlong(Ncomp_naked) - 0x004294BE) - 5;
	*(DWORD*)addr = offset;
}
*/

bool DumpScripts(CStruct* params, CScript* pScript)
{
	CScript::DumpScripts();
	return true;
}

const DWORD numFunctions = sizeof(scripts) / sizeof(CompiledScript);

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	QBKeyHeader* header = NULL;
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		//CreateConsole();


		DWORD old;
		VirtualProtect((void*)0x0042B247, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
		VirtualProtect((void*)0x0042B250, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
		VirtualProtect((void*)0x0042B2C1, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
		VirtualProtect((void*)0x0042B1A4, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
		VirtualProtect((void*)0x0042B1CF, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
		VirtualProtect((void*)0x008A8B4D, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);
		VirtualProtect((void*)0x0042B183, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);

		VirtualProtect((void*)0x0042B2B8, sizeof(BYTE), PAGE_EXECUTE_READWRITE, &old);
		VirtualProtect((void*)0x0042B23E, sizeof(BYTE), PAGE_EXECUTE_READWRITE, &old);



		VirtualProtect((void*)0x00426123, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);//othertable
		VirtualProtect((void*)0x0042635A, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);//othertable
		VirtualProtect((void*)0x0042B91C, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);//othertable
		VirtualProtect((void*)0x0042C231, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);//othertable
		VirtualProtect((void*)0x00426353, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);//othertable
		VirtualProtect((void*)0x0042B923, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);//othertable
		VirtualProtect((void*)0x0042C0F2, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);//othertable
		VirtualProtect((void*)0x0042C10D, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);//othertable
		VirtualProtect((void*)0x0042C24D, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);//othertable
		/*VirtualProtect((void*)0x004282B1, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);//othertable
		VirtualProtect((void*)0x004282BD, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &old);//othertable*/

		VirtualProtect((void*)0x0042611A, sizeof(BYTE), PAGE_EXECUTE_READWRITE, &old);
		VirtualProtect((void*)0x0042634A, sizeof(BYTE), PAGE_EXECUTE_READWRITE, &old);
		VirtualProtect((void*)0x0042B913, sizeof(BYTE), PAGE_EXECUTE_READWRITE, &old);

		for (DWORD i = 0; i < OLD_HASH_SIZE; i += 4)
		{

			*(DWORD*)(pNew + i) = *(DWORD*)(pOld + i);

		}

		/*for (DWORD i = OLD_HASH_SIZE; i < HASH_SIZE; i += 4)
		{
			*(DWORD*)(pNew + i) = 0;
		}*/

		//MessageBox(0, "", "", 0);
		*(DWORD*)0x0042B247 = (DWORD)&hashTable;
		*(DWORD*)0x0042B250 = (DWORD)&hashTable;
		*(DWORD*)0x0042B2C1 = (DWORD)&hashTable;
		*(DWORD*)0x0042B1A4 = (DWORD)&hashTable + 8;
		*(DWORD*)0x0042B1CF = (DWORD)&hashTable + HASH_SIZE + 8;
		*(DWORD*)0x008A8B4D = (DWORD)&hashTable + 4;
		*(DWORD*)0x0042B183 = (DWORD)&hashTable + HASH_SIZE + 4;
		*(BYTE*)0x0042B2B8 = 0x3F;
		*(BYTE*)0x0042B23E = 0x3F;


		/*for (DWORD i = 0; i < OTHER_SIZE; i += 4)
		{
			*(DWORD*)(pNew2 + i) = 0;
		}*/

		for (DWORD i = 0; i < OLD_OTHER_SIZE; i += 4)
		{
			*(DWORD*)(pNew2 + i) = *(DWORD*)(pOld2 + i);
		}


		/*for (DWORD i = 0; i < OLD_MAX_TRIGGERS; i++)
		{
			DWORD q = oldTriggers[i].qbKey;
			q &= 0x0000FFFF;
			q = q * 4 + q;

			TriggerScript* t = (TriggerScript*)(q*4 + pNew2);
			if((BYTE)t->test!=2)
			{
				t->qbKey = oldTriggers[i].qbKey;
				t->unk1 = oldTriggers[i].unk1;
				t->test = oldTriggers[i].test;
				//t->padding = oldTriggers[i].padding;
				t->script = oldTriggers[i].script;
				t->link = oldTriggers[i].link;

			}
			else
			{
				while (t->qbKey)
				{
					t++;
				}
				t->qbKey = oldTriggers[i].qbKey;
				t->unk1 = oldTriggers[i].unk1;
				t->test = oldTriggers[i].test;
				//t->padding = oldTriggers[i].padding;
				t->script = oldTriggers[i].script;
				t->link = oldTriggers[i].link;
			}
		}*/

		* (DWORD*)0x00426123 = (DWORD)&triggers;
		*(DWORD*)0x0042635A = (DWORD)&triggers;
		*(DWORD*)0x0042B91C = (DWORD)&triggers;
		*(DWORD*)0x0042C231 = (DWORD)&triggers;
		*(DWORD*)0x0042612A = (DWORD)&triggers + 8;
		*(DWORD*)0x00426353 = (DWORD)&triggers + 8;
		*(DWORD*)0x0042B923 = (DWORD)&triggers + 8;
		*(DWORD*)0x0042C0F2 = (DWORD)&triggers + 16;
		*(DWORD*)0x0042C10D = (DWORD)&triggers + OTHER_SIZE + 0x10;

		*(DWORD*)0x0042C24D = (DWORD)&triggers + OTHER_SIZE;

		/**(DWORD*)0x004282B1 = (DWORD)&otherTable + OTHER_SIZE;
		*(DWORD*)0x004282BD = (DWORD)&otherTable + OTHER_SIZE;*/
		/**(BYTE*)0x0042611A = 0xFF;
		*(BYTE*)0x0042634A = 0xFF;
		*(BYTE*)0x0042B913 = 0xFF;*/



		//FreeConsole();
		//MessageBox(0, "qbTable generated", "", MB_OK);
		pHookedPresent = NULL;
		for (int i = 0; i < numFunctions; i++)
		{
			header = AddQBKeyHeader(Checksum(scripts[i].name), 1);
			if (header)
			{
				header->type = QBKeyHeader::COMPILED_FUNCTION;
				header->pFunction = scripts[i].pFunction;
			}
			else
				MessageBox(0, "couldn't add script", scripts[i].name, 0);
		}

		header = GetQBKeyHeader(Checksum("DumpScripts"));
		if (header)
		{
			header->pFunction = DumpScripts;
		}

		//MessageBox(0, "going to fix msg", "going to fix msg", MB_OK);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)FixMessage, 0, 0, 0);

		HookFunc(0x004788C5, ReadChatHook, 1);
		HookFunc(0x004E0DF2, AddMessageHook, 2);

		InitSkater();
		FixChat();
		FixRenderBugs();
		FixVertBugs();
		FixClampBugs();
		FixQBParsing();

		FixTagLimitProtections();

		VirtualProtect((void*)0x004A7CAC, sizeof(BYTE), PAGE_EXECUTE_READWRITE, &old);
		VirtualProtect((void*)0x00478983, sizeof(BYTE), PAGE_EXECUTE_READWRITE, &old);

		FILE* f;
		f = fopen("settings", "r+b");
		BYTE tagLimit;
		tagLimit = 32;

		if (f)
		{
			printf("Loading settings...\n");
			fread(&LevelModSettings::UseNewMenu, 1, 1, f);
			fread(&LevelModSettings::FixSound, 1, 1, f);
			fread(&LevelModSettings::TeleFix, 1, 1, f);
			if (fread(&tagLimit, 1, 1, f) != 1)
				fwrite(&tagLimit, 1, 1, f);
			bool disableGrass = false;
			if (fread(&disableGrass, 1, 1, f) != 1)
				fwrite(&disableGrass, 1, 1, f);
			else
			{
				QBKeyHeader* header = GetQBKeyHeader(Checksum("GrassLayersDisabled"));
				if (header)
					header->value.i = disableGrass;
			}
			bool slapOn = true;
			if (fread(&slapOn, 1, 1, f) != 1)
				fwrite(&slapOn, 1, 1, f);
			else if (!slapOn)
			{
				QBKeyHeader* header = GetQBKeyHeader(Checksum("InternetClientCollRadius"));
				if (header)
					header->value.i = 0;
				header = GetQBKeyHeader(Checksum("InternetServerCollRadius"));
				if (header)
					header->value.i = 0;
				header = GetQBKeyHeader(Checksum("SlapIsOn"));
				if (header)
					header->value.i = 0;
			}
			bool hudOn = true;
			if (fread(&hudOn, 1, 1, f) != 1)
				fwrite(&hudOn, 1, 1, f);
			else if (!hudOn)
			{
				QBKeyHeader* header = GetQBKeyHeader(Checksum("HudIsOn"));
				if (header)
					header->value.i = 0;
			}
			if (fread(&version, 4, 1, f) != 1)
			{
				printf("version not found\n");
				version = VERSION;
				fwrite(&version, 4, 1, f);
			}
			printf("Setting Version %.2f\nLevelMod Version %.2f", version, VERSION);
			//
			if (version != VERSION)
			{
				printf("version not same\n");
				fseek(f, -4, SEEK_CUR);
				version = VERSION;
				fwrite(&version, 4, 1, f);
				debugMode = false;
				fwrite(&debugMode, 1, 1, f);
				SpineButton = 7;
				fwrite(&SpineButton, 1, 1, f);

				header = GetQBKeyHeader(Checksum("spine_button_text"));
				if (header)
					memcpy(header->pStr, "Revert", 7);
			}
			else
			{
				if (fread(&debugMode, 1, 1, f) != 1)
					fwrite(&debugMode, 1, 1, f);
				else if (debugMode)
				{

					CreateConsole();
					qbTable = new Script();
					QBKeyHeader* header = GetQBKeyHeader(Checksum("bShowConsole"));
					if (header)
					{
						header->value.i = 1;
						//MessageBox(0, "ShowCOnolse", "", MB_OK);

					}
					header = GetQBKeyHeader(Checksum("bPrintDebug"));
					if (header)
					{
						header->value.i = 1;
						//MessageBox(0, "HookDebug", "", MB_OK);
						HookDebugMessages(NULL, NULL);
					}

				}
				if (fread(&SpineButton, 1, 1, f) != 1)
				{
					SpineButton = 7;
					fwrite(&SpineButton, 1, 1, f);
				}

				header = GetQBKeyHeader(Checksum("spine_button_text"));
				if (header)
				{
					switch (SpineButton)
					{
					case KeyState::REVERT:
						memcpy(header->pStr, "Revert", 7);
						break;
					case KeyState::NOLLIE:
						SpineButton = KeyState::NOLLIE;
						memcpy(header->pStr, "Nollie", 7);
						break;
					case KeyState::SPINLEFT:
						SpineButton = KeyState::SPINLEFT;
						memcpy(header->pStr, "Left Spin Button", 17);
						break;
					case KeyState::SPINRIGHT:
						SpineButton = KeyState::SPINRIGHT;
						memcpy(header->pStr, "Right Spin Button", 18);
						break;
					default:
						SpineButton = KeyState::REVERT;
						printf("Trashed LevelMod Settings file...\n");
						memcpy(header->pStr, "Revert", 7);
						break;
					}
				}
			}
			version = VERSION;
		}
		else
		{
			printf("Writing settings...\n");
			f = fopen("settings", "w+b");
			if (f)
			{
				fwrite(&LevelModSettings::UseNewMenu, 1, 1, f);
				fwrite(&LevelModSettings::FixSound, 1, 1, f);
				fwrite(&LevelModSettings::TeleFix, 1, 1, f);
				fwrite(&tagLimit, 1, 1, f);
				bool disableGrass = false;
				fwrite(&disableGrass, 1, 1, f);
				bool slapOn = true;
				fwrite(&slapOn, 1, 1, f);
				bool hudOn = true;
				fwrite(&hudOn, 1, 1, f);
				version = VERSION;
				fwrite(&version, 4, 1, f);
				debugMode = false;
				fwrite(&debugMode, 1, 1, f);
				SpineButton = 7;
				fwrite(&SpineButton, 1, 1, f);
				header = GetQBKeyHeader(Checksum("spine_button_text"));
				if (header)
					memcpy(header->pStr, "Revert", 7);
				//fwrite(&newSize, 4, 1, f);
				fclose(f);
			}
		}
		_fcloseall();

		FixSpineFlag();

		if (LevelModSettings::UseNewMenu)
		{
			if (VirtualProtect((LPVOID)0x004404CE, sizeof(LevelModSettings::NewMenu), PAGE_EXECUTE_READWRITE, &old))
				memcpy((void*)0x004404CE, &LevelModSettings::NewMenu, sizeof(LevelModSettings::NewMenu));
			else
				MessageBox(0, "couldn't fix protection for", "NewMenu", 0);
		}
		if (LevelModSettings::TeleFix)
			LevelModSettings::FixTele();
		if (LevelModSettings::FixSound)
		{
			if (VirtualProtect((LPVOID)0x004C665D, 1, PAGE_EXECUTE_READWRITE, &old))
				*(BYTE*)0x004C665D = 0xEB;
			else
				MessageBox(0, "couldn't fix protection for", "SFXFix", 0);
		}
		if (tagLimit != 32)
		{
			CStruct pStruct;//= new CStruct;
			CStructHeader head;
			pStruct.head = &head;//new CStructHeader;
			pStruct.tail = pStruct.head;
			//oldLimit = tagLimit;
			pStruct.head->Data = tagLimit;
			pStruct.head->Type = QBKeyHeader::QBKeyType::INT;
			pStruct.head->QBkey = 0;
			SetTagLimit(&pStruct, NULL);
			//delete pStruct.head;
			LevelModSettings::UnlimitedGraf = true;
		}



		//MessageBox(0, "going to hook", "going to hook", MB_OK);
		PresentHook = Hook(15, HookedPresent, 1);//CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)D3D8BASE, NULL, NULL, NULL);
		pPresent = (Present_t)PresentHook.func;

		//HookFopen();
		/*if (HookFopen())
			MessageBox(0, "sucess", "", MB_OK);
		else
			MessageBox(0, "fail", "", MB_OK);*/
			//CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Test, 0, 0, 0);
			//MessageBox(0, "DOne", "", MB_OK);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		/*MessageBox(0, "", "", MB_OK);
		delete qbTable;
		FreeConsole();*/
		break;
	}
	return TRUE;
}


inline void SendChatMsg(char* text)
{
	DEBUGSTART();
	{
		int Param3;

		typedef int(__cdecl* pFixStackAddress)(BYTE);
		pFixStackAddress pFixStack = (pFixStackAddress)(0x00471C60);
		Param3 = pFixStack(0);

		typedef void(__cdecl* pSendChatMsgAddress)(char* text, int Param2, int Param3);
		pSendChatMsgAddress pSendChatMsg = (pSendChatMsgAddress)(0x004788B0);

		_asm mov Param3, eax//Random bugfix 0x0058D668
		pSendChatMsg(text, 0x0058D668, Param3);
	}
	DEBUGEND();
}



void SetStructValues(CStructHeader* pStruct, CStructHeader* values)
{
	pStruct->SetValues(values);
}

void SetArrayValues(CArray* pArray, CStructHeader* values)
{
	pArray->SetValues(values);
}

bool InvalidReadPtr(const void* const __restrict ptr, const DWORD size)
{
	MEMORY_BASIC_INFORMATION mbi;
	if ((VirtualQuery(ptr, &mbi, sizeof(mbi)) == 0) || (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS)))
		return true;
	else if ((mbi.State & MEM_COMMIT) && (((DWORD)((BYTE*)ptr + size)) <= ((DWORD)((BYTE*)mbi.BaseAddress + mbi.RegionSize))))
		return ((mbi.Protect & PAGE_READABLE) == 0);
	else
		return true;
}


bool InvalidReadPtr(const void* const __restrict ptr)
{
	MEMORY_BASIC_INFORMATION mbi;
	if ((VirtualQuery(ptr, &mbi, sizeof(mbi)) == 0) || (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS)))
		return true;
	else if (mbi.State & MEM_COMMIT)
		return ((mbi.Protect & PAGE_READABLE) == 0);
	else
		return true;
}


DWORD GetTagCount()
{
	DWORD* pTags = *(DWORD**)0x0058C300;
	if (pTags == 0 || *pTags == 0)
		return 0;
	return *(DWORD*)((BYTE*)pTags + 0x80);
}


EXTERN QBKeyHeader* GetQBKeyHeader(unsigned long QBKey)
{
	int ShortedQBKey = QBKey & 0x00000FFF;//last 12 bits of QBKey
	int* LoadAddress = (int*)(ShortedQBKey + ShortedQBKey * 4);//get index
	QBKeyHeader* header = (QBKeyHeader*)((int)LoadAddress * 4 + 0x0087D8F8);//0x0087D8F8 = start of main qbTable

	while (header->QBKeyCheck)//loop until end of table or corect header is found
	{
		if (((header->QBKeyCheck << 0x0C) | ShortedQBKey) == QBKey)
			return header;//found header lets return it
		header++;
	}

	typedef QBKeyHeader* (__cdecl* const GetQBKeyHeaderFunc)(const unsigned long QBKey);
	return ((GetQBKeyHeaderFunc)(0x00426340))(QBKey);//didn't find header lets let game search through sub qbTables
}
