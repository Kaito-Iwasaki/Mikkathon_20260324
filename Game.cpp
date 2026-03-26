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
#include "Background2.h"
#include "pause.h"
#include "font.h"
#include "gauge.h"

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
FONT* g_pFontScore = NULL;

//=====================================================================
// 初期化処理
//=====================================================================
void InitGame(void)
{

	// -- Managers --
	InitBulletManager();
	
	// -- Objects --
	InitLevelGenerator();
	InitCamera();
	InitPlayer();
	InitEnemy();
	InitItem();
	InitWeightFollow();
	InitBackground();
	InitBackground2();
	InitEffect();
	InitParticle();
	InitPause();
	InitFont();
	InitGauge();

	// -- Generators --
	InitEnemyGenerator();

	// -- Ex --
	SetCursorMid();

	// -- 構造体・グローバル変数 --
	g_bPauseGame = false;

	g_pFontScore = SetFont(
		FONT_LABEL_DONGURI,
		D3DXVECTOR3(0, 0, 0),
		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0),
		D3DXCOLOR(1,1,0,1),
		50,
		"",
		DT_LEFT | DT_TOP
	);
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
	UninitBackground2();
	UninitEffect();
	UninitParticle();
	UninitPause();
	UninitFont();
	UninitGauge();
	
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
		UpdateBackground2();
		UpdateEffect();
		UpdateParticle();
		UpdateGauge();

		// -- Managers --
		UpdateBulletManager();

		// -- Generators --
		UpdateEnemyGenerator();
		UpdateLevelGenerator();

		// プレイヤー位置の制限
		// 範囲をシーン依存にするためここで処理する
		Clampf(&pPlayer->obj.pos.x, -GAME_STAGE_SIZE.x, GAME_STAGE_SIZE.x);
		Clampf(&pPlayer->obj.pos.y, -GAME_STAGE_SIZE.y, GAME_STAGE_SIZE.y);

		sprintf(&g_pFontScore->aText[0], "Score: %d", pPlayer->nScore);
	}
	else
	{
		UpdatePause();
	}
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawGame(void)
{
	// -- 2D Objects --
	DrawBackground2();
	DrawBackground();
	DrawEffect();
	DrawEnemy();
	DrawBulletManager();
	DrawItem();
	DrawWeightFollow();
	DrawPlayer();
	DrawLevelGenerator();
	DrawFont(g_pFontScore);
	DrawGauge();

	if (g_bPauseGame)
	{// ポーズ画面描画
		DrawPause();
	}
}

//=====================================================================
//ポーズ画面切り替え処理
//=====================================================================
void TogglePauseGame(bool bPause)
{
	g_bPauseGame = bPause;
	SetPauseMenuCursor(0);
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