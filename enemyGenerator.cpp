//=====================================================================
//
// enemyGenerator [enemyGenerator.cpp]
// Author : 
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "enemyGenerator.h"
#include "Enemy.h"
#include "Player.h"
#include "camera.h"
#include "Game.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************


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
void _Horde(int nNum);

//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
int g_nCountEnemyGen = 0;

//=====================================================================
// 初期化処理
//=====================================================================
void InitEnemyGenerator(void)
{
	g_nCountEnemyGen = 0;


	//SetEnemy(ENEMYTYPE_CHASER_BIG, D3DXVECTOR3(500, 500, 0));

	//SetEnemy(ENEMYTYPE_REFRECT_SMALL, D3DXVECTOR3(500, 500, 0));
	//SetEnemy(ENEMYTYPE_REFRECT_SMALL, D3DXVECTOR3(500, 400, 0));
	//SetEnemy(ENEMYTYPE_REFRECT_SMALL, D3DXVECTOR3(500, 300, 0));
	//SetEnemy(ENEMYTYPE_REFRECT_SMALL, D3DXVECTOR3(600, 200, 0));
	//SetEnemy(ENEMYTYPE_REFRECT_SMALL, D3DXVECTOR3(600, 100, 0));
	//SetEnemy(ENEMYTYPE_REFRECT_SMALL, D3DXVECTOR3(700, 100, 0));
	//SetEnemy(ENEMYTYPE_REFRECT_SMALL, D3DXVECTOR3(-700, -100, 0));
	//SetEnemy(ENEMYTYPE_REFRECT_SMALL, D3DXVECTOR3(-700, -100, 0));
	//SetEnemy(ENEMYTYPE_REFRECT_SMALL, D3DXVECTOR3(-700, -100, 0));
	//SetEnemy(ENEMYTYPE_REFRECT_SMALL, D3DXVECTOR3(-700, -100, 0));
	//SetEnemy(ENEMYTYPE_REFRECT_SMALL, D3DXVECTOR3(-700, -100, 0));
	//SetEnemy(ENEMYTYPE_REFRECT_SMALL, D3DXVECTOR3(-700, -700, 0));
	//SetEnemy(ENEMYTYPE_REFRECT_SMALL, D3DXVECTOR3(-700, -700, 0));
	//SetEnemy(ENEMYTYPE_REFRECT_SMALL, D3DXVECTOR3(-700, -700, 0));
	//SetEnemy(ENEMYTYPE_REFRECT_SMALL, D3DXVECTOR3(-700, -700, 0));
	//SetEnemy(ENEMYTYPE_REFRECT_SMALL, D3DXVECTOR3(-700, -700, 0));
	//SetEnemy(ENEMYTYPE_REFRECT_SMALL, D3DXVECTOR3(+700, +700, 0));
	//SetEnemy(ENEMYTYPE_REFRECT_SMALL, D3DXVECTOR3(+700, +700, 0));
	//SetEnemy(ENEMYTYPE_REFRECT_SMALL, D3DXVECTOR3(+700, +700, 0));
	//SetEnemy(ENEMYTYPE_REFRECT_SMALL, D3DXVECTOR3(+700, +700, 0));
	//SetEnemy(ENEMYTYPE_REFRECT_SMALL, D3DXVECTOR3(+700, +700, 0));
	//SetEnemy(ENEMYTYPE_REFRECT_SMALL, D3DXVECTOR3(-700, +100, 0));
	//SetEnemy(ENEMYTYPE_REFRECT_SMALL, D3DXVECTOR3(-700, +100, 0));
	//SetEnemy(ENEMYTYPE_REFRECT_SMALL, D3DXVECTOR3(-700, +100, 0));
	//SetEnemy(ENEMYTYPE_REFRECT_SMALL, D3DXVECTOR3(-700, +100, 0));
	//SetEnemy(ENEMYTYPE_REFRECT_SMALL, D3DXVECTOR3(-700, +700, 0));
	//SetEnemy(ENEMYTYPE_REFRECT_SMALL, D3DXVECTOR3(-700, +700, 0));
	//SetEnemy(ENEMYTYPE_REFRECT_SMALL, D3DXVECTOR3(-700, +700, 0));
	//SetEnemy(ENEMYTYPE_REFRECT_SMALL, D3DXVECTOR3(-700, +700, 0));
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitEnemyGenerator(void)
{

}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateEnemyGenerator(void)
{
	PLAYER* pPlayer = GetPlayer();

	D3DXVECTOR3 vecSpawn;

	if (g_nCountEnemyGen < 60 * 30)
	{// 120-90
		if (g_nCountEnemyGen % 120 == 0)
		{
			do
			{
				vecSpawn = D3DXVECTOR3(RandRange(-GAME_STAGE_SIZE.x, GAME_STAGE_SIZE.x), RandRange(-GAME_STAGE_SIZE.y, GAME_STAGE_SIZE.y), 0);
			} while (!IsOutOfScreen(vecSpawn, D3DXVECTOR3(100, 100, 0), GetCameraRect()));

			SetEnemy(ENEMYTYPE_CHASER_SMALL, vecSpawn);
			SetEnemy(ENEMYTYPE_REFRECT_SMALL, vecSpawn);
		}
	}
	else if (g_nCountEnemyGen < 60 * 90)
	{
		if (g_nCountEnemyGen % 120 == 0)
		{
			do
			{
				vecSpawn = D3DXVECTOR3(RandRange(-GAME_STAGE_SIZE.x, GAME_STAGE_SIZE.x), RandRange(-GAME_STAGE_SIZE.y, GAME_STAGE_SIZE.y), 0);
			} while (!IsOutOfScreen(vecSpawn, D3DXVECTOR3(100, 100, 0), GetCameraRect()));

			SetEnemy(ENEMYTYPE_CHASER_SMALL, vecSpawn);
			SetEnemy(ENEMYTYPE_REFRECT_SMALL, vecSpawn);

			do
			{
				vecSpawn = D3DXVECTOR3(
					RandRange(
						-GAME_STAGE_SIZE.x,
						GAME_STAGE_SIZE.x
					),
					RandRange(
						-GAME_STAGE_SIZE.y,
						GAME_STAGE_SIZE.y
					),
					0
				);
			} while (!IsOutOfScreen(vecSpawn, D3DXVECTOR3(100, 100, 0), GetCameraRect()));

			SetEnemy(ENEMYTYPE_CHASER, vecSpawn);
			SetEnemy(ENEMYTYPE_REFRECT, vecSpawn);
		}
	}
	else if (g_nCountEnemyGen > 60 * 90)
	{
		if (g_nCountEnemyGen % 40 == 0)
		{
			do
			{
				vecSpawn = D3DXVECTOR3(RandRange(-GAME_STAGE_SIZE.x, GAME_STAGE_SIZE.x), RandRange(-GAME_STAGE_SIZE.y, GAME_STAGE_SIZE.y), 0);
			} while (!IsOutOfScreen(vecSpawn, D3DXVECTOR3(100, 100, 0), GetCameraRect()));

			SetEnemy(ENEMYTYPE_CHASER_SMALL, vecSpawn);
			SetEnemy(ENEMYTYPE_REFRECT_SMALL, vecSpawn);


		}

		if (g_nCountEnemyGen % 60 == 0)
		{
			do
			{
				vecSpawn = D3DXVECTOR3(
					RandRange(
						-GAME_STAGE_SIZE.x,
						GAME_STAGE_SIZE.x
					),
					RandRange(
						-GAME_STAGE_SIZE.y,
						GAME_STAGE_SIZE.y
					),
					0
				);
			} while (!IsOutOfScreen(vecSpawn, D3DXVECTOR3(100, 100, 0), GetCameraRect()));

			SetEnemy(ENEMYTYPE_CHASER, vecSpawn);
			SetEnemy(ENEMYTYPE_REFRECT, vecSpawn);
		}

		if (g_nCountEnemyGen % 300 == 0)
		{
			do
			{
				vecSpawn = D3DXVECTOR3(
					RandRange(
						-GAME_STAGE_SIZE.x,
						GAME_STAGE_SIZE.x
					),
					RandRange(
						-GAME_STAGE_SIZE.y,
						GAME_STAGE_SIZE.y
					),
					0
				);
			} while (!IsOutOfScreen(vecSpawn, D3DXVECTOR3(100, 100, 0), GetCameraRect()));

			SetEnemy(ENEMYTYPE_CHASER_BIG, vecSpawn);
		}
	}


	if (g_nCountEnemyGen == 60 * 60)
	{
		_Horde(10);
	}

	g_nCountEnemyGen++;
}

void _Horde(int nNum)
{
	PLAYER* pPlayer = GetPlayer();
	for (int i = 0; i < nNum; i++)
	{
		float fRate = (float)i / (float)nNum;
		D3DXVECTOR3 vecSpawn = pPlayer->obj.pos + D3DXVECTOR3(sinf(D3DX_PI * 2 * fRate), cosf(D3DX_PI * 2 * fRate), 0) * 800;

		if (fabsf(vecSpawn.x) > GAME_STAGE_SIZE.x || fabsf(vecSpawn.y) > GAME_STAGE_SIZE.y)
		{
			continue;
		}
		SetEnemy(ENEMYTYPE_CHASER, vecSpawn);
	}
}