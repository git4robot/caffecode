/************************************************
 * Copyright (C) 2008-2009 www.zivsoft.com
 *
 * 象棋目标：
 * 一、实现开局库；
 * 二、实现PVS(主要变例搜索)；
 * 三、把根节点的搜索单独处理，增加搜索的随机性；
 * 四、克服由长将引起的置换表的不稳定性。
 *
 ***********************************************/

#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "resource.h"
#include "PositionStruct.h"
#include "SortStruct.h"



// 初始化Zobrist表
static void InitZobrist(void) 
{
	int i, j;
	RC4Struct rc4;
	rc4.InitZero();

	Zobrist.Player.InitRC4(rc4);
	for (i = 0; i < 14; i ++) 
	{
		for (j = 0; j < 256; j ++) 
		{
			Zobrist.Table[i][j].InitRC4(rc4);
		}
	}
}


// 装入开局库
static void LoadBook(void) 
{
	HRSRC hrsrc = FindResource(Xqwl.hInst, "BOOK_DATA", RT_RCDATA);
	Search.nBookSize = SizeofResource(Xqwl.hInst, hrsrc) / sizeof(BookItem);
	if (Search.nBookSize > BOOK_SIZE) 
	{
		Search.nBookSize = BOOK_SIZE;
	}
	memcpy(Search.BookTable, LockResource(LoadResource(Xqwl.hInst, hrsrc)),	Search.nBookSize * sizeof(BookItem));
}


static int CompareBook(const void *lpbk1, const void *lpbk2) 
{
	DWORD dw1, dw2;
	dw1 = ((BookItem *) lpbk1)->dwLock;
	dw2 = ((BookItem *) lpbk2)->dwLock;
	return dw1 > dw2 ? 1 : dw1 < dw2 ? -1 : 0;
}


// 搜索开局库
static int SearchBook(void) 
{
	int i, vl, nBookMoves, mv;
	int mvs[MAX_GEN_MOVES], vls[MAX_GEN_MOVES];
	BOOL bMirror;
	BookItem bkToSearch, *lpbk;
	PositionStruct posMirror;
	// 搜索开局库的过程有以下几个步骤

	// 1. 如果没有开局库，则立即返回
	if (Search.nBookSize == 0) 
	{
		return 0;
	}
	// 2. 搜索当前局面
	bMirror = FALSE;
	bkToSearch.dwLock = pos.zobr.dwLock1;
	lpbk = (BookItem *) bsearch(&bkToSearch, Search.BookTable, Search.nBookSize, sizeof(BookItem), CompareBook);
	// 3. 如果没有找到，那么搜索当前局面的镜像局面
	if (lpbk == NULL) 
	{
		bMirror = TRUE;
		pos.Mirror(posMirror);
		bkToSearch.dwLock = posMirror.zobr.dwLock1;
		lpbk = (BookItem *) bsearch(&bkToSearch, Search.BookTable, Search.nBookSize, sizeof(BookItem), CompareBook);
	}
	// 4. 如果镜像局面也没找到，则立即返回
	if (lpbk == NULL)
	{
		return 0;
	}
	// 5. 如果找到，则向前查第一个开局库项
	while (lpbk >= Search.BookTable && lpbk->dwLock == bkToSearch.dwLock)
	{
		lpbk --;
	}
	lpbk ++;
	
	// 6. 把走法和分值写入到"mvs"和"vls"数组中
	vl = nBookMoves = 0;
	while (lpbk < Search.BookTable + Search.nBookSize && lpbk->dwLock == bkToSearch.dwLock)
	{
		mv = (bMirror ? MIRROR_MOVE(lpbk->wmv) : lpbk->wmv);
		if (pos.LegalMove(mv))
		{
			mvs[nBookMoves] = mv;
			vls[nBookMoves] = lpbk->wvl;
			vl += vls[nBookMoves];
			nBookMoves ++;
			if (nBookMoves == MAX_GEN_MOVES) 
			{
				break; // 防止"BOOK.DAT"中含有异常数据
			}
		}
		lpbk ++;
	}
	if (vl == 0)
	{
		return 0; // 防止"BOOK.DAT"中含有异常数据
	}
	
	// 7. 根据权重随机选择一个走法
	vl = rand() % vl;
	for (i = 0; i < nBookMoves; i ++) 
	{
		vl -= vls[i];
		if (vl < 0) 
		{
			break;
		}
	}
	return mvs[i];
}

// 提取置换表项
static int ProbeHash(int vlAlpha, int vlBeta, int nDepth, int &mv) 
{
	BOOL bMate; // 杀棋标志：如果是杀棋，那么不需要满足深度条件
	HashItem hsh;

	hsh = Search.HashTable[pos.zobr.dwKey & (HASH_SIZE - 1)];
	if (hsh.dwLock0 != pos.zobr.dwLock0 || hsh.dwLock1 != pos.zobr.dwLock1) 
	{
		mv = 0;
		return -MATE_VALUE;
	}
	mv = hsh.wmv;
	bMate = FALSE;
	if (hsh.svl > WIN_VALUE) 
	{
		if (hsh.svl < BAN_VALUE) 
		{
			return -MATE_VALUE; // 可能导致搜索的不稳定性，立刻退出，但最佳着法可能拿到
		}
		hsh.svl -= pos.nDistance;
		bMate = TRUE;
	} 
	else if (hsh.svl < -WIN_VALUE) 
	{
		if (hsh.svl > -BAN_VALUE) 
		{
			return -MATE_VALUE; // 同上
		}
		hsh.svl += pos.nDistance;
		bMate = TRUE;
	}
	if (hsh.ucDepth >= nDepth || bMate) 
	{
		if (hsh.ucFlag == HASH_BETA) 
		{
			return (hsh.svl >= vlBeta ? hsh.svl : -MATE_VALUE);
		}
		else if (hsh.ucFlag == HASH_ALPHA) 
		{
			return (hsh.svl <= vlAlpha ? hsh.svl : -MATE_VALUE);
		}
		return hsh.svl;
	}
	return -MATE_VALUE;
};

// 保存置换表项
static void RecordHash(int nFlag, int vl, int nDepth, int mv) 
{
	HashItem hsh;
	hsh = Search.HashTable[pos.zobr.dwKey & (HASH_SIZE - 1)];
	if (hsh.ucDepth > nDepth) 
	{
		return;
	}
	hsh.ucFlag = nFlag;
	hsh.ucDepth = nDepth;
	if (vl > WIN_VALUE) 
	{
		if (mv == 0 && vl <= BAN_VALUE) 
		{
			return; // 可能导致搜索的不稳定性，并且没有最佳着法，立刻退出
		}
		hsh.svl = vl + pos.nDistance;
	} 
	else if (vl < -WIN_VALUE) 
	{
		if (mv == 0 && vl >= -BAN_VALUE) 
		{
			return; // 同上
		}
		hsh.svl = vl - pos.nDistance;
	}
	else 
	{
		hsh.svl = vl;
	}
	hsh.wmv = mv;
	hsh.dwLock0 = pos.zobr.dwLock0;
	hsh.dwLock1 = pos.zobr.dwLock1;
	Search.HashTable[pos.zobr.dwKey & (HASH_SIZE - 1)] = hsh;
};

// MVV/LVA每种子力的价值
static BYTE cucMvvLva[24] = 
{
	0, 0, 0, 0, 0, 0, 0, 0,
	5, 1, 1, 3, 4, 3, 2, 0,
	5, 1, 1, 3, 4, 3, 2, 0
};

// 求MVV/LVA值
inline int MvvLva(int mv) 
{
	return (cucMvvLva[pos.ucpcSquares[DST(mv)]] << 3) - cucMvvLva[pos.ucpcSquares[SRC(mv)]];
}

// "qsort"按MVV/LVA值排序的比较函数
static int CompareMvvLva(const void *lpmv1, const void *lpmv2)
{
	return MvvLva(*(int *) lpmv2) - MvvLva(*(int *) lpmv1);
}




// 对最佳走法的处理
inline void SetBestMove(int mv, int nDepth) 
{
	int *lpmvKillers;
	Search.nHistoryTable[mv] += nDepth * nDepth;
	lpmvKillers = Search.mvKillers[pos.nDistance];
	if (lpmvKillers[0] != mv) 
	{
		lpmvKillers[1] = lpmvKillers[0];
		lpmvKillers[0] = mv;
	}
}
// "GenerateMoves"参数
const BOOL GEN_CAPTURE = TRUE;

// 静态(Quiescence)搜索过程
static int SearchQuiesc(int vlAlpha, int vlBeta) 
{
	int i, nGenMoves;
	int vl, vlBest;
	int mvs[MAX_GEN_MOVES];
	// 一个静态搜索分为以下几个阶段

	// 1. 检查重复局面
	vl = pos.RepStatus();
	if (vl != 0) {
		return pos.RepValue(vl);
	}

	// 2. 到达极限深度就返回局面评价
	if (pos.nDistance == LIMIT_DEPTH) {
		return pos.Evaluate();
	}

	// 3. 初始化最佳值
	vlBest = -MATE_VALUE; // 这样可以知道，是否一个走法都没走过(杀棋)

	if (pos.InCheck()) {
		// 4. 如果被将军，则生成全部走法
		nGenMoves = pos.GenerateMoves(mvs);
		qsort(mvs, nGenMoves, sizeof(int), CompareHistory);
	} 
	else 
	{
		// 5. 如果不被将军，先做局面评价
		vl = pos.Evaluate();
		if (vl > vlBest) 
		{
			vlBest = vl;
			if (vl >= vlBeta) 
			{
				return vl;
			}
			if (vl > vlAlpha) 
			{
				vlAlpha = vl;
			}
		}

		// 6. 如果局面评价没有截断，再生成吃子走法
		nGenMoves = pos.GenerateMoves(mvs, GEN_CAPTURE);
		qsort(mvs, nGenMoves, sizeof(int), CompareMvvLva);
	}

	// 7. 逐一走这些走法，并进行递归
	for (i = 0; i < nGenMoves; i ++) 
	{
		if (pos.MakeMove(mvs[i])) 
		{
			vl = -SearchQuiesc(-vlBeta, -vlAlpha);
			pos.UndoMakeMove();

			// 8. 进行Alpha-Beta大小判断和截断
			if (vl > vlBest) 
			{    // 找到最佳值(但不能确定是Alpha、PV还是Beta走法)
				vlBest = vl;        // "vlBest"就是目前要返回的最佳值，可能超出Alpha-Beta边界
				if (vl >= vlBeta) 
				{ // 找到一个Beta走法
					return vl;        // Beta截断
				}
				if (vl > vlAlpha) 
				{ // 找到一个PV走法
					vlAlpha = vl;     // 缩小Alpha-Beta边界
				}
			}
		}
	}

	// 9. 所有走法都搜索完了，返回最佳值
	return vlBest == -MATE_VALUE ? pos.nDistance - MATE_VALUE : vlBest;
}

// 超出边界(Fail-Soft)的Alpha-Beta搜索过程
static int SearchFull(int vlAlpha, int vlBeta, int nDepth, BOOL bNoNull = FALSE) 
{
	int nHashFlag, vl, vlBest;
	int mv, mvBest, mvHash, nNewDepth;
	SortStruct Sort;
	// 一个Alpha-Beta完全搜索分为以下几个阶段

	// 1. 到达水平线，则调用静态搜索(注意：由于空步裁剪，深度可能小于零)
	if (nDepth <= 0) 
	{
		return SearchQuiesc(vlAlpha, vlBeta);
	}

	// 1-1. 检查重复局面(注意：不要在根节点检查，否则就没有走法了)
	vl = pos.RepStatus();
	if (vl != 0) {
		return pos.RepValue(vl);
	}

	// 1-2. 到达极限深度就返回局面评价
	if (pos.nDistance == LIMIT_DEPTH) 
	{
		return pos.Evaluate();
	}

	// 1-3. 尝试置换表裁剪，并得到置换表走法
	vl = ProbeHash(vlAlpha, vlBeta, nDepth, mvHash);
	if (vl > -MATE_VALUE) {
		return vl;
	}

	// 1-4. 尝试空步裁剪(根节点的Beta值是"MATE_VALUE"，所以不可能发生空步裁剪)
	if (!bNoNull && !pos.InCheck() && pos.NullOkay()) 
	{
		pos.NullMove();
		vl = -SearchFull(-vlBeta, 1 - vlBeta, nDepth - NULL_DEPTH - 1, NO_NULL);
		pos.UndoNullMove();
		if (vl >= vlBeta) {
			return vl;
		}
	}

	// 2. 初始化最佳值和最佳走法
	nHashFlag = HASH_ALPHA;
	vlBest = -MATE_VALUE; // 这样可以知道，是否一个走法都没走过(杀棋)
	mvBest = 0;           // 这样可以知道，是否搜索到了Beta走法或PV走法，以便保存到历史表

	// 3. 初始化走法排序结构
	Sort.Init(mvHash);

	// 4. 逐一走这些走法，并进行递归
	while ((mv = Sort.Next()) != 0) 
	{
		if (pos.MakeMove(mv)) {
			// 将军延伸
			nNewDepth = pos.InCheck() ? nDepth : nDepth - 1;
			// PVS
			if (vlBest == -MATE_VALUE) {
				vl = -SearchFull(-vlBeta, -vlAlpha, nNewDepth);
			} 
			else 
			{
				vl = -SearchFull(-vlAlpha - 1, -vlAlpha, nNewDepth);
				if (vl > vlAlpha && vl < vlBeta) {
					vl = -SearchFull(-vlBeta, -vlAlpha, nNewDepth);
				}
			}
			pos.UndoMakeMove();

			// 5. 进行Alpha-Beta大小判断和截断
			if (vl > vlBest) {    // 找到最佳值(但不能确定是Alpha、PV还是Beta走法)
				vlBest = vl;        // "vlBest"就是目前要返回的最佳值，可能超出Alpha-Beta边界
				if (vl >= vlBeta) { // 找到一个Beta走法
					nHashFlag = HASH_BETA;
					mvBest = mv;      // Beta走法要保存到历史表
					break;            // Beta截断
				}
				if (vl > vlAlpha) { // 找到一个PV走法
					nHashFlag = HASH_PV;
					mvBest = mv;      // PV走法要保存到历史表
					vlAlpha = vl;     // 缩小Alpha-Beta边界
				}
			}
		}
	}

	// 5. 所有走法都搜索完了，把最佳走法(不能是Alpha走法)保存到历史表，返回最佳值
	if (vlBest == -MATE_VALUE) {
		// 如果是杀棋，就根据杀棋步数给出评价
		return pos.nDistance - MATE_VALUE;
	}
	// 记录到置换表
	RecordHash(nHashFlag, vlBest, nDepth, mvBest);
	if (mvBest != 0) {
		// 如果不是Alpha走法，就将最佳走法保存到历史表
		SetBestMove(mvBest, nDepth);
	}
	return vlBest;
}

// 根节点的Alpha-Beta搜索过程
static int SearchRoot(int nDepth) {
	int vl, vlBest, mv, nNewDepth;
	SortStruct Sort;

	vlBest = -MATE_VALUE;
	Sort.Init(Search.mvResult);
	while ((mv = Sort.Next()) != 0) {
		if (pos.MakeMove(mv)) {
			nNewDepth = pos.InCheck() ? nDepth : nDepth - 1;
			if (vlBest == -MATE_VALUE) {
				vl = -SearchFull(-MATE_VALUE, MATE_VALUE, nNewDepth, NO_NULL);
			} else {
				vl = -SearchFull(-vlBest - 1, -vlBest, nNewDepth);
				if (vl > vlBest) {
					vl = -SearchFull(-MATE_VALUE, -vlBest, nNewDepth, NO_NULL);
				}
			}
			pos.UndoMakeMove();
			if (vl > vlBest) {
				vlBest = vl;
				Search.mvResult = mv;
				if (vlBest > -WIN_VALUE && vlBest < WIN_VALUE) {
					vlBest += (rand() & RANDOM_MASK) - (rand() & RANDOM_MASK);
				}
			}
		}
	}
	RecordHash(HASH_PV, vlBest, nDepth, Search.mvResult);
	SetBestMove(Search.mvResult, nDepth);
	return vlBest;
}

// 迭代加深搜索过程
static void SearchMain(void) 
{
	int i, t, vl, nGenMoves;
	int mvs[MAX_GEN_MOVES];

	// 初始化
	memset(Search.nHistoryTable, 0, 65536 * sizeof(int));       // 清空历史表
	memset(Search.mvKillers, 0, LIMIT_DEPTH * 2 * sizeof(int)); // 清空杀手走法表
	memset(Search.HashTable, 0, HASH_SIZE * sizeof(HashItem));  // 清空置换表
	t = clock();       // 初始化定时器
	pos.nDistance = 0; // 初始步数

	// 搜索开局库
	Search.mvResult = SearchBook();
	if (Search.mvResult != 0) 
	{
		pos.MakeMove(Search.mvResult);
		if (pos.RepStatus(3) == 0) 
		{
			pos.UndoMakeMove();
			return;
		}
		pos.UndoMakeMove();
	}

	// 检查是否只有唯一走法
	vl = 0;
	nGenMoves = pos.GenerateMoves(mvs);
	for (i = 0; i < nGenMoves; i ++) 
	{
		if (pos.MakeMove(mvs[i])) 
		{
			pos.UndoMakeMove();
			Search.mvResult = mvs[i];
			vl ++;
		}
	}
	if (vl == 1) {
		return;
	}

	// 迭代加深过程
	for (i = 1; i <= LIMIT_DEPTH; i ++) 
	{
		vl = SearchRoot(i);
		// 搜索到杀棋，就终止搜索
		if (vl > WIN_VALUE || vl < -WIN_VALUE) 
		{
			break;
		}
		// 超过一秒，就终止搜索
		if (clock() - t > CLOCKS_PER_SEC) 
		{
			break;
		}
	}
}

// TransparentBlt 的替代函数，用来修正原函数在 Windows 98 下资源泄漏的问题
static void TransparentBlt2(HDC hdcDest, int nXOriginDest, int nYOriginDest, int nWidthDest, int nHeightDest,HDC hdcSrc, int nXOriginSrc, int nYOriginSrc, int nWidthSrc, int nHeightSrc, UINT crTransparent) 
{
		HDC hImageDC, hMaskDC;
		HBITMAP hOldImageBMP, hImageBMP, hOldMaskBMP, hMaskBMP;

		hImageBMP = CreateCompatibleBitmap(hdcDest, nWidthDest, nHeightDest);
		hMaskBMP = CreateBitmap(nWidthDest, nHeightDest, 1, 1, NULL);
		hImageDC = CreateCompatibleDC(hdcDest);
		hMaskDC = CreateCompatibleDC(hdcDest);
		hOldImageBMP = (HBITMAP) SelectObject(hImageDC, hImageBMP);
		hOldMaskBMP = (HBITMAP) SelectObject(hMaskDC, hMaskBMP);

		if (nWidthDest == nWidthSrc && nHeightDest == nHeightSrc) 
		{
			BitBlt(hImageDC, 0, 0, nWidthDest, nHeightDest,hdcSrc, nXOriginSrc, nYOriginSrc, SRCCOPY);
		} 
		else 
		{
			StretchBlt(hImageDC, 0, 0, nWidthDest, nHeightDest,hdcSrc, nXOriginSrc, nYOriginSrc, nWidthSrc, nHeightSrc, SRCCOPY);
		}
		SetBkColor(hImageDC, crTransparent);
		BitBlt(hMaskDC, 0, 0, nWidthDest, nHeightDest, hImageDC, 0, 0, SRCCOPY);
		SetBkColor(hImageDC, RGB(0,0,0));
		SetTextColor(hImageDC, RGB(255,255,255));
		BitBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, hMaskDC, 0, 0, SRCAND);
		SetBkColor(hdcDest, RGB(255,255,255));
		SetTextColor(hdcDest, RGB(0,0,0));
		BitBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest,hMaskDC, 0, 0, SRCAND);
		BitBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest,hImageDC, 0, 0, SRCPAINT);

		SelectObject(hImageDC, hOldImageBMP);
		DeleteDC(hImageDC);
		SelectObject(hMaskDC, hOldMaskBMP);
		DeleteDC(hMaskDC);
		DeleteObject(hImageBMP);
		DeleteObject(hMaskBMP);
}

// 绘制透明图片
static void DrawTransBmp(HDC hdc, HDC hdcTmp, int xx, int yy, HBITMAP bmp) 
{
	SelectObject(hdcTmp, bmp);
	TransparentBlt2(hdc, xx, yy, SQUARE_SIZE, SQUARE_SIZE, hdcTmp, 0, 0, SQUARE_SIZE, SQUARE_SIZE, MASK_COLOR);
}

// 绘制棋盘
static void DrawBoard(HDC hdc) 
{
	int x, y, xx, yy, sq, pc;
	HDC hdcTmp;

	// 画棋盘
	hdcTmp = CreateCompatibleDC(hdc);
	SelectObject(hdcTmp, Xqwl.bmpBoard);
	BitBlt(hdc, 0, 0, BOARD_WIDTH, BOARD_HEIGHT, hdcTmp, 0, 0, SRCCOPY);
	// 画棋子
	for (x = FILE_LEFT; x <= FILE_RIGHT; x ++) 
	{
		for (y = RANK_TOP; y <= RANK_BOTTOM; y ++) 
		{

			if (Xqwl.bFlipped) 
			{
				xx = BOARD_EDGE + (FILE_FLIP(x) - FILE_LEFT) * SQUARE_SIZE;
				yy = BOARD_EDGE + (RANK_FLIP(y) - RANK_TOP) * SQUARE_SIZE;
			} 
			else 
			{
				xx = BOARD_EDGE + (x - FILE_LEFT) * SQUARE_SIZE;
				yy = BOARD_EDGE + (y - RANK_TOP) * SQUARE_SIZE;
			}
			sq = COORD_XY(x, y);
			pc = pos.ucpcSquares[sq];
			if (pc != 0) 
			{
				DrawTransBmp(hdc, hdcTmp, xx, yy, Xqwl.bmpPieces[pc]);
			}
			if (sq == Xqwl.sqSelected || sq == SRC(Xqwl.mvLast) || sq == DST(Xqwl.mvLast)) 
			{
				DrawTransBmp(hdc, hdcTmp, xx, yy, Xqwl.bmpSelected);
			}
		}
	}
	DeleteDC(hdcTmp);
}





// "DrawSquare"参数
const BOOL DRAW_SELECTED = TRUE;

// 绘制格子
static void DrawSquare(int sq, BOOL bSelected = FALSE) 
{
	int sqFlipped, xx, yy, pc;

	sqFlipped = Xqwl.bFlipped ? SQUARE_FLIP(sq) : sq;
	xx = BOARD_EDGE + (FILE_X(sqFlipped) - FILE_LEFT) * SQUARE_SIZE;
	yy = BOARD_EDGE + (RANK_Y(sqFlipped) - RANK_TOP) * SQUARE_SIZE;
	SelectObject(Xqwl.hdcTmp, Xqwl.bmpBoard);
	BitBlt(Xqwl.hdc, xx, yy, SQUARE_SIZE, SQUARE_SIZE, Xqwl.hdcTmp, xx, yy, SRCCOPY);
	pc = pos.ucpcSquares[sq];
	if (pc != 0) {
		DrawTransBmp(Xqwl.hdc, Xqwl.hdcTmp, xx, yy, Xqwl.bmpPieces[pc]);
	}
	if (bSelected) {
		DrawTransBmp(Xqwl.hdc, Xqwl.hdcTmp, xx, yy, Xqwl.bmpSelected);
	}
}

// 电脑回应一步棋
static void ResponseMove(void) 
{
	int vlRep;
	// 电脑走一步棋
	SetCursor((HCURSOR) LoadImage(NULL, IDC_WAIT, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED));
	SearchMain();
	SetCursor((HCURSOR) LoadImage(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED));
	pos.MakeMove(Search.mvResult);
	// 清除上一步棋的选择标记
	DrawSquare(SRC(Xqwl.mvLast));
	DrawSquare(DST(Xqwl.mvLast));
	// 把电脑走的棋标记出来
	Xqwl.mvLast = Search.mvResult;
	DrawSquare(SRC(Xqwl.mvLast), DRAW_SELECTED);
	DrawSquare(DST(Xqwl.mvLast), DRAW_SELECTED);
	// 检查重复局面
	vlRep = pos.RepStatus(3);
	if (pos.IsMate()) 
	{
		// 如果分出胜负，那么播放胜负的声音，并且弹出不带声音的提示框
		PlayResWav(IDR_LOSS);
		MessageBoxMute("You are failed!\n");
		Xqwl.bGameOver = TRUE;
	} 
	else if (vlRep > 0) 
	{
		vlRep = pos.RepValue(vlRep);
		// 注意："vlRep"是对玩家来说的分值
		PlayResWav(vlRep < -WIN_VALUE ? IDR_LOSS : vlRep > WIN_VALUE ? IDR_WIN : IDR_DRAW);
		MessageBoxMute(vlRep < -WIN_VALUE ? "If you always kill, then we think you are failed." :
			vlRep > WIN_VALUE ? "The computer always kill, we think you win. :-)" : "We can not know who will win.");
		Xqwl.bGameOver = TRUE;
	} 
	else if (pos.nMoveNum > 100) 
	{
		PlayResWav(IDR_DRAW);
		MessageBoxMute("Game Over!");
		Xqwl.bGameOver = TRUE;
	} 
	else 
	{
		// 如果没有分出胜负，那么播放将军、吃子或一般走子的声音
		PlayResWav(pos.InCheck() ? IDR_CHECK2 : pos.Captured() ? IDR_CAPTURE2 : IDR_MOVE2);
		if (pos.Captured()) 
		{
			pos.SetIrrev();
		}
	}
}

// 点击格子事件处理
static void ClickSquare(int sq) 
{
	int pc, mv, vlRep;
	Xqwl.hdc = GetDC(Xqwl.hWnd);
	Xqwl.hdcTmp = CreateCompatibleDC(Xqwl.hdc);
	sq = Xqwl.bFlipped ? SQUARE_FLIP(sq) : sq;
	pc = pos.ucpcSquares[sq];

	if ((pc & SIDE_TAG(pos.sdPlayer)) != 0) 
	{
		// 如果点击自己的子，那么直接选中该子
		if (Xqwl.sqSelected != 0)
		{
			DrawSquare(Xqwl.sqSelected);
		}
		Xqwl.sqSelected = sq;
		DrawSquare(sq, DRAW_SELECTED);
		if (Xqwl.mvLast != 0) 
		{
			DrawSquare(SRC(Xqwl.mvLast));
			DrawSquare(DST(Xqwl.mvLast));
		}
		PlayResWav(IDR_CLICK); // 播放点击的声音

	} 
	else if (Xqwl.sqSelected != 0 && !Xqwl.bGameOver) 
	{
		// 如果点击的不是自己的子，但有子选中了(一定是自己的子)，那么走这个子
		mv = MOVE(Xqwl.sqSelected, sq);
		if (pos.LegalMove(mv)) 
		{
			if (pos.MakeMove(mv)) 
			{
				Xqwl.mvLast = mv;
				DrawSquare(Xqwl.sqSelected, DRAW_SELECTED);
				DrawSquare(sq, DRAW_SELECTED);
				Xqwl.sqSelected = 0;
				// 检查重复局面
				vlRep = pos.RepStatus(3);
				if (pos.IsMate()) 
				{
					// 如果分出胜负，那么播放胜负的声音，并且弹出不带声音的提示框
					PlayResWav(IDR_WIN);
					MessageBoxMute("Congratulations!");
					Xqwl.bGameOver = TRUE;
				} 
				else if (vlRep > 0) 
				{
					vlRep = pos.RepValue(vlRep);
					// 注意："vlRep"是对电脑来说的分值
					PlayResWav(vlRep > WIN_VALUE ? IDR_LOSS : vlRep < -WIN_VALUE ? IDR_WIN : IDR_DRAW);
					MessageBoxMute(vlRep > WIN_VALUE ? "If you always kill, then we think you are failed." :vlRep < -WIN_VALUE ? "The computer always kill, we think you win. :-)" : "We can not know who will win.");
					Xqwl.bGameOver = TRUE;
				} 
				else if (pos.nMoveNum > 100) 
				{
					PlayResWav(IDR_DRAW);
					MessageBoxMute("Game Over!");
					Xqwl.bGameOver = TRUE;
				}
				else 
				{
					// 如果没有分出胜负，那么播放将军、吃子或一般走子的声音
					PlayResWav(pos.InCheck() ? IDR_CHECK : pos.Captured() ? IDR_CAPTURE : IDR_MOVE);
					if (pos.Captured()) 
					{
						pos.SetIrrev();
					}
					ResponseMove(); // 轮到电脑走棋
				}
			} 
			else 
			{
				PlayResWav(IDR_ILLEGAL); // 播放被将军的声音
			}
		}
		// 如果根本就不符合走法(例如马不走日字)，那么程序不予理会
	}
	DeleteDC(Xqwl.hdcTmp);
	ReleaseDC(Xqwl.hWnd, Xqwl.hdc);
}

// 初始化棋局
static void Startup(void) 
{
	pos.Startup();
	Xqwl.sqSelected = Xqwl.mvLast = 0;
	Xqwl.bGameOver = FALSE;
}

// 窗体事件捕捉过程
static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
	int x, y;
	HDC hdc;
	RECT rect;
	PAINTSTRUCT ps;
	MSGBOXPARAMS mbp;

	switch (uMsg) 
	{
		// 新建窗口
	  case WM_CREATE:
		  // 调整窗口位置和尺寸
		  GetWindowRect(hWnd, &rect);
		  x = rect.left;
		  y = rect.top;
		  rect.right = rect.left + BOARD_WIDTH;
		  rect.bottom = rect.top + BOARD_HEIGHT;
		  AdjustWindowRect(&rect, WINDOW_STYLES, TRUE);
		  MoveWindow(hWnd, x, y, rect.right - rect.left, rect.bottom - rect.top, TRUE);
		  break;

		  // 退出
	  case WM_DESTROY:
		  PostQuitMessage(0);
		  break;
		  // 菜单命令
	  case WM_COMMAND:
		  switch (LOWORD(wParam)) 
		  {

			  //用户\电脑先走
			  case IDM_FILE_RED:
			  case IDM_FILE_BLACK:
				  Xqwl.bFlipped = (LOWORD(wParam) == IDM_FILE_BLACK);
				  Startup();
				  hdc = GetDC(Xqwl.hWnd);
				  DrawBoard(hdc);
				  if (Xqwl.bFlipped) 
				  {
					  Xqwl.hdc = hdc;
					  Xqwl.hdcTmp = CreateCompatibleDC(Xqwl.hdc);
					  ResponseMove();
					  DeleteDC(Xqwl.hdcTmp);
				  }
				  ReleaseDC(Xqwl.hWnd, hdc);
				  break;
				  
				  //退出
			  case IDM_FILE_EXIT:
				  DestroyWindow(Xqwl.hWnd);
				  break;

			  case IDM_HELP_ABOUT:
				  // 弹出带象棋图标的对话框
				  MessageBeep(MB_ICONINFORMATION);
				  mbp.cbSize = sizeof(MSGBOXPARAMS);
				  mbp.hwndOwner = hWnd;
				  mbp.hInstance = Xqwl.hInst;
				  mbp.lpszText = cszAbout;
				  mbp.lpszCaption = "About";
				  mbp.dwStyle = MB_USERICON;
				  mbp.lpszIcon = MAKEINTRESOURCE(IDI_APPICON);
				  mbp.dwContextHelpId = 0;
				  mbp.lpfnMsgBoxCallback = NULL;
				  mbp.dwLanguageId = 0;
				  MessageBoxIndirect(&mbp);
				  break;

			  case ID_OPTIONS_RESETASTEP:
				  if(bReset==0)
				  {
					  //悔一步棋
					  pos.ResetOneStep();
					  //更新棋盘
					  hdc = GetDC(Xqwl.hWnd);
					  DrawBoard(hdc);
					  ReleaseDC(Xqwl.hWnd,hdc);
					  bReset++;
				  }
				  else
				  {
					  MessageBoxMute("Only reset 1 step.");
				  }
				  break;

			  case ID_EDIT_EXCHANGEWITHCOMPUTER:
				  Xqwl.bFlipped=!Xqwl.bFlipped;
				  //交换走子方
				  hdc = GetDC(Xqwl.hWnd);
				  DrawBoard(hdc);
				  //电脑下棋
				  Xqwl.hdc = hdc;
				  Xqwl.hdcTmp = CreateCompatibleDC(Xqwl.hdc);
				  ResponseMove();
				  DeleteDC(Xqwl.hdcTmp);
				  //释放
				  ReleaseDC(Xqwl.hWnd,hdc);
				  break;
		  }
		  break;

		  // 绘图
	  case WM_PAINT:
		  hdc = BeginPaint(Xqwl.hWnd, &ps);
		  DrawBoard(hdc);
		  EndPaint(Xqwl.hWnd, &ps);
		  break;
		  
		  // 鼠标点击
	  case WM_LBUTTONDOWN:
		  x = FILE_LEFT + (LOWORD(lParam) - BOARD_EDGE) / SQUARE_SIZE;
		  y = RANK_TOP + (HIWORD(lParam) - BOARD_EDGE) / SQUARE_SIZE;
		  if (x >= FILE_LEFT && x <= FILE_RIGHT && y >= RANK_TOP && y <= RANK_BOTTOM) 
		  {
			  ClickSquare(COORD_XY(x, y));
		  }
		  break;
		  // 其他事件
	  default:
		  return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return FALSE;
}

// 装入资源图片
inline HBITMAP LoadResBmp(int nResId) 
{
	return (HBITMAP) LoadImage(Xqwl.hInst, MAKEINTRESOURCE(nResId), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
}


// 入口过程
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 
{
	int i;
	MSG msg;
	WNDCLASSEX wce;

	// 初始化全局变量
	srand((DWORD) time(NULL));
	InitZobrist();
	Xqwl.hInst = hInstance;
	LoadBook();
	Xqwl.bFlipped = FALSE;
	Startup();

	// 装入图片
	Xqwl.bmpBoard = LoadResBmp(IDB_BOARD);
	Xqwl.bmpSelected = LoadResBmp(IDB_SELECTED);
	for (i = PIECE_KING; i <= PIECE_PAWN; i ++) 
	{
		Xqwl.bmpPieces[SIDE_TAG(0) + i] = LoadResBmp(IDB_RK + i);
		Xqwl.bmpPieces[SIDE_TAG(1) + i] = LoadResBmp(IDB_BK + i);
	}

	// 设置窗口
	wce.cbSize = sizeof(WNDCLASSEX);
	wce.style = 0;
	wce.lpfnWndProc = (WNDPROC) WndProc;
	wce.cbClsExtra = wce.cbWndExtra = 0;
	wce.hInstance = hInstance;
	wce.hIcon = (HICON) LoadImage(hInstance, MAKEINTRESOURCE(IDI_APPICON), IMAGE_ICON, 32, 32, LR_SHARED);
	wce.hCursor = (HCURSOR) LoadImage(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
	wce.hbrBackground = (HBRUSH) (COLOR_BTNFACE + 1);
	wce.lpszMenuName = MAKEINTRESOURCE(IDM_MAINMENU);
	wce.lpszClassName = "ZChess";
	wce.hIconSm = (HICON) LoadImage(hInstance, MAKEINTRESOURCE(IDI_APPICON), IMAGE_ICON, 16, 16, LR_SHARED);
	RegisterClassEx(&wce);

	// 打开窗口
	Xqwl.hWnd = CreateWindow("ZChess", "ZChess", WINDOW_STYLES, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	if (Xqwl.hWnd == NULL) 
	{
		return 0;  
	}
	ShowWindow(Xqwl.hWnd, nCmdShow);
	UpdateWindow(Xqwl.hWnd);

	// 接收消息
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}
