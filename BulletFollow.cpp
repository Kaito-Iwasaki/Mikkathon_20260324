//=====================================================================
//
// BulletFollow [BulletFollow.cpp]
// Author : Kaito Iwasaki
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "BulletFollow.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define TEXTURE_FILENAME	NULL
#define INIT_POS			D3DXVECTOR3(100.0f, 100.0f, 0.0f)
#define INIT_SIZE			D3DXVECTOR3(10.0f, 10.0f, 0.0f)
#define INIT_COLOR			D3DXCOLOR(1, 0, 0, 1)

#define BULLETFOLLOW_GAP	(30.0f)

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
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBulletFollow = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffBulletFollow = NULL;
BULLETFOLLOW g_aBulletFollow[PLAYER_MAX_HOLDABLE_BULLET];

//=====================================================================
// 初期化処理
//=====================================================================
void InitBulletFollow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 構造体の初期化
	memset(&g_aBulletFollow[0], 0, sizeof(BULLETFOLLOW) * MAX_BULLETFOLLOW);
	for (int i = 0; i < MAX_BULLETFOLLOW; i++)
	{
		g_aBulletFollow[i].obj.pos = INIT_POS;
		g_aBulletFollow[i].obj.size = INIT_SIZE;
		g_aBulletFollow[i].obj.color = INIT_COLOR;
		g_aBulletFollow[i].obj.bVisible = true;
	}

	// テクスチャの読み込み
	if (TEXTURE_FILENAME)
	{
		D3DXCreateTextureFromFile(
			pDevice,
			TEXTURE_FILENAME,
			&g_pTexBuffBulletFollow
		);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_BULLETFOLLOW,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBulletFollow,
		NULL
	);

}

//=====================================================================
// 終了処理
//=====================================================================
void UninitBulletFollow(void)
{
	if (g_pTexBuffBulletFollow != NULL)
	{// テクスチャの破棄
		g_pTexBuffBulletFollow->Release();
		g_pTexBuffBulletFollow = NULL;
	}

	if (g_pVtxBuffBulletFollow != NULL)
	{// 頂点バッファの破棄
		g_pVtxBuffBulletFollow->Release();
		g_pVtxBuffBulletFollow = NULL;
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateBulletFollow(void)
{
	PLAYER* pPlayer = GetPlayer();

	for (int i = 0; i < pPlayer->nBulletLeft; i++)
	{
		D3DXVECTOR3 destination;
		D3DXVECTOR3 direction;

		if (i == 0)
		{
			destination = pPlayer->obj.pos;
		}
		else
		{
			destination = g_aBulletFollow[i - 1].obj.pos;
		}

		direction = Normalize(destination - g_aBulletFollow[i].obj.pos);

		if (Magnitude(g_aBulletFollow[i].obj.pos, destination) > BULLETFOLLOW_GAP)
		{
			g_aBulletFollow[i].obj.pos += direction * PLAYER_SPEED;
		}

		g_aBulletFollow[i].obj.rot.z = atan2f(direction.x, direction.y);
	}
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawBulletFollow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D* pVtx;
	PLAYER* pPlayer = GetPlayer();

	// 頂点バッファをロックして頂点情報へのポインタを取得
	g_pVtxBuffBulletFollow->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	for (int i = 0; i < pPlayer->nBulletLeft; i++, pVtx += 4)
	{
		SetVertexPos(pVtx, g_aBulletFollow[i].obj);
		SetVertexRHW(pVtx, 1.0f);
		SetVertexColor(pVtx, g_aBulletFollow[i].obj.color);
		SetVertexTexturePos(pVtx);
	}

	// 頂点バッファをアンロック
	g_pVtxBuffBulletFollow->Unlock();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBulletFollow, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < pPlayer->nBulletLeft; i++)
	{
		if (g_aBulletFollow[i].obj.bVisible)
		{// 表示状態
			// テクスチャの設定
			pDevice->SetTexture(0, NULL);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
		}
	}
}