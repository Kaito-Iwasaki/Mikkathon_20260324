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
#include "effect.h"
#include "particle.h"
#include "LevelGenerator.h"

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
void SetCursorMid(void);

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
	InitCamera();
	InitPlayer();
	InitEnemy();
	InitItem();
	InitWeightFollow();
	InitBackground();
	InitEffect();
	InitParticle();
  
	// -- Managers --
	InitBulletManager();
	
	// -- Generators --
	InitEnemyGenerator();
	InitLevelGenerator();

	// -- Ex --
	SetCursorMid();
	GeneratorLevel(100, D3DXVECTOR3(640, 360, 0));

	// -- 構造体・グローバル変数 --
	g_bPauseGame = false;
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitGame(void)
{
	// -- Objects --
	UninitCamera();
	UninitPlayer();
	UninitEnemy();
	UninitItem();
	UninitEnemy();
	UninitWeightFollow();
	UninitBackground();
	UninitEffect();
	UninitParticle();
	
	// -- Managers --
	UninitBulletManager();
	
	// -- Generators --
	UninitEnemyGenerator();
	UninitLevelGenerator();
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
		UpdateCamera();
		UpdatePlayer();
		UpdateEnemy();
		UpdateItem();
		UpdateWeightFollow();
		UpdateBackground();
		UpdateEffect();
		UpdateParticle();

		// -- Managers --
		UpdateBulletManager();

		// -- Generators --
		UpdateEnemyGenerator();
		UpdateLevelGenerator();
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
	DrawEffect();
	DrawLevelGenerator();

	if (g_bPauseGame)
	{// ポーズ画面描画

	}
}

//=====================================================================
// マウスカーソルの初期位置中央移動処理
//=====================================================================
void SetCursorMid(void)
{
	WINDOWPLACEMENT place = {};		// ウィンドウの位置情報
	POINT winmid;					// ウィンドウの中央

	// ウィンドウの位置情報を取得
	GetWindowPlacement(GetMainWindow(), &place);

	// ウィンドウの中央を計算
	winmid.x = place.rcNormalPosition.right - SCREEN_CENTER;
	winmid.y = place.rcNormalPosition.bottom - SCREEN_VCENTER;

	// ウィンドウの中央にマウスカーソルを表示
	SetCursorPos(winmid.x, winmid.y);
}