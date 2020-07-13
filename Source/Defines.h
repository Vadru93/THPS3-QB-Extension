#pragma once
#include "windows.h"
#include "vector"
#include <d3dx8.h>
#include <d3dx8core.h>
#include <map>

#ifdef EXPORTING
#define EXTERN __declspec(dllexport)
#else
#define EXTERN __declspec(dllimport)
#pragma comment(lib, "LevelMod.lib")
#endif

bool EXTERN InvalidReadPtr(const void* const __restrict ptr, const DWORD size);
bool EXTERN InvalidReadPtr(const void* const __restrict ptr);

enum enum_matrix
{
	RIGHT=0,
	UP,
	AT,
	POS,
};

enum enum_vertex
{
	X=0,
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