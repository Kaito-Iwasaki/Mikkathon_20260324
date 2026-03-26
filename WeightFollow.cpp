//=====================================================================
//
// WeightFollow [WeightFollow.cpp]
// Author : Kaito Iwasaki
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "WeightFollow.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define TEXTURE_FILENAME	"data\\TEXTURE\\Dumbbell.png"
#define INIT_POS			D3DXVECTOR3(100.0f, 100.0f, 0.0f)
#define INIT_SIZE			D3DXVECTOR3(100.0f, 100.0f, 0.0f)
#define INIT_COLOR			D3DXCOLOR_WHITE

#define WEIGHTFOLLOW_GAP	(30.0f)

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
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWeightFollow = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffWeightFollow = NULL;
WEIGHTFOLLOW g_aWeightFollow[PLAYER_MAX_HOLDABLE_BULLET];

//=====================================================================
// 初期化処理
//=====================================================================
void InitWeightFollow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 構造体の初期化
	memset(&g_aWeightFollow[0], 0, sizeof(WEIGHTFOLLOW) * MAX_WEIGHTFOLLOW);
	for (int i = 0; i < MAX_WEIGHTFOLLOW; i++)
	{
		g_aWeightFollow[i].obj.pos = INIT_POS;
		g_aWeightFollow[i].obj.size = INIT_SIZE;
		g_aWeightFollow[i].obj.color = INIT_COLOR;
		g_aWeightFollow[i].obj.bVisible = true;
	}

	// テクスチャの読み込み
	if (TEXTURE_FILENAME)
	{
		D3DXCreateTextureFromFile(
			pDevice,
			TEXTURE_FILENAME,
			&g_pTexBuffWeightFollow
		);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_WEIGHTFOLLOW,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWeightFollow,
		NULL
	);

}

//=====================================================================
// 終了処理
//=====================================================================
void UninitWeightFollow(void)
{
	if (g_pTexBuffWeightFollow != NULL)
	{// テクスチャの破棄
		g_pTexBuffWeightFollow->Release();
		g_pTexBuffWeightFollow = NULL;
	}

	if (g_pVtxBuffWeightFollow != NULL)
	{// 頂点バッファの破棄
		g_pVtxBuffWeightFollow->Release();
		g_pVtxBuffWeightFollow = NULL;
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateWeightFollow(void)
{
	PLAYER* pPlayer = GetPlayer();
	if (pPlayer->state == PLAYERSTATE_SMASH) return;

	for (int i = 0; i < pPlayer->nMaxBullet; i++)
	{
		D3DXVECTOR3 destination;
		D3DXVECTOR3 direction;

		if (i == 0)
		{
			destination = pPlayer->obj.pos;
		}
		else
		{
			destination = g_aWeightFollow[i - 1].obj.pos;
		}

		direction = Normalize(destination - g_aWeightFollow[i].obj.pos);

		if (Magnitude(g_aWeightFollow[i].obj.pos, destination) > WEIGHTFOLLOW_GAP)
		{
			g_aWeightFollow[i].obj.pos += direction * pPlayer->fSpeed;
		}

		g_aWeightFollow[i].obj.rot.z = atan2f(direction.x, direction.y);
	}
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawWeightFollow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D* pVtx;
	PLAYER* pPlayer = GetPlayer();

	// 頂点バッファをロックして頂点情報へのポインタを取得
	g_pVtxBuffWeightFollow->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	for (int i = 0; i < pPlayer->nBulletLeft; i++, pVtx += 4)
	{
		SetVertexPos(pVtx, g_aWeightFollow[i].obj);
		SetVertexRHW(pVtx, 1.0f);
		SetVertexColor(pVtx, g_aWeightFollow[i].obj.color);
		SetVertexTexturePos(pVtx);
	}

	// 頂点バッファをアンロック
	g_pVtxBuffWeightFollow->Unlock();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffWeightFollow, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < pPlayer->nBulletLeft; i++)
	{
		if (g_aWeightFollow[i].obj.bVisible)
		{// 表示状態
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTexBuffWeightFollow);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
		}
	}
}