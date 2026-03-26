//=====================================================================
//
// Enemy.cppのヘッダファイル [Enemy.h]
// Author : Kaito Iwasaki
// 
//=====================================================================
#ifndef _Enemy_H_
#define _Enemy_H_

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
#define MAX_ENEMY	(256)

//*********************************************************************
// 
// ***** 列挙型 *****
// 
//*********************************************************************
//*********************************************************************
// 敵の種類（タイプ）
//*********************************************************************
typedef enum
{
	// 動かない敵
	ENEMYTYPE_STATIC = 0,

	// プレイヤー方向に向かって追いかけてくる敵
	ENEMYTYPE_CHASER,
	ENEMYTYPE_CHASER_SMALL,
	ENEMYTYPE_CHASER_BIG,

	ENEMYTYPE_REFRECT,
	ENEMYTYPE_REFRECT_SMALL,
	ENEMYTYPE_REFRECT_BIG,

	ENEMYTYPE_MAX
}ENEMYTYPE;

//*********************************************************************
// 敵の状態（ステート）
//*********************************************************************
typedef enum
{
	// 通常
	ENEMYSTATE_NORMAL = 0,

	// ダメージを受けた直後
	ENEMYSTATE_DAMAGE,

	// 死んだ直後
	ENEMYSTATE_SMASH,

	ENEMYSTATE_MAX
}ENEMYSTATE;

//*********************************************************************
// 
// ***** 構造体 *****
// 
//*********************************************************************
typedef struct ENEMY
{
	BASEOBJECT obj;
	bool bUsed;
	D3DXVECTOR3 move;
	float fRotMove;
	ENEMYSTATE state;
	int nConunterState;
	int nLife;
	float fSpeed;
	ENEMYTYPE type;
	int nScore;
	int nTexture;
};

//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
ENEMY* SetEnemy(ENEMYTYPE type, D3DXVECTOR3 pos);
ENEMY* GetEnemy(void);
bool DamageEnemy(ENEMY* pEnemy, int nDamage);
void SmashEnemy(ENEMY* pEnemy);
void KillEnemy(ENEMY* pEnemy);
void SetEnemyState(ENEMY* pEnemy, ENEMYSTATE newState);

#endif