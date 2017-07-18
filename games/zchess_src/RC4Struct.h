#pragma once
// RC4������������
struct RC4Struct 
{
	BYTE s[256];
	int x, y;

	// �ÿ���Կ��ʼ��������������
	void InitZero(void) 
	{
		int i, j;
		BYTE uc;

		x = y = j = 0;
		for (i = 0; i < 256; i ++) 
		{
			s[i] = i;
		}
		for (i = 0; i < 256; i ++) 
		{
			j = (j + s[i]) & 255;
			uc = s[i];
			s[i] = s[j];
			s[j] = uc;
		}
	}

	// ��������������һ���ֽ�
	BYTE NextByte(void) 
	{  
		BYTE uc;
		x = (x + 1) & 255;
		y = (y + s[x]) & 255;
		uc = s[x];
		s[x] = s[y];
		s[y] = uc;
		return s[(s[x] + s[y]) & 255];
	}

	// ���������������ĸ��ֽ�
	DWORD NextLong(void) 
	{ 
		BYTE uc0, uc1, uc2, uc3;
		uc0 = NextByte();
		uc1 = NextByte();
		uc2 = NextByte();
		uc3 = NextByte();
		return uc0 + (uc1 << 8) + (uc2 << 16) + (uc3 << 24);
	}
};