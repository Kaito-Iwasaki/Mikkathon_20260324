//=====================================================================
//
// itemGenerator [itemGenerator.cpp]
// Author : 
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "itemGenerator.h"
#include "Item.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** 構造体 *****
// 
//*********************************************************************
typedef struct
{
	ITEM_GENERATE_SETTING IGS;	// アイテム出現の設定
	int nCounter;				// フレームカウント
} ItemGenerator;

//*********************************************************************
// 
// ***** 列挙型 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
ItemGenerator g_itemGenerator = {};		// ジェネレーターの情報

//=====================================================================
// 更新処理
//=====================================================================
void UpdateItemGenerator(void)
{
	if (g_itemGenerator.nCounter % g_itemGenerator.IGS.nFrameSpawn == 0)
	{
		ITEM_GENERATE_SETTING *pSetting = &g_itemGenerator.IGS;

		// アイテムの出現数
		int nSpawn = rand() % (pSetting->nUpperSpawn - pSetting->nLowerSpawn) + pSetting->nLowerSpawn;
		
		// アイテムを出現
		for (int nCntItem = 0; nCntItem < nSpawn; nCntItem++)
		{
			int nXMax = pSetting->rect.right, nXMin = pSetting->rect.left;
			int nYMax = pSetting->rect.bottom, nYMin = pSetting->rect.top;

			// 位置を設定
			float fXPos = (float)(rand() % nXMax + nXMin);
			float fYPos = (float)(rand() % nYMax + nYMin);

			// 種類を設定
			ITEMTYPE type = (ITEMTYPE)(rand() % ITEMTYPE_MAX);

			SetItem(D3DXVECTOR3(fXPos, fYPos, 0), type);
		}
	}

	g_itemGenerator.nCounter++;
}

//=====================================================================
// 出現設定処理
//=====================================================================
void SetItemGenerator(ITEM_GENERATE_SETTING igs)
{
	ZeroMemory(&g_itemGenerator, sizeof(ItemGenerator));

	// 設定保存
	g_itemGenerator.IGS = igs;
}