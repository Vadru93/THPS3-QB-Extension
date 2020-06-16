#pragma once
#include "Defines.h"

EXTERN char* FindChecksumName(DWORD checksum);






inline unsigned long Checksum(const char* string)//THPS3 Function for CRC32
{
	unsigned long checksum;

	typedef unsigned long(__cdecl* pCRC32Address)(const char* string);
	pCRC32Address pCRC32 = (pCRC32Address)(0x004265F0);
	checksum = pCRC32(string);

	return checksum;
}
