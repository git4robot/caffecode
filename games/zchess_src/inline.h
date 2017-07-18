#pragma once
#include "zchess.h"

//是否悔一步棋
static int bReset = 1;

// 翻转格子
inline int SQUARE_FLIP(int sq) 
{
	return 254 - sq;
}


// 根据纵坐标和横坐标获得格子
inline int COORD_XY(int x, int y)
{
	return x + (y << 4);
}

// 纵坐标水平镜像
inline int FILE_FLIP(int x) 
{
	return 14 - x;
}

// 获得格子的纵坐标
inline int FILE_X(int sq) 
{
	return sq & 15;
}

// 获得格子的横坐标
inline int RANK_Y(int sq) 
{
	return sq >> 4;
}

// 格子水平镜像
inline int MIRROR_SQUARE(int sq) 
{
	return COORD_XY(FILE_FLIP(FILE_X(sq)), RANK_Y(sq));
}

// "qsort"按历史表排序的比较函数
static int CompareHistory(const void *lpmv1, const void *lpmv2) 
{
	return Search.nHistoryTable[*(int *) lpmv2] - Search.nHistoryTable[*(int *) lpmv1];
}

// 横坐标垂直镜像
inline int RANK_FLIP(int y) 
{
	return 15 - y;
}

// 格子水平镜像
inline int SQUARE_FORWARD(int sq, int sd) 
{
	return sq - 16 + (sd << 5);
}

// 走法是否符合帅(将)的步长
inline BOOL KING_SPAN(int sqSrc, int sqDst) 
{
	return ccLegalSpan[sqDst - sqSrc + 256] == 1;
}

// 走法是否符合仕(士)的步长
inline BOOL ADVISOR_SPAN(int sqSrc, int sqDst) 
{
	return ccLegalSpan[sqDst - sqSrc + 256] == 2;
}

// 走法是否符合相(象)的步长
inline BOOL BISHOP_SPAN(int sqSrc, int sqDst) 
{
	return ccLegalSpan[sqDst - sqSrc + 256] == 3;
}

// 相(象)眼的位置
inline int BISHOP_PIN(int sqSrc, int sqDst) 
{
	return (sqSrc + sqDst) >> 1;
}

// 马腿的位置
inline int KNIGHT_PIN(int sqSrc, int sqDst) 
{
	return sqSrc + ccKnightPin[sqDst - sqSrc + 256];
}

// 是否未过河
inline BOOL HOME_HALF(int sq, int sd) 
{
	return (sq & 0x80) != (sd << 7);
}

// 是否已过河
inline BOOL AWAY_HALF(int sq, int sd) 
{
	return (sq & 0x80) == (sd << 7);
}

// 是否在河的同一边
inline BOOL SAME_HALF(int sqSrc, int sqDst) 
{
	return ((sqSrc ^ sqDst) & 0x80) == 0;
}

// 是否在同一行
inline BOOL SAME_RANK(int sqSrc, int sqDst) 
{
	return ((sqSrc ^ sqDst) & 0xf0) == 0;
}

// 是否在同一列
inline BOOL SAME_FILE(int sqSrc, int sqDst) 
{
	return ((sqSrc ^ sqDst) & 0x0f) == 0;
}

// 获得红黑标记(红子是8，黑子是16)
inline int SIDE_TAG(int sd) 
{
	return 8 + (sd << 3);
}

// 获得对方红黑标记
inline int OPP_SIDE_TAG(int sd) 
{
	return 16 - (sd << 3);
}


// 判断棋子是否在棋盘中
inline BOOL IN_BOARD(int sq) 
{
	return ccInBoard[sq] != 0;
}

// 判断棋子是否在九宫中
inline BOOL IN_FORT(int sq) 
{
	return ccInFort[sq] != 0;
}

// 根据起点和终点获得走法
inline int MOVE(int sqSrc, int sqDst) {
	return sqSrc + sqDst * 256;
}

// 走法水平镜像
inline int MIRROR_MOVE(int mv) 
{
	return MOVE(MIRROR_SQUARE(SRC(mv)), MIRROR_SQUARE(DST(mv)));
}

// 播放资源声音
inline void PlayResWav(int nResId) 
{
	PlaySound(MAKEINTRESOURCE(nResId), Xqwl.hInst, SND_ASYNC | SND_NOWAIT | SND_RESOURCE);
}
