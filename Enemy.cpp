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
#include "sound.h"
#include "Game.h"
#include "scene.h"

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

#define ENEMY_INIT_LIFE		(10)

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
void _InitEnemyParamsByType(ENEMY* pEnemy);
void _OnEnemyType(ENEMY* pEnemy);
void _OnEnemyState(ENEMY* pEnemy);
void _CollisionEnemyPlayer(ENEMY* pEnemy);
void _CollisionEnemyEnemy(ENEMY* pEnemy);

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

		// 敵の種類別処理
		_OnEnemyType(&g_aEnemy[i]);

		// 敵の状態別処理
		_OnEnemyState(&g_aEnemy[i]);

		// プレイヤーとの衝突判定
		_CollisionEnemyPlayer(&g_aEnemy[i]);

		// 敵同士で押し出し合って重ならないようにする
		_CollisionEnemyEnemy(&g_aEnemy[i]);
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
ENEMY* SetEnemy(ENEMYTYPE type, D3DXVECTOR3 pos)
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
		g_aEnemy[i].fSpeed = 3.0f;
		g_aEnemy[i].type = type;

		_InitEnemyParamsByType(&g_aEnemy[i]);

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

	if (pEnemy->nLife <= 0)
	{
		SmashEnemy(pEnemy);
		return true;
	}
	else
	{
		return false;
	}
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

	PlaySound(SOUND_LABEL_SE_EXPLOSION);

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

void _InitEnemyParamsByType(ENEMY* pEnemy)
{
	switch (pEnemy->type)
	{
	case ENEMYTYPE_STATIC:
		pEnemy->nLife = 200;
		pEnemy->nScore = 100;
		pEnemy->fSpeed = 0;
		break;

	case ENEMYTYPE_CHASER:
		pEnemy->nLife = 100;
		pEnemy->nScore = 100;
		pEnemy->fSpeed = 3;
		break;

	case ENEMYTYPE_REFRECT:
		pEnemy->move = Vector2To3(GetRandomVector2());
		pEnemy->nLife = 200;
		pEnemy->nScore = 200;
		pEnemy->fSpeed = 3;
		break;

	default:
		break;
	}
}

void _OnEnemyType(ENEMY* pEnemy)
{
	if (pEnemy->nLife <= 0) return;

	PLAYER* pPlayer = GetPlayer();
	if (pPlayer->state == PLAYERSTATE_SMASH) return;

	switch (pEnemy->type)
	{
	case ENEMYTYPE_STATIC:

		break;

	case ENEMYTYPE_CHASER:
	{
		D3DXVECTOR3 vecMoveDir = Direction(pEnemy->obj.pos, pPlayer->obj.pos);
		pEnemy->obj.pos += vecMoveDir * pEnemy->fSpeed;
		pEnemy->obj.rot.z = atan2(vecMoveDir.x, vecMoveDir.y);
		break;
	}

	case ENEMYTYPE_REFRECT:
		pEnemy->obj.pos += pEnemy->move * pEnemy->fSpeed;
		pEnemy->obj.rot.z = atan2(pEnemy->move.x, pEnemy->move.y);

		int fRangeX = 1500.0f;
		int fRangeY = 1500.0f;

		if (pEnemy->obj.pos.x < -fRangeX || pEnemy->obj.pos.x > fRangeX)
		{
			pEnemy->move.x *= -1;
		}
		if (pEnemy->obj.pos.y < -fRangeY || pEnemy->obj.pos.y > fRangeY)
		{
			pEnemy->move.y *= -1;
		}

		break;

	}

}

void _OnEnemyState(ENEMY* pEnemy)
{
	PLAYER* pPlayer = GetPlayer();
	D3DXVECTOR3 vecCameraPos = GetCameraPos();

	switch (pEnemy->state)
	{
	case ENEMYSTATE_NORMAL:
		pEnemy->obj.color = COLOR_NORMAL;
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

		if (IsObjectOutOfScreen(pEnemy->obj, GetCameraRect()) || 
			fabsf(pEnemy->obj.pos.x) >= GAME_STAGE_SIZE.x ||
			fabsf(pEnemy->obj.pos.y) >= GAME_STAGE_SIZE.y)
		{
			KillEnemy(pEnemy);
		}

		break;
	}

	default:
		break;
	}

	Clampf(&pEnemy->obj.pos.x, -GAME_STAGE_SIZE.x, GAME_STAGE_SIZE.x);
	Clampf(&pEnemy->obj.pos.y, -GAME_STAGE_SIZE.y, GAME_STAGE_SIZE.y);
	
	pEnemy->nConunterState++;
}

void _CollisionEnemyPlayer(ENEMY* pEnemy)
{
	PLAYER* pPlayer = GetPlayer();

	if (pPlayer->state == PLAYERSTATE_SMASH) return;

	D3DXVECTOR3 vecEnemyToPlayer = pPlayer->obj.pos - pEnemy->obj.pos;
	float sizeAAndB = pEnemy->obj.size.x * 0.5f + pPlayer->obj.size.x * 0.5f;

	if (Magnitude(vecEnemyToPlayer) < sizeAAndB)
	{
		if (pPlayer->state != PLAYERSTATE_SUPER)
		{
			SmashPlayer(Normalize(vecEnemyToPlayer));
		}
	}
}

void _CollisionEnemyEnemy(ENEMY* pEnemyA)
{
	ENEMY* pEnemyB;

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		pEnemyB = &g_aEnemy[i];
		if (pEnemyA == pEnemyB) continue;

		D3DXVECTOR3 vecEnemyAToEnemyB = pEnemyB->obj.pos - pEnemyA->obj.pos;
		float sizeAAndB = pEnemyA->obj.size.x * 0.5f + pEnemyB->obj.size.x * 0.5f;

		if (Magnitude(vecEnemyAToEnemyB) < sizeAAndB)
		{
			pEnemyB->obj.pos += Normalize(vecEnemyAToEnemyB);
		}
	}
}