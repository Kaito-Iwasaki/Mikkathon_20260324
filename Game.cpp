//=====================================================================
//
// Game [Game.cpp]
// Author : 
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "Game.h"
#include "Player.h"
#include "Enemy.h"
#include "bulletManager.h"
#include "Item.h"
#include "WeightFollow.h"
#include "Background.h"
#include "enemyGenerator.h"
#include "input.h"
#include "camera.h"
#include "DebugProc.h"

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


//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
bool g_bPauseGame = false;

//=====================================================================
// 初期化処理
//=====================================================================
void InitGame(void)
{
	// -- Objects --
	InitPlayer();
	InitEnemy();
	InitItem();
	InitWeightFollow();
	InitBackground();
  
	SetEnemy(D3DXVECTOR3(500, 500, 0));
  
	// -- Managers --
	InitBulletManager();
	
	// -- Generators --
	InitEnemyGenerator();

	// -- 構造体・グローバル変数 --
	g_bPauseGame = false;
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitGame(void)
{
	// -- Objects --
	UninitPlayer();
	UninitEnemy();
	UninitItem();
	UninitEnemy();
	UninitWeightFollow();
	UninitBackground();
	
	// -- Managers --
	UninitBulletManager();
	
	// -- Generators --
	UninitEnemyGenerator();
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateGame(void)
{
	// -- デバッグ表示 --
	PLAYER* pPlayer = GetPlayer();
	CAMERA* pCamera = GetCamera();

	PrintDebugProc("------------------------------------------------------------\n");
	PrintDebugProc("[ ゲーム画面 ]\n");
	PrintDebugProc("------------------------------------------------------------\n");
	PrintDebugProc("[ カメラ位置 ]: ( %f, %f, %f )\n", pCamera->pos.x, pCamera->pos.y, pCamera->pos.z);
	PrintDebugProc("[ プレイヤー位置 ]: ( %f, %f, %f )\n", pPlayer->obj.pos.x, pPlayer->obj.pos.y, pPlayer->obj.pos.z);
	PrintDebugProc("------------------------------------------------------------\n");

	//------------------------------------------------------------------

	// -- ポーズ切り替え --
	if (
		GetKeyboardTrigger(DIK_P) ||
		GetJoypadTrigger(JOYKEY_START)
		)
	{
		g_bPauseGame ^= 1;
	}

	//------------------------------------------------------------------

	if (!g_bPauseGame)
	{
		// -- Objects --
		UpdatePlayer();
		UpdateEnemy();
		UpdateItem();
		UpdateWeightFollow();
		UpdateBackground();

		// -- Managers --
		UpdateBulletManager();

		// -- Generators --
		UpdateEnemyGenerator();
	}
	else
	{

	}
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawGame(void)
{
	// -- 2D Objects --
	DrawBackground();
	DrawEnemy();
	DrawBulletManager();
	DrawItem();
	DrawWeightFollow();
	DrawPlayer();

	if (g_bPauseGame)
	{

	}
}