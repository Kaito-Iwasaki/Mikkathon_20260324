//=====================================================================
// 
// 汎用関数 [util.h]
// Author : Kaito Iwasaki
//
//=====================================================================
#ifndef _UTIL_H_
#define _UTIL_H_

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
#define D3DXVECTOR3_ZERO			D3DXVECTOR3(0.0f, 0.0f, 0.0f)		// ベクトル初期値
#define D3DXVECTOR3_RIGHT			D3DXVECTOR3(1.0f, 0.0f, 0.0f)
#define D3DXVECTOR3_UP				D3DXVECTOR3(0.0f, 1.0f, 0.0f)
#define D3DXVECTOR3_FRONT			D3DXVECTOR3(0.0f, 0.0f, 1.0f)

#define D3DXCOLOR_ZERO				D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f)
#define D3DXCOLOR_WHITE				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
#define D3DXCOLOR_BLACK				D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)
#define D3DXCOLOR_ALPHA(alpha)		D3DXCOLOR(0.0f, 0.0f, 0.0f, alpha)
#define D3DXCOLOR_GRAY(brightness)	D3DXCOLOR(brightness, brightness, brightness, 1.0f)

#define PI					D3DX_PI
#define RAD(degree)			D3DXToRadian(degree)
#define DEGREE(rad)			D3DXToRadian(rad)

#define BIT_CHECK(a,b)   (a >> b) & 1
#define BIT_SET(a,b)     a |= (1 << b)
#define BIT_UNSET(a,b)   a &= ~(1 << b)

#define RELEASE(p)	if (p != NULL) {p->Release(); p = nullptr;}

//*********************************************************************
// 
// ***** 列挙型 *****
// 
//*********************************************************************
typedef enum
{
	AXIS_X = 0,
	AXIS_Y,
	AXIS_Z,
	AXIS_MAX,
}AXIS;

typedef enum
{
	DIRECTION_FORWARD = 0,
	DIRECTION_BACKWARD,
	DIRECTION_LEFTWARD,
	DIRECTION_RIGHTWARD,
	DIRECTION_UPWARD,
	DIRECTION_DOWNWARD,
	DIRECTION_MAX,
}DIRECTION;

//*********************************************************************
// 
// ***** 構造体 *****
// 
//*********************************************************************
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 size;
}TRANSFORM;

//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************
int Clamp(int nNum, int nMin, int nMax);
bool Clamp(int* pNum, int nMin, int nMax);
float Clampf(float fNum, float fMin, float fMax);
bool Clampf(float* pNum, float fMin, float fMax);
float Lerpf(float a, float b, float t);
D3DXVECTOR3 Lerp(D3DXVECTOR3 vecA, D3DXVECTOR3 vecB, float t);
int RandRange(int nMin, int nMax);
float DotProduct(D3DXVECTOR2 vectorA, D3DXVECTOR2 vectorB);
float DotProduct(D3DXVECTOR3 vectorA, D3DXVECTOR3 vectorB);
D3DXVECTOR3 CrossProduct(D3DXVECTOR3 vectorA, D3DXVECTOR3 vectorB);
D3DXVECTOR3 Normalize(D3DXVECTOR3 vector);
D3DXVECTOR3 Direction(D3DXVECTOR3 from, D3DXVECTOR3 to);
D3DXVECTOR3 Direction(float fAngle);
D3DXVECTOR3 GetRandomVector(void);
float Angle(float fromX, float fromY, float toX, float toY);
float GetFixedRotation(float fRot);
D3DXVECTOR3 GetFixedRotation(D3DXVECTOR3 rot);
float Magnitude(D3DXVECTOR3 vector);
float Magnitude(D3DXVECTOR3 from, D3DXVECTOR3 to);
D3DXVECTOR2 Vector3To2(D3DXVECTOR3 source);
D3DXVECTOR3 Vector2To3(D3DXVECTOR2 source, float fValueZ = 0.0f);
void MatrixRotationPosition(D3DXMATRIX* pOut, D3DXVECTOR3 rot, D3DXVECTOR3 pos);
void MatrixRotationPosition(D3DXMATRIX* pOut, float fRotX, float fRotY, float fRotZ, float fPosX, float fPosY, float fPosZ);
bool IsCrossingVector(D3DXVECTOR3 vec0, D3DXVECTOR3 vec1, D3DXVECTOR3 pos, D3DXVECTOR3 posOld, D3DXVECTOR3* vecHit);
D3DXVECTOR3 ScreenToWorld(D3DXVECTOR2 posScreen, D3DXVECTOR3 posPlane, D3DXVECTOR3 norPlane);
bool LoadBin(const char* pFilePath, void* pBuffer, size_t ElementSize, size_t ElementCount);
bool SaveBin(const char* pFilePath, void* pBuffer, size_t ElementSize, size_t ElementCount);

#endif