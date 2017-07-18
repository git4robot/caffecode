#pragma once
#include "inline.h"
#include "RC4Struct.h"
#include "ZobristStruct.h"

// Zobrist表
static struct 
{
	ZobristStruct Player;
	ZobristStruct Table[14][256];
}Zobrist;

// 局面结构
struct PositionStruct 
{
	int sdPlayer;                   // 轮到谁走，0=红方，1=黑方
	BYTE ucpcSquares[256];          // 棋盘上的棋子
	int vlWhite, vlBlack;           // 红、黑双方的子力价值
	int nDistance, nMoveNum;        // 距离根节点的步数，历史走法数
	MoveStruct mvsList[MAX_MOVES];  // 历史走法信息列表
	ZobristStruct zobr;             // Zobrist

	// 清空棋盘
	void ClearBoard(void) 
	{         
		sdPlayer = vlWhite = vlBlack = nDistance = 0;
		memset(ucpcSquares, 0, 256);
		zobr.InitZero();
	}

	// 清空(初始化)历史走法信息
	void SetIrrev(void) 
	{           
		mvsList[0].Set(0, 0, Checked(), zobr.dwKey);
		nMoveNum = 1;
	}

	// 初始化棋盘
	void Startup(void)
	{
		int sq, pc;
		ClearBoard();
		for (sq = 0; sq < 256; sq ++) 
		{
			pc = cucpcStartup[sq];
			if (pc != 0) 
			{
				AddPiece(sq, pc);
			}
		}
		SetIrrev();
	}

	// 交换走子方
	void ChangeSide(void) 
	{         
		sdPlayer = 1 - sdPlayer;
		zobr.Xor(Zobrist.Player);
	}


	// 在棋盘上放一枚棋子
	void AddPiece(int sq, int pc) 
	{ 
		ucpcSquares[sq] = pc;
		// 红方加分，黑方(注意"cucvlPiecePos"取值要颠倒)减分
		if (pc < 16) 
		{
			vlWhite += cucvlPiecePos[pc - 8][sq];
			zobr.Xor(Zobrist.Table[pc - 8][sq]);
		} 
		else 
		{
			vlBlack += cucvlPiecePos[pc - 16][SQUARE_FLIP(sq)];
			zobr.Xor(Zobrist.Table[pc - 9][sq]);
		}
	}

	// 从棋盘上拿走一枚棋子,其实就是置零
	void DelPiece(int sq, int pc) 
	{ 
		ucpcSquares[sq] = 0;
		// 红方减分，黑方(注意"cucvlPiecePos"取值要颠倒)加分
		if (pc < 16) 
		{
			vlWhite -= cucvlPiecePos[pc - 8][sq];
			zobr.Xor(Zobrist.Table[pc - 8][sq]);
		} 
		else 
		{
			vlBlack -= cucvlPiecePos[pc - 16][SQUARE_FLIP(sq)];
			zobr.Xor(Zobrist.Table[pc - 9][sq]);
		}
	}

	// 局面评价函数
	int Evaluate(void) const 
	{      
		return (sdPlayer == 0 ? vlWhite - vlBlack : vlBlack - vlWhite) + ADVANCED_VALUE;
	}

	// 是否被将军
	BOOL InCheck(void) const 
	{      
		return mvsList[nMoveNum - 1].ucbCheck;
	}

	// 上一步是否吃子
	BOOL Captured(void) const 
	{     
		return mvsList[nMoveNum - 1].ucpcCaptured != 0;
	}

	// 搬一步棋的棋子
	int MovePiece(int mv)
	{
		int sqSrc, sqDst, pc, pcCaptured;
		sqSrc = SRC(mv);
		sqDst = DST(mv);
		pcCaptured = ucpcSquares[sqDst];
		if (pcCaptured != 0) 
		{
			DelPiece(sqDst, pcCaptured);
		}
		pc = ucpcSquares[sqSrc];
		DelPiece(sqSrc, pc);
		AddPiece(sqDst, pc);
		return pcCaptured;
	}

	// 撤消搬一步棋的棋子
	void UndoMovePiece(int mv, int pcCaptured)
	{
		int sqSrc, sqDst, pc;
		sqSrc = SRC(mv);
		sqDst = DST(mv);
		pc = ucpcSquares[sqDst];
		DelPiece(sqDst, pc);
		AddPiece(sqSrc, pc);
		if (pcCaptured != 0) 
		{
			AddPiece(sqDst, pcCaptured);
		}
	}

	// 走一步棋
	BOOL MakeMove(int mv)
	{
		bReset=0;
		int pcCaptured;
		DWORD dwKey;

		dwKey = zobr.dwKey;
		pcCaptured = MovePiece(mv);
		if (Checked()) 
		{
			UndoMovePiece(mv, pcCaptured);
			return FALSE;
		}
		ChangeSide();
		mvsList[nMoveNum].Set(mv, pcCaptured, Checked(), dwKey);
		nMoveNum ++;
		nDistance ++;
		return TRUE;
	}

	//悔一步棋
	void ResetOneStep(void)
	{
		int pcCaptured=mvsList[nMoveNum].ucpcCaptured;
		int pcCaptured2=mvsList[nMoveNum-1].ucpcCaptured;
		bool eat=(pcCaptured!=0)||(pcCaptured2!=0);
		if(!eat)
		{
			UndoMakeMove();
			UndoMakeMove();
		}
		else
		{
			MessageBoxMute("Compute does not promise to reset a step.");
		}
				
	}
	

	// 撤消走一步棋
	void UndoMakeMove(void) 
	{                   
		nDistance --;
		nMoveNum --;
		ChangeSide();
		UndoMovePiece(mvsList[nMoveNum].wmv, mvsList[nMoveNum].ucpcCaptured);
	}

	void NullMove(void) // 走一步空步
	{                       
		DWORD dwKey;
		dwKey = zobr.dwKey;
		ChangeSide();
		mvsList[nMoveNum].Set(0, 0, FALSE, dwKey);
		nMoveNum ++;
		nDistance ++;
	}

	// 撤消走一步空步
	void UndoNullMove(void) 
	{                   
		nDistance --;
		nMoveNum --;
		ChangeSide();
	}
	
	// 生成所有走法，如果"bCapture"为"TRUE"则只生成吃子走法
	int GenerateMoves(int *mvs, BOOL bCapture = FALSE) const 
	{
		int i, j, nGenMoves, nDelta, sqSrc, sqDst;
		int pcSelfSide, pcOppSide, pcSrc, pcDst;
		// 生成所有走法，需要经过以下几个步骤：

		nGenMoves = 0;
		pcSelfSide = SIDE_TAG(sdPlayer);
		pcOppSide = OPP_SIDE_TAG(sdPlayer);
		for (sqSrc = 0; sqSrc < 256; sqSrc ++) 
		{

			// 1. 找到一个本方棋子，再做以下判断：
			pcSrc = ucpcSquares[sqSrc];
			if ((pcSrc & pcSelfSide) == 0) 
			{
				continue;
			}

			// 2. 根据棋子确定走法
			switch (pcSrc - pcSelfSide) 
			{
				case PIECE_KING:
					for (i = 0; i < 4; i ++)
					{
						sqDst = sqSrc + ccKingDelta[i];
						if (!IN_FORT(sqDst)) 
						{
							continue;
						}
						pcDst = ucpcSquares[sqDst];
						if (bCapture ? (pcDst & pcOppSide) != 0 : (pcDst & pcSelfSide) == 0) {
							mvs[nGenMoves] = MOVE(sqSrc, sqDst);
							nGenMoves ++;
						}
					}
					break;
				case PIECE_ADVISOR:
					for (i = 0; i < 4; i ++) 
					{
						sqDst = sqSrc + ccAdvisorDelta[i];
						if (!IN_FORT(sqDst)) 
						{
							continue;
						}
						pcDst = ucpcSquares[sqDst];
						if (bCapture ? (pcDst & pcOppSide) != 0 : (pcDst & pcSelfSide) == 0) 
						{
							mvs[nGenMoves] = MOVE(sqSrc, sqDst);
							nGenMoves ++;
						}
					}
					break;
				case PIECE_BISHOP:
					for (i = 0; i < 4; i ++) {
						sqDst = sqSrc + ccAdvisorDelta[i];
						if (!(IN_BOARD(sqDst) && HOME_HALF(sqDst, sdPlayer) && ucpcSquares[sqDst] == 0)) 
						{
							continue;
						}
						sqDst += ccAdvisorDelta[i];
						pcDst = ucpcSquares[sqDst];
						if (bCapture ? (pcDst & pcOppSide) != 0 : (pcDst & pcSelfSide) == 0) 
						{
							mvs[nGenMoves] = MOVE(sqSrc, sqDst);
							nGenMoves ++;
						}
					}
					break;
				case PIECE_KNIGHT:
					for (i = 0; i < 4; i ++) 
					{
						sqDst = sqSrc + ccKingDelta[i];
						if (ucpcSquares[sqDst] != 0) 
						{
							continue;
						}
						for (j = 0; j < 2; j ++) 
						{
							sqDst = sqSrc + ccKnightDelta[i][j];
							if (!IN_BOARD(sqDst)) 
							{
								continue;
							}
							pcDst = ucpcSquares[sqDst];
							if (bCapture ? (pcDst & pcOppSide) != 0 : (pcDst & pcSelfSide) == 0) 
							{
								mvs[nGenMoves] = MOVE(sqSrc, sqDst);
								nGenMoves ++;
							}
						}
					}
					break;

				case PIECE_ROOK:
					for (i = 0; i < 4; i ++) 
					{
						nDelta = ccKingDelta[i];
						sqDst = sqSrc + nDelta;
						while (IN_BOARD(sqDst)) 
						{
							pcDst = ucpcSquares[sqDst];
							if (pcDst == 0) 
							{
								if (!bCapture) 
								{
									mvs[nGenMoves] = MOVE(sqSrc, sqDst);
									nGenMoves ++;
								}
							} 
							else 
							{
								if ((pcDst & pcOppSide) != 0) 
								{
									mvs[nGenMoves] = MOVE(sqSrc, sqDst);
									nGenMoves ++;
								}
								break;
							}
							sqDst += nDelta;
						}
					}
					break;

				case PIECE_CANNON:
					for (i = 0; i < 4; i ++) 
					{
						nDelta = ccKingDelta[i];
						sqDst = sqSrc + nDelta;
						while (IN_BOARD(sqDst)) 
						{
							pcDst = ucpcSquares[sqDst];
							if (pcDst == 0) 
							{
								if (!bCapture) 
								{
									mvs[nGenMoves] = MOVE(sqSrc, sqDst);
									nGenMoves ++;
								}
							}
							else 
							{
								break;
							}
							sqDst += nDelta;
						}
						sqDst += nDelta;
						while (IN_BOARD(sqDst)) 
						{
							pcDst = ucpcSquares[sqDst];
							if (pcDst != 0) 
							{
								if ((pcDst & pcOppSide) != 0) 
								{
									mvs[nGenMoves] = MOVE(sqSrc, sqDst);
									nGenMoves ++;
								}
								break;
							}
							sqDst += nDelta;
						}
					}
					break;
				case PIECE_PAWN:
					sqDst = SQUARE_FORWARD(sqSrc, sdPlayer);
					if (IN_BOARD(sqDst)) {
						pcDst = ucpcSquares[sqDst];
						if (bCapture ? (pcDst & pcOppSide) != 0 : (pcDst & pcSelfSide) == 0) 
						{
							mvs[nGenMoves] = MOVE(sqSrc, sqDst);
							nGenMoves ++;
						}
					}
					if (AWAY_HALF(sqSrc, sdPlayer)) 
					{
						for (nDelta = -1; nDelta <= 1; nDelta += 2) 
						{
							sqDst = sqSrc + nDelta;
							if (IN_BOARD(sqDst)) 
							{
								pcDst = ucpcSquares[sqDst];
								if (bCapture ? (pcDst & pcOppSide) != 0 : (pcDst & pcSelfSide) == 0) 
								{
									mvs[nGenMoves] = MOVE(sqSrc, sqDst);
									nGenMoves ++;
								}
							}
						}
					}
					break;
				}
			}
			return nGenMoves;
	}

	// 判断走法是否合理
	BOOL LegalMove(int mv) const
	{
		int sqSrc, sqDst, sqPin;
		int pcSelfSide, pcSrc, pcDst, nDelta;
		// 判断走法是否合法，需要经过以下的判断过程：

		// 1. 判断起始格是否有自己的棋子
		sqSrc = SRC(mv);
		pcSrc = ucpcSquares[sqSrc];
		pcSelfSide = SIDE_TAG(sdPlayer);
		if ((pcSrc & pcSelfSide) == 0) 
		{
			return FALSE;
		}

		// 2. 判断目标格是否有自己的棋子
		sqDst = DST(mv);
		pcDst = ucpcSquares[sqDst];
		if ((pcDst & pcSelfSide) != 0) 
		{
			return FALSE;
		}

		// 3. 根据棋子的类型检查走法是否合理
		switch (pcSrc - pcSelfSide) 
		{
		  case PIECE_KING:
			  return IN_FORT(sqDst) && KING_SPAN(sqSrc, sqDst);
		  case PIECE_ADVISOR:
			  return IN_FORT(sqDst) && ADVISOR_SPAN(sqSrc, sqDst);
		  case PIECE_BISHOP:
			  return SAME_HALF(sqSrc, sqDst) && BISHOP_SPAN(sqSrc, sqDst) &&
				  ucpcSquares[BISHOP_PIN(sqSrc, sqDst)] == 0;
		  case PIECE_KNIGHT:
			  sqPin = KNIGHT_PIN(sqSrc, sqDst);
			  return sqPin != sqSrc && ucpcSquares[sqPin] == 0;
		  case PIECE_ROOK:
		  case PIECE_CANNON:
			  if (SAME_RANK(sqSrc, sqDst)) 
			  {
				  nDelta = (sqDst < sqSrc ? -1 : 1);
			  } 
			  else if (SAME_FILE(sqSrc, sqDst)) 
			  {
				  nDelta = (sqDst < sqSrc ? -16 : 16);
			  }
			  else 
			  {
				  return FALSE;
			  }
			  sqPin = sqSrc + nDelta;
			  while (sqPin != sqDst && ucpcSquares[sqPin] == 0) 
			  {
				  sqPin += nDelta;
			  }
			  if (sqPin == sqDst) {
				  return pcDst == 0 || pcSrc - pcSelfSide == PIECE_ROOK;
			  }
			  else if (pcDst != 0 && pcSrc - pcSelfSide == PIECE_CANNON) 
			  {
				  sqPin += nDelta;
				  while (sqPin != sqDst && ucpcSquares[sqPin] == 0) 
				  {
					  sqPin += nDelta;
				  }
				  return sqPin == sqDst;
			  } 
			  else 
			  {
				  return FALSE;
			  }
		  case PIECE_PAWN:
			  if (AWAY_HALF(sqDst, sdPlayer) && (sqDst == sqSrc - 1 || sqDst == sqSrc + 1)) 
			  {
				  return TRUE;
			  }
			  return sqDst == SQUARE_FORWARD(sqSrc, sdPlayer);

		  default:
			  return FALSE;
		}
	}


	// 判断是否被将军
	BOOL Checked(void) const                   
	{
		int i, j, sqSrc, sqDst;
		int pcSelfSide, pcOppSide, pcDst, nDelta;
		pcSelfSide = SIDE_TAG(sdPlayer);
		pcOppSide = OPP_SIDE_TAG(sdPlayer);
		// 找到棋盘上的帅(将)，再做以下判断：

		for (sqSrc = 0; sqSrc < 256; sqSrc ++) 
		{
			if (ucpcSquares[sqSrc] != pcSelfSide + PIECE_KING) {
				continue;
			}

			// 1. 判断是否被对方的兵(卒)将军
			if (ucpcSquares[SQUARE_FORWARD(sqSrc, sdPlayer)] == pcOppSide + PIECE_PAWN) {
				return TRUE;
			}
			for (nDelta = -1; nDelta <= 1; nDelta += 2) {
				if (ucpcSquares[sqSrc + nDelta] == pcOppSide + PIECE_PAWN) {
					return TRUE;
				}
			}

			// 2. 判断是否被对方的马将军(以仕(士)的步长当作马腿)
			for (i = 0; i < 4; i ++) {
				if (ucpcSquares[sqSrc + ccAdvisorDelta[i]] != 0) {
					continue;
				}
				for (j = 0; j < 2; j ++) {
					pcDst = ucpcSquares[sqSrc + ccKnightCheckDelta[i][j]];
					if (pcDst == pcOppSide + PIECE_KNIGHT) {
						return TRUE;
					}
				}
			}

			// 3. 判断是否被对方的车或炮将军(包括将帅对脸)
			for (i = 0; i < 4; i ++) {
				nDelta = ccKingDelta[i];
				sqDst = sqSrc + nDelta;
				while (IN_BOARD(sqDst)) {
					pcDst = ucpcSquares[sqDst];
					if (pcDst != 0) {
						if (pcDst == pcOppSide + PIECE_ROOK || pcDst == pcOppSide + PIECE_KING) {
							return TRUE;
						}
						break;
					}
					sqDst += nDelta;
				}
				sqDst += nDelta;
				while (IN_BOARD(sqDst)) 
				{
					int pcDst = ucpcSquares[sqDst];
					if (pcDst != 0)
					{
						if (pcDst == pcOppSide + PIECE_CANNON) 
						{
							return TRUE;
						}
						break;
					}
					sqDst += nDelta;
				}
			}
			return FALSE;
		}
		return FALSE;
	}

	BOOL IsMate(void)                          // 判断是否被杀
	{
		int i, nGenMoveNum, pcCaptured;
		int mvs[MAX_GEN_MOVES];

		nGenMoveNum = GenerateMoves(mvs);
		for (i = 0; i < nGenMoveNum; i ++) 
		{
			pcCaptured = MovePiece(mvs[i]);
			if (!Checked()) 
			{
				UndoMovePiece(mvs[i], pcCaptured);
				return FALSE;
			}
			else {
				UndoMovePiece(mvs[i], pcCaptured);
			}
		}
		return TRUE;
	}



	int DrawValue(void) const // 和棋分值
	{                 
		return (nDistance & 1) == 0 ? -DRAW_VALUE : DRAW_VALUE;
	}

	int RepStatus(int nRecur = 1) const        // 检测重复局面
	{
		BOOL bSelfSide, bPerpCheck, bOppPerpCheck;
		const MoveStruct *lpmvs;

		bSelfSide = FALSE;
		bPerpCheck = bOppPerpCheck = TRUE;
		lpmvs = mvsList + nMoveNum - 1;
		while (lpmvs->wmv != 0 && lpmvs->ucpcCaptured == 0) 
		{
			if (bSelfSide) 
			{
				bPerpCheck = bPerpCheck && lpmvs->ucbCheck;
				if (lpmvs->dwKey == zobr.dwKey) 
				{
					nRecur --;
					if (nRecur == 0) 
					{
						return 1 + (bPerpCheck ? 2 : 0) + (bOppPerpCheck ? 4 : 0);
					}
				}
			}
			else 
			{
				bOppPerpCheck = bOppPerpCheck && lpmvs->ucbCheck;
			}
			bSelfSide = !bSelfSide;
			lpmvs --;
		}
		return 0;
	}

	// 重复局面分值
	int RepValue(int nRepStatus) const 
	{        
		int vlReturn;
		vlReturn = ((nRepStatus & 2) == 0 ? 0 : nDistance - BAN_VALUE) + ((nRepStatus & 4) == 0 ? 0 : BAN_VALUE - nDistance);
		return vlReturn == 0 ? DrawValue() : vlReturn;
	}

	// 判断是否允许空步裁剪
	BOOL NullOkay(void) const 
	{                 
		return (sdPlayer == 0 ? vlWhite : vlBlack) > NULL_MARGIN;
	}

	// 对局面镜像
	void Mirror(PositionStruct &posMirror) const
	{
		int sq, pc;
		posMirror.ClearBoard();
		for (sq = 0; sq < 256; sq ++) 
		{
			pc = ucpcSquares[sq];
			if (pc != 0)
			{
				posMirror.AddPiece(MIRROR_SQUARE(sq), pc);
			}
		}
		if (sdPlayer == 1) {
			posMirror.ChangeSide();
		}
		posMirror.SetIrrev();
	}
};