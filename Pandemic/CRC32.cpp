#include "CRC32.h"

// This code is based on: https://github.com/madler/zlib/blob/master/crc32.c
#define CRC_POLY		0xEDB88320ul
#define CRC_STARTDATA	0xFFFFFFFFul

// The lookup table
uint32_t CRC32::table[256] = { 0 };

void CRC32::Initialize()
{
	uint32_t i, j, crc;

	for(i = 0; i < 256; i++)
	{
		crc = i;

		for(j = 0; j < 8; j++)
		{
			if(crc & 0x00000001L) crc = (crc >> 1) ^ CRC_POLY;
			else crc = crc >> 1;
		}

		table[i] = crc;
	}
}

CRC32::CRC32() :
	crc(CRC_STARTDATA)
{
}

uint32_t CRC32::CCalculate(const unsigned char* data, unsigned long length)
{
	const unsigned char* ptr = data;
	if(ptr != nullptr)
	{
		for(unsigned long a = 0; a < length; a++)
		{
			crc = (crc >> 8) ^ table[(crc ^ (uint32_t)*ptr++) & 0x000000FFul];
		}
	}

	return (crc ^ 0xFFFFFFFFul);
}
