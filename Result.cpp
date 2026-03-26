//=====================================================================
//
// Result [Result.cpp]
// Author : Tenma Saito
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "Result.h"
#include "fade.h"
#include "input.h"
#include "Background.h"
#include "sound.h"
#include "font.h"
#include "Player.h"
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
typedef enum
{
	RESULTTYPE_KCAL = 0,	// 消費カロリー
	RESULTTYPE_KG,			// 体重
	RESULTTYPE_MAX
}RESULTTYPE;

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
ENDINGTYPE g_edType = ED_B;
FONT *g_apFontResult[RESULTTYPE_MAX] = {};

//=====================================================================
// 初期化処理
//=====================================================================
void InitResult(void)
{
	StopSound();
	PlaySound(SOUND_LABEL_BGM_RESULT);

	// -- Objects --
	InitBackground();
	InitFont();

	// -- 構造体・グローバル変数 --
	g_apFontResult[RESULTTYPE_KCAL] = SetFont(
		FONT_LABEL_TAMANEGI,
		D3DXVECTOR3(180, 265, 0),
		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0),
		D3DXCOLOR(0, 0, 0, 1),
		50,
		"",
		DT_CENTER | DT_TOP
	);

	g_apFontResult[RESULTTYPE_KG] = SetFont(
		FONT_LABEL_TAMANEGI,
		D3DXVECTOR3(180, 330, 0),
		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0),
		D3DXCOLOR(0, 0, 0, 1),
		80,
		"",
		DT_CENTER | DT_TOP
	);
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitResult(void)
{
	// -- Objects --
	UninitFont();
	UninitBackground();
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateResult(void)
{
	// 画面遷移
	if (GetMouseTrigger(MOUSE_LEFT)
		|| GetJoypadTrigger(JOYKEY_A)
		|| GetJoypadTrigger(JOYKEY_START))
	{
		SetFade(SCENE_TITLE);
	}

	// -- Objects --
	UpdateBackground();

	// 各スコアを表示
	sprintf(&g_apFontResult[RESULTTYPE_KCAL]->aText[0], "消費カロリー : %d Kcal", GetPlayer()->nScore);
	sprintf(&g_apFontResult[RESULTTYPE_KG]->aText[0], "筋肉 : %d", GetLevel(GetPlayer()->nIdxLevel));
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawResult(void)
{
	// -- Objects --
	DrawBackground();
	DrawFont(g_apFontResult[RESULTTYPE_KCAL]);
	DrawFont(g_apFontResult[RESULTTYPE_KG]);
}

//=====================================================================
// エンド取得処理
//=====================================================================
ENDINGTYPE GetEndingType(void)
{
	return g_edType;
}