//=====================================================================
//
// Enemy [Enemy.cpp]
// Author : Kaito Iwasaki
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "Enemy.h"
#include "Player.h"
#include "bulletGenerator.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define TEXTURE_FILENAME	NULL
#define INIT_POS			D3DXVECTOR3(100.0f, 100.0f, 0.0f)
#define INIT_SIZE			D3DXVECTOR3(100.0f, 100.0f, 0.0f)
#define INIT_COLOR			D3DXCOLOR(0, 0, 1, 1)

#define COLOR_NORMAL		INIT_COLOR
#define COLOR_DAMAGED		D3DXCOLOR(1, 0, 0, 1)

#define ENEMY_INIT_LIFE		(100)

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
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffEnemy = NULL;
ENEMY g_aEnemy[MAX_ENEMY];

//=====================================================================
// 初期化処理
//=====================================================================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 構造体の初期化
	memset(&g_aEnemy, 0, sizeof(ENEMY) * MAX_ENEMY);
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		g_aEnemy[i].obj.pos = INIT_POS;
		g_aEnemy[i].obj.size = INIT_SIZE;
		g_aEnemy[i].obj.color = INIT_COLOR;
		g_aEnemy[i].obj.bVisible = true;
	}

	// テクスチャの読み込み
	if (TEXTURE_FILENAME)
	{
		D3DXCreateTextureFromFile(
			pDevice,
			TEXTURE_FILENAME,
			&g_pTexBuffEnemy
		);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL
	);

}

//=====================================================================
// 終了処理
//=====================================================================
void UninitEnemy(void)
{
	if (g_pTexBuffEnemy != NULL)
	{// テクスチャの破棄
		g_pTexBuffEnemy->Release();
		g_pTexBuffEnemy = NULL;
	}

	if (g_pVtxBuffEnemy != NULL)
	{// 頂点バッファの破棄
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateEnemy(void)
{
	PLAYER* pPlayer = GetPlayer();

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (!g_aEnemy[i].bUsed) continue;

		if (g_aEnemy[i].state == ENEMYSTATE_DAMAGE)
		{
			SetEnemyState(&g_aEnemy[i], ENEMYSTATE_NORMAL);
		}
		else if (Magnitude(g_aEnemy[i].obj.pos, pPlayer->obj.pos) < (g_aEnemy[i].obj.size.x * 0.5f + pPlayer->obj.size.x * 0.5f) * 2)
		{
			DamageEnemy(&g_aEnemy[i]);
			g_aEnemy[i].obj.pos += Direction(pPlayer->obj.pos, g_aEnemy[i].obj.pos) * PLAYER_SPEED * 0.75f;

			D3DXVECTOR3 punch_start = pPlayer->obj.pos + Vector2To3(GetRandomVector2() * 100);
			D3DXVECTOR3 punch_dir = Direction(punch_start, g_aEnemy[i].obj.pos);
			D3DXVECTOR3 punch_rot = D3DXVECTOR3(0, 0, atan2f(punch_dir.x, punch_dir.y));
			//D3DXVECTOR3 moveDir = Vector2To3(GetRandomVector2());
			//D3DXVECTOR3 moveRot = D3DXVECTOR3(0, 0, )

			GenerateBullet(punch_start, punch_rot, 10, 0, BT_TEST);
		}



		switch (g_aEnemy[i].state)
		{
		case ENEMYSTATE_NORMAL:
			g_aEnemy[i].obj.color = COLOR_NORMAL;
			break;

		case ENEMYSTATE_DAMAGE:
			g_aEnemy[i].obj.color = COLOR_DAMAGED;
			break;

		default:
			break;
		}
	}
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D* pVtx;

	// 頂点バッファをロックして頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	for (int i = 0; i < MAX_ENEMY; i++, pVtx += 4)
	{
		if (!g_aEnemy[i].obj.bVisible || !g_aEnemy[i].bUsed) continue;

		SetVertexPos(pVtx, g_aEnemy[i].obj);
		SetVertexRHW(pVtx, 1.0f);
		SetVertexColor(pVtx, g_aEnemy[i].obj.color);
		SetVertexTexturePos(pVtx);
	}

	// 頂点バッファをアンロック
	g_pVtxBuffEnemy->Unlock();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (!g_aEnemy[i].obj.bVisible || !g_aEnemy[i].bUsed) continue;

		// テクスチャの設定
		pDevice->SetTexture(0, NULL);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
	}
}

//=====================================================================
// 敵設定処理
//=====================================================================
ENEMY* SetEnemy(D3DXVECTOR3 pos)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		if (g_aEnemy[i].bUsed) continue;

		ZeroMemory(&g_aEnemy[i], sizeof(ENEMY));
		g_aEnemy[i].bUsed = true;
		g_aEnemy[i].obj.pos = pos;
		g_aEnemy[i].obj.size = INIT_SIZE;
		g_aEnemy[i].obj.color = INIT_COLOR;
		g_aEnemy[i].obj.bVisible = true;
		g_aEnemy[i].nLife = ENEMY_INIT_LIFE;

		return &g_aEnemy[i];
	}

	return NULL;
}

//=====================================================================
// 敵取得処理
//=====================================================================
ENEMY* GetEnemy(D3DXVECTOR3 pos)
{
	return &g_aEnemy[0];
}

//=====================================================================
// 敵ダメージ処理
//=====================================================================
void DamageEnemy(ENEMY* pEnemy)
{
	SetEnemyState(pEnemy, ENEMYSTATE_DAMAGE);
	pEnemy->nLife--;

	if (pEnemy->nLife < 0)
	{
		pEnemy->bUsed = false;
	}
}

//=====================================================================
// 敵状態設定処理
//=====================================================================
void SetEnemyState(ENEMY* pEnemy, ENEMYSTATE newState)
{
	pEnemy->state = newState;
	pEnemy->nConunterState = 0;
}