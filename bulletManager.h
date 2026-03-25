//=====================================================================
//
// bulletManager.cppのヘッダファイル [bulletManager.h]
// Author : 
// 
//=====================================================================
#ifndef _BulletManager_H_
#define _BulletManager_H_

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "main.h"
#include "baseObject.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define DYNAMIC_ASSERT(expr, outstr) if(!expr){ OutputDebugString(TEXT(outstr)); __debugbreak(); }	// 動的なアサ―ト感知

//*********************************************************************
// 
// ***** 列挙型 *****
// 
//*********************************************************************
// --- 弾の種類 --- //
typedef enum
{
	BT_TEST = 0,	// テスト用発射弾
	BT_MAX
}BULLETTYPE;

//*********************************************************************
// 
// ***** 構造体 *****
// 
//*********************************************************************
// --- 弾の構造体 --- //
typedef struct Bullet
{
	BASEOBJECT obj;			// オブジェクト情報
	D3DXVECTOR3 move;		// 移動量
	D3DXVECTOR3 rotMove;	// 回転の移動量
	BULLETTYPE type;		// 弾の種類
	float fSpeed;			// 速度
	int nLife;				// 現在の残存時間
	int nMaxLife;			// 設定された残存時間
	bool bUse;				// 使用されているか
} Bullet;

typedef Bullet *LPBULLET, *PBULLET;

// --- 弾に関する定数まとめ --- //
typedef struct BULLET_CONST
{
	static const int nBulletMax;
}BULLET_CONST;

//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************
void InitBulletManager(void);
void UninitBulletManager(void);
void UpdateBulletManager(void);
void DrawBulletManager(void);

LPBULLET GetBulletPtr(void);
#endif