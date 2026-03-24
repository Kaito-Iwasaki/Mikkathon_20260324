//=====================================================================
//
// main.cppのヘッダファイル [main.cpp]
// Author : Kaito Iwasaki
// 
//=====================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "d3dx9.h"		// 描画処理に必要
#define DIRECTINPUT_VERSION		(0x0800)	
#include "dinput.h"		// 入力処理に必要
#include "xinput.h"		// ジョイパッド入力処理に必要
#include "xaudio2.h"	// サウンド処理に必要

//*********************************************************************
// 
// ライブラリへのリンク
// 
//*********************************************************************
#pragma comment(lib, "d3d9.lib")		// 描画処理に必要
#pragma comment(lib, "d3dx9.lib")		// 上の拡張ライブラリ
#pragma comment(lib, "dxguid.lib")		// DirextXコンポーネントの使用に必要
#pragma comment(lib, "winmm.lib")		// システム時刻取得に必要
#pragma comment(lib, "dinput8.lib")		// 入力処理に必要
#pragma comment(lib, "xinput.lib")		// ジョイパッド処理に必要

//*********************************************************************
// 
// マクロ定義
// 
//*********************************************************************
#define SCREEN_WIDTH	(1280)			// 画面幅
#define SCREEN_HEIGHT	(720)			// 画面高さ
#define SCREEN_CENTER	(SCREEN_WIDTH / 2)
#define SCREEN_VCENTER	(SCREEN_HEIGHT / 2)
#define WINDOWED		(true)			// ウィンドウ表示

#define FVF_VERTEX_2D		(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)	// 頂点フォーマット[2D]

//*********************************************************************
// 
// ***** 構造体 *****
// 
//*********************************************************************
//*********************************************************************
// 頂点情報[2D]構造体
//*********************************************************************
typedef struct
{
	D3DXVECTOR3 pos;
	float rhw;
	D3DCOLOR col;
	D3DXVECTOR2 tex;
}VERTEX_2D;

//*********************************************************************
// 
// プロトタイプ宣言
// 
//*********************************************************************
LPDIRECT3DDEVICE9 GetDevice(void);
LPDIRECTINPUT8 GetInput(void);
HWND GetMainWindow(void);

#endif
