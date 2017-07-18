#pragma once
#include "RC4Struct.h"
// Zobrist�ṹ
struct ZobristStruct 
{
	DWORD dwKey, dwLock0, dwLock1;

	// �������Zobrist
	void InitZero(void) 
	{                 
		dwKey = dwLock0 = dwLock1 = 0;
	}


	// �����������Zobrist
	void InitRC4(RC4Struct &rc4) 
	{ 
		dwKey = rc4.NextLong();
		dwLock0 = rc4.NextLong();
		dwLock1 = rc4.NextLong();
	}

	// ִ��XOR����
	void Xor(const ZobristStruct &zobr) 
	{ 
		dwKey ^= zobr.dwKey;
		dwLock0 ^= zobr.dwLock0;
		dwLock1 ^= zobr.dwLock1;
	}

	void Xor(const ZobristStruct &zobr1, const ZobristStruct &zobr2) 
	{
		dwKey ^= zobr1.dwKey ^ zobr2.dwKey;
		dwLock0 ^= zobr1.dwLock0 ^ zobr2.dwLock0;
		dwLock1 ^= zobr1.dwLock1 ^ zobr2.dwLock1;
	}
};

