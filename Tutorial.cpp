//=====================================================================
//
// Tutorial [Tutorial.cpp]
// Author : Tenma Saito
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
// Scene
#include "Tutorial.h"
#include "Game.h"

// GameObject
#include "fade.h"
#include "input.h"
#include "Background.h"
#include "Background2.h"
#include "Player.h"
#include "Enemy.h"
#include "Item.h"
#include "WeightFollow.h"
#include "camera.h"
#include "effect.h"
#include "particle.h"
#include "fade.h"
#include "font.h"
#include "gauge.h"
#include "pause.h"

// Manager
#include "bulletManager.h"

// Generator
#include "itemGenerator.h"
#include "LevelGenerator.h"
#include "sound.h"
#include "TutorialDecal.h"


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
void SetCursorMid_Tutorial(void);

//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
bool g_bPauseTutorial = false;
FONT* g_pFontTutorial = NULL;

//=====================================================================
// 初期化処理
//=====================================================================
void InitTutorial(void)
{
	StopSound();
	PlaySound(SOUND_LABEL_BGM_TUTORIAL);

	// -- Managers --
	InitBulletManager();

	// -- Generators --
	InitLevelGenerator();

	// -- Objects --
	InitCamera();
	InitPlayer();
	InitEnemy();
	InitItem();
	InitWeightFollow();
	InitBackground();
	InitBackground2();
	InitEffect();
	InitParticle();
	InitGauge();
	InitPause();
	InitFont();
	InitTutorialDecal();

	// -- Ex --
	SetCursorMid_Tutorial();

	// 敵を出現
	SetEnemy(ENEMYTYPE_STATIC, GetPlayer()->obj.pos + D3DXVECTOR3(-400, 0, 0));
	SetEnemy(ENEMYTYPE_STATIC, GetPlayer()->obj.pos + D3DXVECTOR3(400, 0, 0));

	// アイテムを設置
	for (int nCntItem = 0; nCntItem < ITEMTYPE_MAX; nCntItem++)
	{
		SetItem(GetPlayer()->obj.pos + D3DXVECTOR3(600, 150 * nCntItem, 0),
			(ITEMTYPE)nCntItem,
			ITEM_CONST::DefColor,
			ITEM_CONST::DefSize + ITEM_CONST::DefSizeDiff * (nCntItem + 1));
	}

	g_bPauseTutorial = false;

	g_pFontTutorial = SetFont(
		FONT_LABEL_TAMANEGI,
		D3DXVECTOR3(0, 0, 0),
		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0),
		D3DXCOLOR(0, 0, 0, 1),
		60,
		"",
		DT_CENTER | DT_TOP
	);
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitTutorial(void)
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
	UninitGauge();
	UninitPause();
	UninitFont();
	UninitTutorialDecal();

	// -- Managers --
	UninitBulletManager();

	// -- Generators --
	UninitLevelGenerator();
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateTutorial(void)
{
	PLAYER *pPlayer = GetPlayer();

	// 画面遷移
	if (GetMouseTrigger(MOUSE_LEFT)
		|| GetJoypadTrigger(JOYKEY_A)
		|| GetJoypadTrigger(JOYKEY_START))
	{
		SetFade(SCENE_GAME);
	}

	//------------------------------------------------------------------

		// -- ポーズ切り替え --
	//if (
	//	GetKeyboardTrigger(DIK_P) ||
	//	GetJoypadTrigger(JOYKEY_START)
	//	)
	//{
	//	g_bPauseTutorial ^= 1;
	//}

	//------------------------------------------------------------------

	if (!g_bPauseTutorial)
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
		UpdateTutorialDecal();

		// -- Managers --
		UpdateBulletManager();

		// -- Generators --
		UpdateLevelGenerator();

		// プレイヤー位置の制限
		// 範囲をシーン依存にするためここで処理する
		Clampf(&pPlayer->obj.pos.x, -GAME_STAGE_SIZE.x * 0.1f, GAME_STAGE_SIZE.x * 0.5f);
		Clampf(&pPlayer->obj.pos.y, -GAME_STAGE_SIZE.y * 0.1f, GAME_STAGE_SIZE.y * 0.3f);

		ENEMY* pEnemy = GetEnemy();

		for (int i = 0; i < MAX_ENEMY; i++, pEnemy++)
		{
			Clampf(&pEnemy->obj.pos.x, -GAME_STAGE_SIZE.x * 0.1f, GAME_STAGE_SIZE.x * 0.5f);
			Clampf(&pEnemy->obj.pos.y, -GAME_STAGE_SIZE.y * 0.1f, GAME_STAGE_SIZE.y * 0.3f);
		}

		sprintf(&g_pFontTutorial->aText[0], "左クリック・A・STARTで開始");
	}
	else
	{
		UpdatePause();
	}
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawTutorial(void)
{
	// -- 2D Objects --
	DrawBackground2();
	DrawBackground();
	DrawTutorialDecal();
	DrawEffect();
	DrawEnemy();
	DrawBulletManager();
	DrawItem();
	DrawWeightFollow();
	DrawPlayer();
	DrawLevelGenerator();
	DrawGauge();
	DrawFont(g_pFontTutorial);

	if (g_bPauseTutorial)
	{// ポーズ画面描画
		DrawPause();
	}
}

//=====================================================================
// マウスカーソルの初期位置中央移動処理
//=====================================================================
void SetCursorMid_Tutorial(void)
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