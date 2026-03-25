//=====================================================================
//
// Background2 [Background2.cpp]
// Author : 
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "Background2.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define TEXTURE_FILENAME	NULL
#define INIT_POS			D3DXVECTOR3(0.0f, 0.0f, 0.0f)
#define INIT_SIZE			D3DXVECTOR3(SCREEN_WIDTH * 1000, SCREEN_HEIGHT * 1000, 0.0f)
#define INIT_COLOR			D3DXCOLOR(0.71, 0.94, 1.0, 1.0)

//*********************************************************************
// 
// ***** 構造体 *****
// 
//*********************************************************************


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
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBackground2 = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffBackground2 = NULL;
Background2Struct g_Background2;

//=====================================================================
// 初期化処理
//=====================================================================
void InitBackground2(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 構造体の初期化
	memset(&g_Background2, 0, sizeof(Background2Struct));
	g_Background2.obj.pos = INIT_POS;
	g_Background2.obj.size = INIT_SIZE;
	g_Background2.obj.color = INIT_COLOR;
	g_Background2.obj.bVisible = true;

	// テクスチャの読み込み
	if (TEXTURE_FILENAME)
	{
		D3DXCreateTextureFromFile(
			pDevice,
			TEXTURE_FILENAME,
			&g_pTexBuffBackground2
		);
	}


	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBackground2,
		NULL
	);

}

//=====================================================================
// 終了処理
//=====================================================================
void UninitBackground2(void)
{
	if (g_pTexBuffBackground2 != NULL)
	{// テクスチャの破棄
		g_pTexBuffBackground2->Release();
		g_pTexBuffBackground2 = NULL;
	}

	if (g_pVtxBuffBackground2 != NULL)
	{// 頂点バッファの破棄
		g_pVtxBuffBackground2->Release();
		g_pVtxBuffBackground2 = NULL;
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateBackground2(void)
{

}

//=====================================================================
// 描画処理
//=====================================================================
void DrawBackground2(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D* pVtx;

	// 頂点バッファをロックして頂点情報へのポインタを取得
	g_pVtxBuffBackground2->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	SetVertexPos(pVtx, g_Background2.obj);
	SetVertexRHW(pVtx, 1.0f);
	SetVertexColor(pVtx, g_Background2.obj.color);
	SetVertexTexturePos(pVtx);

	// 頂点バッファをアンロック
	g_pVtxBuffBackground2->Unlock();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBackground2, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_Background2.obj.bVisible)
	{// 表示状態
		// テクスチャの設定
		pDevice->SetTexture(0, NULL);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}