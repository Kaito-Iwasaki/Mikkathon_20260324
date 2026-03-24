//=====================================================================
//
// Camera.cppのヘッダファイル [Camera.h]
// Author : Kaito Iwasaki
// 
//=====================================================================
#ifndef _Camera_H_
#define _Camera_H_

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "main.h"

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
//*********************************************************************
// カメラタイプ列挙型
//*********************************************************************
typedef enum
{
	CAMERATYPE_GAME = 0,
	CAMERATYPE_TIME,
	CAMERATYPE_MAX
}CAMERATYPE;

//*********************************************************************
// カメラモード列挙型
//*********************************************************************
typedef enum
{
	CAMERAMODE_NONE = 0,			// なし（プログラム制御）
	CAMERAMODE_SIDEVIEWFOCUS1,		// サイドビュー（１Pフォーカス）
	CAMERAMODE_SIDEVIEWFOCUS2,		// サイドビュー（２Pフォーカス）
	CAMERAMODE_SIDEVIEW2P,			// サイドビュー（２プレイヤーゲーム用）
	CAMERAMODE_FREE,				// 自由操作
	CAMERAMODE_MAX
}CAMERAMODE;

//*********************************************************************
// 
// ***** 構造体 *****
// 
//*********************************************************************
typedef struct CAMERA
{
	D3DXVECTOR3 posV;
	D3DXVECTOR3 posR;
	D3DXVECTOR3 vecU;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 offset;
	D3DXVECTOR3 move;
	CAMERAMODE mode;
	float fDistance;
	float fDistanceMove;
	D3DXMATRIX mtxProjection;	// プロジェクションマトリックス（投影行列）
	D3DXMATRIX mtxView;			// ビューマトリックス
	D3DVIEWPORT9 viewport;
	int nCountState;
};

//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(int nIdx);
CAMERA* GetCamera(int nIdx);
void MoveCamera(int nIdx, D3DXVECTOR3 move);
void SetCameraPos(int nIdx, D3DXVECTOR3 pos);
void SetCameraPosV(int nIdx, D3DXVECTOR3 pos);
void SetCameraPosR(int nIdx, D3DXVECTOR3 pos);
void SetCameraPosVFromAngle(int nIdx);
void SetCameraPosRFromAngle(int nIdx);
void RotateCameraByV(int nIdx, D3DXVECTOR3 rotMove);
void RotateCameraByR(int nIdx, D3DXVECTOR3 rotMove);

#endif