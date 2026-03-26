//=============================================================================
//
// サウンド処理 [sound.h]
// Author : KAITO IWASAKI
// Special Thanks : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンド一覧
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_SE_PUNCH_00 = 0,
	SOUND_LABEL_SE_PUNCH_01,
	SOUND_LABEL_SE_PUNCH_02,
	SOUND_LABEL_SE_SMASH,
	SOUND_LABEL_SE_PLAYERSMASH,
	SOUND_LABEL_SE_EXPLOSION,
	SOUND_LABEL_SE_EAT,
	SOUND_LABEL_BGM_TITLE,
	SOUND_LABEL_BGM_TUTORIAL,
	SOUND_LABEL_BGM_GAME,
	SOUND_LABEL_BGM_RESULT,
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
