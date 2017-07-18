#pragma once
#include "zchess.h"

//�Ƿ��һ����
static int bReset = 1;

// ��ת����
inline int SQUARE_FLIP(int sq) 
{
	return 254 - sq;
}


// ����������ͺ������ø���
inline int COORD_XY(int x, int y)
{
	return x + (y << 4);
}

// ������ˮƽ����
inline int FILE_FLIP(int x) 
{
	return 14 - x;
}

// ��ø��ӵ�������
inline int FILE_X(int sq) 
{
	return sq & 15;
}

// ��ø��ӵĺ�����
inline int RANK_Y(int sq) 
{
	return sq >> 4;
}

// ����ˮƽ����
inline int MIRROR_SQUARE(int sq) 
{
	return COORD_XY(FILE_FLIP(FILE_X(sq)), RANK_Y(sq));
}

// "qsort"����ʷ������ıȽϺ���
static int CompareHistory(const void *lpmv1, const void *lpmv2) 
{
	return Search.nHistoryTable[*(int *) lpmv2] - Search.nHistoryTable[*(int *) lpmv1];
}

// �����괹ֱ����
inline int RANK_FLIP(int y) 
{
	return 15 - y;
}

// ����ˮƽ����
inline int SQUARE_FORWARD(int sq, int sd) 
{
	return sq - 16 + (sd << 5);
}

// �߷��Ƿ����˧(��)�Ĳ���
inline BOOL KING_SPAN(int sqSrc, int sqDst) 
{
	return ccLegalSpan[sqDst - sqSrc + 256] == 1;
}

// �߷��Ƿ������(ʿ)�Ĳ���
inline BOOL ADVISOR_SPAN(int sqSrc, int sqDst) 
{
	return ccLegalSpan[sqDst - sqSrc + 256] == 2;
}

// �߷��Ƿ������(��)�Ĳ���
inline BOOL BISHOP_SPAN(int sqSrc, int sqDst) 
{
	return ccLegalSpan[sqDst - sqSrc + 256] == 3;
}

// ��(��)�۵�λ��
inline int BISHOP_PIN(int sqSrc, int sqDst) 
{
	return (sqSrc + sqDst) >> 1;
}

// ���ȵ�λ��
inline int KNIGHT_PIN(int sqSrc, int sqDst) 
{
	return sqSrc + ccKnightPin[sqDst - sqSrc + 256];
}

// �Ƿ�δ����
inline BOOL HOME_HALF(int sq, int sd) 
{
	return (sq & 0x80) != (sd << 7);
}

// �Ƿ��ѹ���
inline BOOL AWAY_HALF(int sq, int sd) 
{
	return (sq & 0x80) == (sd << 7);
}

// �Ƿ��ںӵ�ͬһ��
inline BOOL SAME_HALF(int sqSrc, int sqDst) 
{
	return ((sqSrc ^ sqDst) & 0x80) == 0;
}

// �Ƿ���ͬһ��
inline BOOL SAME_RANK(int sqSrc, int sqDst) 
{
	return ((sqSrc ^ sqDst) & 0xf0) == 0;
}

// �Ƿ���ͬһ��
inline BOOL SAME_FILE(int sqSrc, int sqDst) 
{
	return ((sqSrc ^ sqDst) & 0x0f) == 0;
}

// ��ú�ڱ��(������8��������16)
inline int SIDE_TAG(int sd) 
{
	return 8 + (sd << 3);
}

// ��öԷ���ڱ��
inline int OPP_SIDE_TAG(int sd) 
{
	return 16 - (sd << 3);
}


// �ж������Ƿ���������
inline BOOL IN_BOARD(int sq) 
{
	return ccInBoard[sq] != 0;
}

// �ж������Ƿ��ھŹ���
inline BOOL IN_FORT(int sq) 
{
	return ccInFort[sq] != 0;
}

// ���������յ����߷�
inline int MOVE(int sqSrc, int sqDst) {
	return sqSrc + sqDst * 256;
}

// �߷�ˮƽ����
inline int MIRROR_MOVE(int mv) 
{
	return MOVE(MIRROR_SQUARE(SRC(mv)), MIRROR_SQUARE(DST(mv)));
}

// ������Դ����
inline void PlayResWav(int nResId) 
{
	PlaySound(MAKEINTRESOURCE(nResId), Xqwl.hInst, SND_ASYNC | SND_NOWAIT | SND_RESOURCE);
}
