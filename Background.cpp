//=====================================================================
//
// Background [Background.cpp]
// Author : Kaito Iwasaki
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "Background.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define TEXTURE_FILENAME	"data\\TEXTURE\\background.jpg"
#define INIT_POS			D3DXVECTOR3(0.0f, 0.0f, 0.0f)
#define INIT_SIZE			D3DXVECTOR3(3000.0f, 3000.0f, 0.0f)
#define INIT_COLOR			D3DXCOLOR_WHITE

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
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBackground = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffBackground = NULL;
BackgroundStruct g_Background;

//=====================================================================
// 初期化処理
//=====================================================================
void InitBackground(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 構造体の初期化
	memset(&g_Background, 0, sizeof(BackgroundStruct));
	g_Background.obj.pos = INIT_POS;
	g_Background.obj.size = INIT_SIZE;
	g_Background.obj.color = INIT_COLOR;
	g_Background.obj.bVisible = true;

	// テクスチャの読み込み
	if (TEXTURE_FILENAME)
	{
		D3DXCreateTextureFromFile(
			pDevice,
			TEXTURE_FILENAME,
			&g_pTexBuffBackground
		);
	}


	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBackground,
		NULL
	);

}

//=====================================================================
// 終了処理
//=====================================================================
void UninitBackground(void)
{
	if (g_pTexBuffBackground != NULL)
	{// テクスチャの破棄
		g_pTexBuffBackground->Release();
		g_pTexBuffBackground = NULL;
	}

	if (g_pVtxBuffBackground != NULL)
	{// 頂点バッファの破棄
		g_pVtxBuffBackground->Release();
		g_pVtxBuffBackground = NULL;
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateBackground(void)
{

}

//=====================================================================
// 描画処理
//=====================================================================
void DrawBackground(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D* pVtx;

	// 頂点バッファをロックして頂点情報へのポインタを取得
	g_pVtxBuffBackground->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	SetVertexPos(pVtx, g_Background.obj);
	SetVertexRHW(pVtx, 1.0f);
	SetVertexColor(pVtx, g_Background.obj.color);
	SetVertexTexturePos(pVtx);

	// 頂点バッファをアンロック
	g_pVtxBuffBackground->Unlock();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBackground, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_Background.obj.bVisible)
	{// 表示状態
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTexBuffBackground);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}