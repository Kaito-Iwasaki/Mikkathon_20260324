//=====================================================================
//
// Player.cppのヘッダファイル [Player.h]
// Author : Kaito Iwasaki
// 
//=====================================================================
#ifndef _Player_H_
#define _Player_H_

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "main.h"
#include "baseObject.h"
#include "util.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define PLAYER_INIT_SPEED			(5.0f)
#define PLAYER_INIT_POWER			(10)
#define PLAYER_INIT_ROTSPEED		(0.05f)
#define PLAYER_INIT_BULLETSPEED		(10.0f)
#define PLAYER_MAX_HOLDABLE_BULLET	(5)

//*********************************************************************
// 
// ***** 列挙型 *****
// 
//*********************************************************************
typedef enum
{
	PLAYERSTATE_NORMAL = 0,
	PLAYERSTATE_APPEAR,
	PLAYERSTATE_SUPER,
	PLAYERSTATE_SMASH,
	PLAYERSTATE_MAX
}PLAYERSTATE;

//*********************************************************************
// 
// ***** 構造体 *****
// 
//*********************************************************************
typedef struct PLAYER
{
	BASEOBJECT obj;
	D3DXVECTOR3 move;
	D3DXVECTOR3 rotMove;
	int nTexture;
	int nMaxBullet;
	int nBulletLeft;
	float fBulletSpeed;
	int nPower;
	float fSpeed;
	int nIdxLevel;
	PLAYERSTATE state;
	int nCounterState;
	bool bControlEnabled;
	int nScore;
};

//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER* GetPlayer(void);
void SetPlayerState(PLAYERSTATE newState);
PLAYERSTATE GetPlayerState(void);
void SmashPlayer(D3DXVECTOR3 dir);
void AddScore(int nValue);

#endif