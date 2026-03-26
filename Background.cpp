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
#include "scene.h"
#include "Result.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define TEXTURE_FILENAME	"data\\TEXTURE\\background.jpg"
#define INIT_POS			D3DXVECTOR3(0.0f, 0.0f, 0.0f)
#define INIT_SIZE			D3DXVECTOR3(3000.0f, 3000.0f, 0.0f)
#define INIT_COLOR			D3DXCOLOR_WHITE
#define UNRESULTSCENE_NUM	(3)
#define TEXTURE_NUM			(UNRESULTSCENE_NUM + ED_MAX)
#define RELEASE_ARRAY(pp, num) do{ for(int i = 0; i < num; i++){ RELEASE(pp[i]); } }while(0)	// 配列バッファ解放(配列の先頭アドレス, 配列数)

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
LPDIRECT3DTEXTURE9 g_apTexBuffBackground[TEXTURE_NUM] = {};
BackgroundStruct g_Background;

const char *g_apTextureBG[TEXTURE_NUM] =
{
	"data/TEXTURE/title.jpg",
	"data/TEXTURE/background.jpg",
	"data/TEXTURE/background.jpg",
	"data/TEXTURE/ResultB.jpg",
	"data/TEXTURE/ResultA.jpg",
	"data/TEXTURE/ResultS.jpg",
};

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
	for (int nCntTex = 0; nCntTex < TEXTURE_NUM; nCntTex++)
	{
		if (g_apTextureBG[nCntTex])
		{
			D3DXCreateTextureFromFile(
				pDevice,
				g_apTextureBG[nCntTex],
				&g_apTexBuffBackground[nCntTex]
			);
		}
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
	// テクスチャの解放
	RELEASE_ARRAY(g_apTexBuffBackground, TEXTURE_NUM);

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
	if (GetCurrentScene() != SCENE_GAME && GetCurrentScene() != SCENE_TUTORIAL)
	{
		pVtx[0].pos = D3DXVECTOR3(0, 0, 0);
		pVtx[1].pos = D3DXVECTOR3(1280, 0, 0);
		pVtx[2].pos = D3DXVECTOR3(0, 720, 0);
		pVtx[3].pos = D3DXVECTOR3(1280, 720, 0);
	}

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
		int nIdxTexture = 0;
		if (GetCurrentScene() != SCENE_RESULT)
		{
			nIdxTexture = GetCurrentScene();
		}
		else
		{
			nIdxTexture = UNRESULTSCENE_NUM + GetEndingType();
		}

		// テクスチャの設定
		pDevice->SetTexture(0, g_apTexBuffBackground[nIdxTexture]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}