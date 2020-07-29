#pragma once
#ifndef DEFINES_H
#define DEFINES_H
#include <winsock2.h>
#include "windows.h"
#include "vector"
#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <fenv.h>
#include <string.h>
#include <d3dx9.h>
#include <d3dx9core.h>
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
  _printf( _T("Parameter ") _T(#p) \
  _T(" is not a valid read pointer\r\n") \
  _T("CFUNC ")  __FUNCTION__ _T("\r\n")); return NULL; \
} \
}

//where p = pointer to data s = size of data
#define VALIDATE_DATA(p, s) \
{ \
if (InvalidReadPtr(p, s)) \
{ \
  _printf( _T("Parameter ") _T(#p) \
  _T(" is not a valid read area\r\n") \
  _T("CFUNC ") __FUNCTION__ _T("\r\n")); return NULL; \
} \
}

void _printf(const char* str, ...)
{
//#if 1
	va_list va_alist;
	va_start(va_alist, str);
	char logbuf[512] = { 0 };
	_vsnprintf(logbuf + strlen(logbuf), sizeof(logbuf) - strlen(logbuf), str, va_alist);
	va_end(va_alist);
	printf(logbuf);
	//printf(str, ...);
//#endif
}
#endif