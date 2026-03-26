//=====================================================================
//
// gauge.cppのヘッダファイル [gauge.h]
// Author : Tenma Saito
// 
//=====================================================================
#ifndef _Gauge_H_
#define _Gauge_H_

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "main.h"
#include "baseObject.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** 列挙型 *****
// 
//*********************************************************************
typedef enum
{
	GAUGESTATE_NONE = 0,		// NULL
	GAUGESTATE_CHARGE,			// チャージ中
	GAUGESTATE_FULLCHARGE,		// チャージ完了
	GAUGESTATE_BURST,			// 解放中
	GAUGESTATE_MAX
}GAUGESTATE;


//*********************************************************************
// 
// ***** 構造体 *****
// 
//*********************************************************************
typedef struct Gauge
{
	BASEOBJECT obj;		// オブジェクト情報
	GAUGESTATE state;	// ゲージの状態
	float fLerp;		// ラープ変換用変数
	int nCounterState;	// 状態カウンター
	int nAnimationCounter;	// アニメーションカウンター
	bool bAlpha;		// α変動反転
}Gauge;

typedef Gauge *LPGAUGE, *PGAUGE;

typedef struct GAUGE_CONST
{
	static const D3DXVECTOR3 DefPosition;			// 位置
	static const D3DXVECTOR2 DefSize;				// 基本サイズ
	static const int aStateCount[GAUGESTATE_MAX];	// 各状態の状態カウント
	static const int nGaugePhase;					// ゲージ段階数
	static const float fAnimSpeed;					// アニメーションスピード
} GAUGE_CONST;

//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************
void InitGauge(void);
void UninitGauge(void);
void UpdateGauge(void);
void DrawGauge(void);
LPGAUGE GetGaugePtr(void);							// ポインタ取得
void AddGauge(int nValue);
#endif