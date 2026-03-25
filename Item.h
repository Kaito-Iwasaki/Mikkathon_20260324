//=====================================================================
//
// enemyGenerator.cppのヘッダファイル [enemyGenerator.h]
// Author : 
// 
//=====================================================================
#ifndef _Item_H_
#define _Item_H_

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


//*********************************************************************
// 
// ***** 列挙型 *****
// 
//*********************************************************************
// --- アイテムの種類 --- //
typedef enum
{
	ITEMTYPE_PROTEIN_ALPHA = 0,	// PROTEINタイプα
	ITEMTYPE_PROTEIN_BETA,		// PROTEINタイプβ
	ITEMTYPE_PROTEIN_GAMMA,		// PROTEINタイプγ
	ITEMTYPE_MAX
} ITEMTYPE;

// --- アイテムの状態 --- //
typedef enum
{
	ITEMSTATE_NONE = 0,		// 無し
	ITEMSTATE_NORMAL,		// 通常
	ITEMSTATE_SPAWN,		// 出現時
	ITEMSTATE_GET,			// 取得時
	ITEMSTATE_MAX			
}ITEMSTATE;

//*********************************************************************
// 
// ***** 構造体 *****
// 
//*********************************************************************
// --- アイテムの情報 --- //
typedef struct Item
{
	BASEOBJECT obj;		// オブジェクト情報
	ITEMTYPE type;		// アイテムの種類
	ITEMSTATE state;	// 状態
	unsigned int nCounterState;	// 状態カウンタ
	bool bUse;			// 使用済みか
} Item;

typedef Item *LPITEM, *PITEM;

// --- アイテムに関する定数まとめ --- //
typedef struct ITEM_CONST
{
	static const D3DXCOLOR DefColor;		// 基本色
	static const D3DXVECTOR2 DefSize;		// 基本サイズ
	static const D3DXVECTOR2 DefSizeEffect;	// 演出基本サイズ
	static const float fAlphaDecrease;		// エフェクトα値の減少係数
	static const float fYIncrease;			// エフェクトの上昇係数
	static const unsigned int aStateCount[ITEMSTATE_MAX];	// 各アイテムの状態維持時間
	static const int aLevel[ITEMTYPE_MAX];	// 各アイテムのレベルアップ幅
	static const int nMaxItem;				// アイテムの最大数
}ITEM_CONST;

//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************
void InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);

LPITEM GetItemPtr(void);
void SetItem(D3DXVECTOR3 pos, ITEMTYPE type, D3DXCOLOR color = ITEM_CONST::DefColor, D3DXVECTOR2 size = ITEM_CONST::DefSize);
#endif