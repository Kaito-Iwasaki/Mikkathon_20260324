//=====================================================================
//
// TutorialDecal [TutorialDecal.cpp]
// Author : 
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "TutorialDecal.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define TEXTURE_FILENAME	"data\\TEXTURE\\Tutorial.png"
#define INIT_POS			D3DXVECTOR3(100.0f, 400.0f, 0.0f)
#define INIT_SIZE			D3DXVECTOR3(1000.0f, 500.0f, 0.0f)
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
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorialDecal = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffTutorialDecal = NULL;
TutorialDecalStruct g_TutorialDecal;

//=====================================================================
// 初期化処理
//=====================================================================
void InitTutorialDecal(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 構造体の初期化
	memset(&g_TutorialDecal, 0, sizeof(TutorialDecalStruct));
	g_TutorialDecal.obj.pos = INIT_POS;
	g_TutorialDecal.obj.size = INIT_SIZE;
	g_TutorialDecal.obj.color = INIT_COLOR;
	g_TutorialDecal.obj.bVisible = true;

	// テクスチャの読み込み
	if (TEXTURE_FILENAME)
	{
		D3DXCreateTextureFromFile(
			pDevice,
			TEXTURE_FILENAME,
			&g_pTexBuffTutorialDecal
		);
	}


	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorialDecal,
		NULL
	);

}

//=====================================================================
// 終了処理
//=====================================================================
void UninitTutorialDecal(void)
{
	if (g_pTexBuffTutorialDecal != NULL)
	{// テクスチャの破棄
		g_pTexBuffTutorialDecal->Release();
		g_pTexBuffTutorialDecal = NULL;
	}

	if (g_pVtxBuffTutorialDecal != NULL)
	{// 頂点バッファの破棄
		g_pVtxBuffTutorialDecal->Release();
		g_pVtxBuffTutorialDecal = NULL;
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateTutorialDecal(void)
{

}

//=====================================================================
// 描画処理
//=====================================================================
void DrawTutorialDecal(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D* pVtx;

	// 頂点バッファをロックして頂点情報へのポインタを取得
	g_pVtxBuffTutorialDecal->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	SetVertexPos(pVtx, g_TutorialDecal.obj);
	SetVertexRHW(pVtx, 1.0f);
	SetVertexColor(pVtx, g_TutorialDecal.obj.color);
	SetVertexTexturePos(pVtx);

	// 頂点バッファをアンロック
	g_pVtxBuffTutorialDecal->Unlock();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTutorialDecal, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_TutorialDecal.obj.bVisible)
	{// 表示状態
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTexBuffTutorialDecal);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}