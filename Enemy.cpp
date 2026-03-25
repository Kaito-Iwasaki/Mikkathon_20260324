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
#include "camera.h"
#include "effect.h"
#include "particle.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define TEXTURE_FILENAME	"data\\TEXTURE\\Enemy.png"
#define INIT_POS			D3DXVECTOR3(100.0f, 100.0f, 0.0f)
#define INIT_SIZE			D3DXVECTOR3(100.0f, 100.0f, 0.0f)
#define INIT_COLOR			D3DXCOLOR(1, 1, 1, 1)

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
void _OnEnemyState(ENEMY* pEnemy);

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
		if (g_aEnemy[i].bUsed == false) continue;

		_OnEnemyState(&g_aEnemy[i]);

		for (int j = 0; j < MAX_ENEMY; j++)
		{
			if (g_aEnemy[j].bUsed == false) continue;
			if (j == i) continue;

			D3DXVECTOR3 vecEnemyAToEnemyB = g_aEnemy[j].obj.pos - g_aEnemy[i].obj.pos;
			float sizeAAndB = g_aEnemy[i].obj.size.x * 0.5f + g_aEnemy[j].obj.size.x * 0.5f;

			if (Magnitude(vecEnemyAToEnemyB) < sizeAAndB)
			{
				g_aEnemy[j].obj.pos += Normalize(vecEnemyAToEnemyB);
			}
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
		pDevice->SetTexture(0, g_pTexBuffEnemy);

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
ENEMY* GetEnemy(void)
{
	return &g_aEnemy[0];
}

//=====================================================================
// 敵ダメージ処理
//=====================================================================
bool DamageEnemy(ENEMY* pEnemy, int nDamage)
{
	ShakeCamera(1);
	SetEnemyState(pEnemy, ENEMYSTATE_DAMAGE);
	pEnemy->nLife -= nDamage;

	if (pEnemy->nLife < 0)
	{
		SmashEnemy(pEnemy);
	}

	return !pEnemy->bUsed;
}

//=====================================================================
// 敵吹っ飛ばし処理
//=====================================================================
void SmashEnemy(ENEMY* pEnemy)
{
	PLAYER* pPlayer = GetPlayer();

	ShakeCamera(2);

	SetEnemyState(pEnemy, ENEMYSTATE_SMASH);
	pEnemy->obj.color = COLOR_NORMAL;
	pEnemy->move = Direction(pPlayer->obj.pos, pEnemy->obj.pos) * 20; 
	pEnemy->fRotMove = rand() % 2 == 0 ? 0.5f : -0.5f;
}

//=====================================================================
// 敵死亡処理
//=====================================================================
void KillEnemy(ENEMY* pEnemy)
{
	PLAYER* pPlayer = GetPlayer();

	ShakeCamera(10);

	EFFECTINFO info;
	info.col = D3DXCOLOR_WHITE;
	info.fMaxAlpha = 0.1f;
	info.fMaxScale = 1.3f;
	info.fRotSpeed = 0;
	info.fSpeed = 7.0f;
	info.nMaxLife = 60;

	SetParticle(info, pEnemy->obj.pos, 0, D3DX_PI, 1, 15);

	pEnemy->bUsed = false;
}

//=====================================================================
// 敵状態設定処理
//=====================================================================
void SetEnemyState(ENEMY* pEnemy, ENEMYSTATE newState)
{
	pEnemy->state = newState;
	pEnemy->nConunterState = 0;
}

void _OnEnemyState(ENEMY* pEnemy)
{
	PLAYER* pPlayer = GetPlayer();
	D3DXVECTOR3 vecCameraPos = GetCameraPos();

	switch (pEnemy->state)
	{
	case ENEMYSTATE_NORMAL:
		pEnemy->obj.color = COLOR_NORMAL;
		pEnemy->obj.pos += Direction(pEnemy->obj.pos, pPlayer->obj.pos) * 3.0f;
		break;

	case ENEMYSTATE_DAMAGE:
		if (pEnemy->obj.color != COLOR_DAMAGED)
		{
			pEnemy->obj.color = COLOR_DAMAGED;
		}
		else
		{
			pEnemy->obj.color = COLOR_NORMAL;
		}

		SetEnemyState(pEnemy, ENEMYSTATE_NORMAL);

		break;

	case ENEMYSTATE_SMASH:
	{
		pEnemy->obj.pos += pEnemy->move;
		pEnemy->obj.rot.z += pEnemy->fRotMove;

		EFFECTINFO info;
		info.col = D3DXCOLOR_WHITE;
		info.fMaxAlpha = 0.1f;
		info.fMaxScale = 0.7f;
		info.fRotSpeed = 0.1f;
		info.fSpeed = 0.5f;
		info.nMaxLife = 120;

		SetParticle(info, pEnemy->obj.pos, 0, D3DX_PI, 1, 1);

		if (IsObjectOutOfScreen(pEnemy->obj, GetCameraRect()))
		{
			KillEnemy(pEnemy);
		}

		break;
	}

	default:
		break;
	}
	
	pEnemy->nConunterState++;
}