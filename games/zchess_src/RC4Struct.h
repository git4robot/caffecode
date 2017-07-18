#pragma once
// RC4密码流生成器
struct RC4Struct 
{
	BYTE s[256];
	int x, y;

	// 用空密钥初始化密码流生成器
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

	// 生成密码流的下一个字节
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

	// 生成密码流的下四个字节
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