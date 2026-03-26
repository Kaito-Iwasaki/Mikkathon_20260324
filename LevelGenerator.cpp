//================================================================================================================
//
// LevelGenerator [LevelGenerator.cpp]
// Author : TENMA
//
//================================================================================================================
#include "LevelGenerator.h"
#include "Enemy.h"
#include "baseObject.h"
#include <assert.h>

//*************************************************************************************************
//*** マクロ定義 ***
//*************************************************************************************************
#define MAX_LEVEL			(MAX_ENEMY + 1)	// 設定できるレベルの最大数
#define NUM_PLACE			(3)				// タイマーの最大桁数
#define NUM_WIDTH			(15.0f)			// タイマーの数字の横幅
#define NUM_HEIGHT			(20.0f)			// タイマーの数字の縦幅
#define WIDTH_LV			(30)
#define HEIGHT_LV			(15)
#define NUM_SPACE			(30.0f)			// タイマーの数字の隙間
#define RELEASE(p) do{ if(p != nullptr){ (p)->Release(); (p) = nullptr;} }while(0)				// バッファ解放
#define RELEASE_ARRAY(pp, num) do{ for(int i = 0; i < num; i++){ RELEASE(pp[i]); } }while(0)	// 配列バッファ解放(配列の先頭アドレス, 配列数)
#define TEX_NUM				(2)

//*************************************************************************************************
//*** スコア構造体 ***
//*************************************************************************************************
typedef struct
{
	BASEOBJECT objLv;		// Lvのポリゴン
	BASEOBJECT aLevelNum[NUM_PLACE];	// 数字のポリゴン
	int nLevel;				// スコア
	bool bUse;				// 使用状況
	int nCounter;			// 点滅カウント
	bool bBlink;			// 点滅
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffNum;	// 数字の頂点バッファのポインタ
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffLv;		// Lvの頂点バッファのポインタ
} Level, *LPLEVEL, *PLEVEL;

//*************************************************************************************************
//*** プロトタイプ宣言 ***
//*************************************************************************************************
LPLEVEL GetLevelPtr(void);
bool CreateBufferLevel(LPLEVEL pOut);

//*************************************************************************************************
//*** グローバル変数 ***
//*************************************************************************************************
Level g_aLevel[MAX_LEVEL];				// スコア情報
LPDIRECT3DTEXTURE9 g_apTexBuffLevel[TEX_NUM];	// テクスチャバッファのポインタ

const char *g_apTextureLevel[TEX_NUM] =
{
	"data/TEXTURE/number000.png",
	"data/TEXTURE/Lv.png",
};

//=================================================================================================
// --- スコアの初期化処理 ---
//=================================================================================================
void InitLevelGenerator(void)
{
	// 初期化
	ZeroMemory(GetLevelPtr(), sizeof(Level) * MAX_LEVEL);

	// バッファの作成
	LPLEVEL pLevel = GetLevelPtr();

	for (int nCntItem = 0; nCntItem < MAX_LEVEL; nCntItem++, pLevel++)
	{
		CreateBufferLevel(pLevel);
		pLevel->objLv.bVisible = true;
	}

	// テクスチャの読み込み
	for (int nCntTex = 0; nCntTex < TEX_NUM; nCntTex++)
	{
		if (g_apTextureLevel[nCntTex] == nullptr)continue;

		D3DXCreateTextureFromFile(GetDevice(),
			g_apTextureLevel[nCntTex],
			&g_apTexBuffLevel[nCntTex]);
	}
}

//=================================================================================================
// --- スコアの終了 ---
//=================================================================================================
void UninitLevelGenerator(void)
{
	LPLEVEL pLevel = GetLevelPtr();

	// 頂点バッファの解放
	for (int nCntLevel = 0; nCntLevel < MAX_LEVEL; nCntLevel++, pLevel++)
	{
		RELEASE(pLevel->pVtxBuffLv);
		RELEASE(pLevel->pVtxBuffNum);
	}

	// テクスチャの解放
	RELEASE_ARRAY(g_apTexBuffLevel, TEX_NUM);
}

//=================================================================================================
// --- スコアの更新 ---
//=================================================================================================
void UpdateLevelGenerator(void)
{
	LPLEVEL pLevel = GetLevelPtr();	// レベルのポインタ取得
	VERTEX_2D* pVtx;				// 頂点情報へのポインタ

	for (int nCntItem = 0; nCntItem < MAX_LEVEL; nCntItem++, pLevel++)
	{
		if (pLevel->bUse == false) continue;

		if (pLevel->bUse)
		{// 表示状態
			if (pLevel->bBlink)
			{
				pLevel->nCounter--;
				if (pLevel->nCounter % 10 < 5)
				{
					pLevel->objLv.bVisible = false;
				}
				else
				{
					pLevel->objLv.bVisible = true;
				}

				if (pLevel->nCounter <= 0)
				{
					pLevel->bBlink = false;
					pLevel->objLv.bVisible = true;
					pLevel->nCounter = 60;
				}
			}

			// 頂点バッファをロックし、頂点情報へのポインタを取得
			pLevel->pVtxBuffNum->Lock(0, 0, (void**)&pVtx, 0);

			for (int nCntVtx = 0; nCntVtx < NUM_PLACE; nCntVtx++)
			{
				// 頂点座標の設定(座標設定は必ず右回りで！！！)
				pVtx[0].pos.x = pLevel->aLevelNum[nCntVtx].pos.x + (NUM_SPACE * nCntVtx) - NUM_WIDTH;
				pVtx[0].pos.y = pLevel->aLevelNum[nCntVtx].pos.y - NUM_HEIGHT;
				pVtx[0].pos.z = 0.0f;

				pVtx[1].pos.x = pLevel->aLevelNum[nCntVtx].pos.x + (NUM_SPACE * nCntVtx) + NUM_WIDTH;
				pVtx[1].pos.y = pLevel->aLevelNum[nCntVtx].pos.y - NUM_HEIGHT;
				pVtx[1].pos.z = 0.0f;

				pVtx[2].pos.x = pLevel->aLevelNum[nCntVtx].pos.x + (NUM_SPACE * nCntVtx) - NUM_WIDTH;
				pVtx[2].pos.y = pLevel->aLevelNum[nCntVtx].pos.y + NUM_HEIGHT;
				pVtx[2].pos.z = 0.0f;

				pVtx[3].pos.x = pLevel->aLevelNum[nCntVtx].pos.x + (NUM_SPACE * nCntVtx) + NUM_WIDTH;
				pVtx[3].pos.y = pLevel->aLevelNum[nCntVtx].pos.y + NUM_HEIGHT;
				pVtx[3].pos.z = 0.0f;

				// オフセット適用
				SetVertexPos(pVtx, pVtx[0].pos, pVtx[1].pos, pVtx[2].pos, pVtx[3].pos);

				pVtx += 4;
			}

			pLevel->pVtxBuffNum->Unlock();

			// 頂点バッファをロックし、頂点情報へのポインタを取得
			pLevel->pVtxBuffLv->Lock(0, 0, (void**)&pVtx, 0);

			// 頂点情報を設定
			pVtx[0].pos.x = pLevel->objLv.pos.x - WIDTH_LV;
			pVtx[0].pos.y = pLevel->objLv.pos.y - HEIGHT_LV;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = pLevel->objLv.pos.x + WIDTH_LV;
			pVtx[1].pos.y = pLevel->objLv.pos.y - HEIGHT_LV;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = pLevel->objLv.pos.x - WIDTH_LV;
			pVtx[2].pos.y = pLevel->objLv.pos.y + HEIGHT_LV;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = pLevel->objLv.pos.x + WIDTH_LV;
			pVtx[3].pos.y = pLevel->objLv.pos.y + HEIGHT_LV;
			pVtx[3].pos.z = 0.0f;

			// オフセット適用
			SetVertexPos(pVtx, pVtx[0].pos, pVtx[1].pos, pVtx[2].pos, pVtx[3].pos);

			pLevel->pVtxBuffLv->Unlock();
		}
	}
}

//=================================================================================================
// --- スコアの描画 ---
//=================================================================================================
void DrawLevelGenerator(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	LPLEVEL pLevel = GetLevelPtr();	// レベルのポインタ取得

	for (int nCntItem = 0; nCntItem < MAX_LEVEL; nCntItem++, pLevel++)
	{
		if (pLevel->bUse == false) continue;

		if (pLevel->bUse)
		{// 表示状態
			// 数字の描画

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, pLevel->pVtxBuffNum, 0, sizeof(VERTEX_2D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_apTexBuffLevel[0]);

			if (pLevel->objLv.bVisible)
			{
				for (int nCntNum = 0; nCntItem < NUM_PLACE; nCntItem++)
				{
					// ポリゴンの描画
					pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntItem, 2);
				}
			}

			// Lvの描画

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, pLevel->pVtxBuffLv, 0, sizeof(VERTEX_2D));

			// テクスチャの設定
			pDevice->SetTexture(0, g_apTexBuffLevel[1]);

			if (pLevel->objLv.bVisible)
			{
				// ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
			}
		}
	}
}

//=================================================================================================
// --- スコアの設定 ---
//=================================================================================================
int GeneratorLevel(int nLevel, D3DXVECTOR3 posCenter)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();;			// デバイスのポインタ
	VERTEX_2D* pVtx;			// 頂点情報へのポインタ
	int aTexU[128];				//各桁の数字を収納
	int nDiff = 1;
	int nCntLevel;
	LPLEVEL pLevel = GetLevelPtr();

	for (int nCntLevel = 0; nCntLevel < MAX_LEVEL; nCntLevel++, pLevel++)
	{
		if (pLevel->bUse == false)
		{
			pLevel->nLevel = nLevel;

			for (int nCntTex = 0; nCntTex < NUM_PLACE; nCntTex++)
			{
				aTexU[nCntTex] = pLevel->nLevel % (int)powf(10.0f, (float)(NUM_PLACE - nCntTex)) / (int)powf(10.0f, (float)(NUM_PLACE - nCntTex) - 1.0f);
			}

			// 頂点バッファをロックし、頂点情報へのポインタを取得
			pLevel->pVtxBuffNum->Lock(0, 0, (void**)&pVtx, 0);

			for (int nCntVtx = 0; nCntVtx < NUM_PLACE; nCntVtx++)
			{
				pLevel->aLevelNum[nCntVtx].pos = posCenter;

				// 頂点座標の設定(座標設定は必ず右回りで！！！)
				pVtx[0].pos.x = pLevel->aLevelNum[nCntVtx].pos.x + (NUM_SPACE * nCntVtx) - NUM_WIDTH;
				pVtx[0].pos.y = pLevel->aLevelNum[nCntVtx].pos.y - NUM_HEIGHT;
				pVtx[0].pos.z = 0.0f;

				pVtx[1].pos.x = pLevel->aLevelNum[nCntVtx].pos.x + (NUM_SPACE * nCntVtx) + NUM_WIDTH;
				pVtx[1].pos.y = pLevel->aLevelNum[nCntVtx].pos.y - NUM_HEIGHT;
				pVtx[1].pos.z = 0.0f;

				pVtx[2].pos.x = pLevel->aLevelNum[nCntVtx].pos.x + (NUM_SPACE * nCntVtx) - NUM_WIDTH;
				pVtx[2].pos.y = pLevel->aLevelNum[nCntVtx].pos.y + NUM_HEIGHT;
				pVtx[2].pos.z = 0.0f;

				pVtx[3].pos.x = pLevel->aLevelNum[nCntVtx].pos.x + (NUM_SPACE * nCntVtx) + NUM_WIDTH;
				pVtx[3].pos.y = pLevel->aLevelNum[nCntVtx].pos.y + NUM_HEIGHT;
				pVtx[3].pos.z = 0.0f;

				// オフセット適用
				SetVertexPos(pVtx, pVtx[0].pos, pVtx[1].pos, pVtx[2].pos, pVtx[3].pos);

				// rhwの設定
				pVtx[0].rhw = 1.0f;
				pVtx[1].rhw = 1.0f;
				pVtx[2].rhw = 1.0f;
				pVtx[3].rhw = 1.0f;

				// 頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2((0.1f * aTexU[nCntVtx]), 0.0f);
				pVtx[1].tex = D3DXVECTOR2((0.1f * aTexU[nCntVtx]) + 0.1f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2((0.1f * aTexU[nCntVtx]), 1.0f);
				pVtx[3].tex = D3DXVECTOR2((0.1f * aTexU[nCntVtx]) + 0.1f, 1.0f);

				pVtx += 4;
			}

			pLevel->pVtxBuffNum->Unlock();

			// 頂点バッファをロックし、頂点情報へのポインタを取得
			pLevel->pVtxBuffLv->Lock(0, 0, (void**)&pVtx, 0);

			pLevel->objLv.pos.x = posCenter.x + (4 * NUM_SPACE);
			pLevel->objLv.pos.y = posCenter.y;
			pLevel->objLv.pos.z = 0.0f;
			pLevel->objLv.size.x = WIDTH_LV;
			pLevel->objLv.size.y = HEIGHT_LV;

			// 頂点情報を設定
			pVtx[0].pos.x = pLevel->objLv.pos.x - WIDTH_LV;
			pVtx[0].pos.y = pLevel->objLv.pos.y - HEIGHT_LV;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = pLevel->objLv.pos.x + WIDTH_LV;
			pVtx[1].pos.y = pLevel->objLv.pos.y - HEIGHT_LV;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = pLevel->objLv.pos.x - WIDTH_LV;
			pVtx[2].pos.y = pLevel->objLv.pos.y + HEIGHT_LV;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = pLevel->objLv.pos.x + WIDTH_LV;
			pVtx[3].pos.y = pLevel->objLv.pos.y + HEIGHT_LV;
			pVtx[3].pos.z = 0.0f;

			// オフセット適用
			SetVertexPos(pVtx, pVtx[0].pos, pVtx[1].pos, pVtx[2].pos, pVtx[3].pos);
			SetVertexRHW(pVtx, 1.0f);
			SetVertexColor(pVtx, D3DXCOLOR(1, 1, 1, 1));
			SetVertexTexturePos(pVtx);

			pLevel->pVtxBuffLv->Unlock();
			pLevel->bUse = true;

			return nCntLevel;
		}
	}

	return -1;
}

//=====================================================================
// ポインタ取得処理
//=====================================================================
LPLEVEL GetLevelPtr(void)
{
	return &g_aLevel[0];
}

//=====================================================================
// バッファ作成処理
//=====================================================================
bool CreateBufferLevel(LPLEVEL pOut)
{
	// NULLCHECK
	if (pOut == nullptr) return false;

	// 初期化
	*pOut = {};

	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	if (pDevice == nullptr) return false;	// NULLCHECK

	HRESULT hr;		// エラーコード

	// 頂点バッファ確保
	hr = pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * (4 * NUM_PLACE),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&pOut->pVtxBuffNum,
		NULL);

	hr = pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&pOut->pVtxBuffLv,
		NULL);

	return true;
}

//=====================================================================
// 移動処理
//=====================================================================
void SetPositionLevel(int nIdxLevel, D3DXVECTOR3 pos)
{
	LPLEVEL pLevel = GetLevelPtr();	// レベルのポインタ取得
	VERTEX_2D* pVtx;				// 頂点情報へのポインタ

	pLevel += nIdxLevel;
	if (pLevel->bUse == false) return;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	pLevel->pVtxBuffNum->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntVtx = 0; nCntVtx < NUM_PLACE; nCntVtx++)
	{
		pLevel->aLevelNum[nCntVtx].pos = pos;

		// 頂点座標の設定(座標設定は必ず右回りで！！！)
		pVtx[0].pos.x = pLevel->aLevelNum[nCntVtx].pos.x + (NUM_SPACE * nCntVtx) - NUM_WIDTH;
		pVtx[0].pos.y = pLevel->aLevelNum[nCntVtx].pos.y - NUM_HEIGHT;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = pLevel->aLevelNum[nCntVtx].pos.x + (NUM_SPACE * nCntVtx) + NUM_WIDTH;
		pVtx[1].pos.y = pLevel->aLevelNum[nCntVtx].pos.y - NUM_HEIGHT;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = pLevel->aLevelNum[nCntVtx].pos.x + (NUM_SPACE * nCntVtx) - NUM_WIDTH;
		pVtx[2].pos.y = pLevel->aLevelNum[nCntVtx].pos.y + NUM_HEIGHT;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = pLevel->aLevelNum[nCntVtx].pos.x + (NUM_SPACE * nCntVtx) + NUM_WIDTH;
		pVtx[3].pos.y = pLevel->aLevelNum[nCntVtx].pos.y + NUM_HEIGHT;
		pVtx[3].pos.z = 0.0f;

		// オフセット適用
		SetVertexPos(pVtx, pVtx[0].pos, pVtx[1].pos, pVtx[2].pos, pVtx[3].pos);

		pVtx += 4;
	}

	pLevel->pVtxBuffNum->Unlock();

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	pLevel->pVtxBuffLv->Lock(0, 0, (void**)&pVtx, 0);

	pLevel->objLv.pos = pos;
	pLevel->objLv.pos.x = pos.x + (3.7f * NUM_SPACE);

	// 頂点情報を設定
	pVtx[0].pos.x = pLevel->objLv.pos.x - WIDTH_LV;
	pVtx[0].pos.y = pLevel->objLv.pos.y - HEIGHT_LV;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = pLevel->objLv.pos.x + WIDTH_LV;
	pVtx[1].pos.y = pLevel->objLv.pos.y - HEIGHT_LV;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = pLevel->objLv.pos.x - WIDTH_LV;
	pVtx[2].pos.y = pLevel->objLv.pos.y + HEIGHT_LV;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = pLevel->objLv.pos.x + WIDTH_LV;
	pVtx[3].pos.y = pLevel->objLv.pos.y + HEIGHT_LV;
	pVtx[3].pos.z = 0.0f;

	// オフセット適用
	SetVertexPos(pVtx, pVtx[0].pos, pVtx[1].pos, pVtx[2].pos, pVtx[3].pos);

	pLevel->pVtxBuffLv->Unlock();
}

//=====================================================================
// バッファ作成処理
//=====================================================================
void AddLevel(int nIdxLevel, int nValue)
{
	LPLEVEL pLevel = GetLevelPtr();	// レベルのポインタ取得

	pLevel += nIdxLevel;
	if (pLevel->bUse == false) return;

	SetLevel(nIdxLevel, pLevel->nLevel + nValue);
}

//=====================================================================
// バッファ作成処理
//=====================================================================
void SetLevel(int nIdxLevel, int nElem)
{
	LPLEVEL pLevel = GetLevelPtr();	// レベルのポインタ取得
	VERTEX_2D* pVtx;				// 頂点情報へのポインタ
	int aTexU[128];					//各桁の数字を収納

	pLevel += nIdxLevel;
	if (pLevel->bUse == false) return;

	if (pLevel->nLevel / 100 < nElem / 100)
	{
		pLevel->bBlink = true;
		pLevel->nCounter = 60;
	}

	pLevel->nLevel = nElem;
	if (pLevel->nLevel > 999) pLevel->nLevel = 999;

	// スコアから各桁を求める
	for (int nCntTex = 0; nCntTex < NUM_PLACE; nCntTex++)
	{
		aTexU[nCntTex] = pLevel->nLevel % (int)powf(10.0f, (float)(NUM_PLACE - nCntTex)) / (int)powf(10.0f, (float)(NUM_PLACE - nCntTex) - 1.0f);
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	pLevel->pVtxBuffNum->Lock(0, 0, (void**)&pVtx, 0);

	// 適用
	for (int nCntVtx = 0; nCntVtx < NUM_PLACE; nCntVtx++)
	{
		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2((0.1f * aTexU[nCntVtx]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2((0.1f * aTexU[nCntVtx]) + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((0.1f * aTexU[nCntVtx]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2((0.1f * aTexU[nCntVtx]) + 0.1f, 1.0f);

		if (pLevel->nLevel == 999)
		{
			pVtx[0].col = D3DXCOLOR(1, 1, 0, 1);
			pVtx[1].col = D3DXCOLOR(1, 1, 0, 1);
			pVtx[2].col = D3DXCOLOR(1, 1, 0, 1);
			pVtx[3].col = D3DXCOLOR(1, 1, 0, 1);
		}

		pVtx += 4;
	}

	pLevel->pVtxBuffNum->Unlock();
}

//=====================================================================
// バッファ作成処理
//=====================================================================
void RemoveLevel(int nIdxLevel)
{
	LPLEVEL pLevel = GetLevelPtr();	// レベルのポインタ取得

	pLevel += nIdxLevel;
	if (pLevel->bUse == false) return;

	pLevel->bUse = false;
	pLevel->nLevel = 0;
}

//=====================================================================
// レベル取得処理
//=====================================================================
int GetLevel(int nIdxLevel)
{
	LPLEVEL pLevel = GetLevelPtr();	// レベルのポインタ取得

	pLevel += nIdxLevel;
	if (pLevel->bUse == false) return 0;

	return pLevel->nLevel;
}