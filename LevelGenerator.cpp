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
#define WIDTH_LV			(20)
#define HEIGHT_LV			(10)
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
	int nLevelNum;			// スコアの桁数
	bool bUse;				// 使用状況
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
	NULL,
};

//=================================================================================================
// --- スコアの初期化処理 ---
//=================================================================================================
void InitLevel(void)
{
	// 初期化
	ZeroMemory(GetLevelPtr(), sizeof(Level) * MAX_LEVEL);

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
void UninitLevel(void)
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
void UpdateLevel(void)
{
	
}

//=================================================================================================
// --- スコアの描画 ---
//=================================================================================================
void DrawLevel(void)
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

			for (int nCntNum = 0; nCntItem < NUM_PLACE; nCntItem++)
			{
				// ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntItem, 2);
			}

			// Lvの描画

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, pLevel->pVtxBuffLv, 0, sizeof(VERTEX_2D));

			// テクスチャの設定
			pDevice->SetTexture(0, g_apTexBuffLevel[1]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
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
	LPLEVEL pLevel = &g_aLevel[0];

	for (int nCntLevelPointer = 0; nCntLevelPointer < MAX_LEVEL; nCntLevelPointer++, pLevel++)
	{
		if (pLevel->bUse == false)
		{
			CreateBufferLevel(pLevel);

			pLevel->nLevel = nLevel;

			for (nCntLevel = 0; nCntLevel < NUM_PLACE; nCntLevel++)
			{
				aTexU[nCntLevel] = pLevel->nLevel % (int)powf(10.0f, (float)(NUM_PLACE - nCntLevel)) / (int)powf(10.0f, (float)(NUM_PLACE - nCntLevel) - 1.0f);
			}

			// 頂点バッファをロックし、頂点情報へのポインタを取得
			pLevel->pVtxBuffNum->Lock(0, 0, (void**)&pVtx, 0);

			for (nCntLevel = 0; nCntLevel < NUM_PLACE; nCntLevel++)
			{
				// 頂点座標の設定(座標設定は必ず右回りで！！！)
				pVtx[0].pos.x = pLevel->aLevelNum[nCntLevel].pos.x + (NUM_SPACE * nCntLevel) - NUM_WIDTH;
				pVtx[0].pos.y = pLevel->aLevelNum[nCntLevel].pos.y - NUM_HEIGHT;
				pVtx[0].pos.z = 0.0f;

				pVtx[1].pos.x = pLevel->aLevelNum[nCntLevel].pos.x + (NUM_SPACE * nCntLevel) + NUM_WIDTH;
				pVtx[1].pos.y = pLevel->aLevelNum[nCntLevel].pos.y - NUM_HEIGHT;
				pVtx[1].pos.z = 0.0f;

				pVtx[2].pos.x = pLevel->aLevelNum[nCntLevel].pos.x + (NUM_SPACE * nCntLevel) - NUM_WIDTH;
				pVtx[2].pos.y = pLevel->aLevelNum[nCntLevel].pos.y + NUM_HEIGHT;
				pVtx[2].pos.z = 0.0f;

				pVtx[3].pos.x = pLevel->aLevelNum[nCntLevel].pos.x + (NUM_SPACE * nCntLevel) + NUM_WIDTH;
				pVtx[3].pos.y = pLevel->aLevelNum[nCntLevel].pos.y + NUM_HEIGHT;
				pVtx[3].pos.z = 0.0f;

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
				pVtx[0].tex = D3DXVECTOR2((0.1f * aTexU[nCntLevel]), 0.0f);
				pVtx[1].tex = D3DXVECTOR2((0.1f * aTexU[nCntLevel]) + 0.1f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2((0.1f * aTexU[nCntLevel]), 1.0f);
				pVtx[3].tex = D3DXVECTOR2((0.1f * aTexU[nCntLevel]) + 0.1f, 1.0f);

				pVtx += 4;
			}

			pLevel->pVtxBuffNum->Unlock();

			// 頂点バッファをロックし、頂点情報へのポインタを取得
			pLevel->pVtxBuffNum->Lock(0, 0, (void**)&pVtx, 0);

			pLevel->objLv.pos.x = posCenter.x - (2 * NUM_WIDTH);
			pLevel->objLv.pos.y = posCenter.y;
			pLevel->objLv.pos.z = 0.0f;
			pLevel->objLv.size.x = WIDTH_LV;
			pLevel->objLv.size.y = HEIGHT_LV;

			// 頂点情報を設定
			SetVertexPos(pVtx, pLevel->objLv);
			SetVertexRHW(pVtx, 1.0f);
			SetVertexColor(pVtx, pLevel->objLv.color);
			SetVertexTexturePos(pVtx);

			pLevel->pVtxBuffNum->Unlock();

			return nCntLevelPointer;
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
	hr = pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * (4 * pOut->nLevelNum),
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