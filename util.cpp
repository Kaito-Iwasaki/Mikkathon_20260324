//=====================================================================
// 
// 汎用関数 [util.cpp]
// Author : Kaito Iwasaki
//
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "util.h"

//=====================================================================
// 整数を範囲内に収める処理
//=====================================================================
int Clamp(int nNum, int nMin, int nMax)
{
	if (nNum < nMin)
	{
		return nMin;
	}
	else if (nNum > nMax)
	{
		return nMax;
	}

	return nNum;
}

//=====================================================================
// 整数を範囲内に収める処理（ポインタから直接編集）
//=====================================================================
bool Clamp(int* pNum, int nMin, int nMax)
{
	if (*pNum < nMin)
	{
		*pNum = nMin;
		return true;
	}
	else if (*pNum > nMax)
	{
		*pNum = nMax;
		return true;
	}

	return false;
}

//=====================================================================
// 小数を範囲内に収める処理
//=====================================================================
float Clampf(float fNum, float fMin, float fMax)
{
	if (fNum < fMin)
	{
		return fMin;
	}
	else if (fNum > fMax)
	{
		return fMax;
	}

	return fNum;
}

//=====================================================================
// 小数を範囲内に収める処理（ポインタから直接編集）
//=====================================================================
bool Clampf(float* pNum, float fMin, float fMax)
{
	if (*pNum < fMin)
	{
		*pNum = fMin;
		return true;
	}
	else if (*pNum > fMax)
	{
		*pNum = fMax;
		return true;
	}

	return false;
}

//=====================================================================
// 線形補間
//=====================================================================
float Lerpf(float a, float b, float t)
{
	return a * (1.0f - t) + b * t;
}

//=====================================================================
// 線形補間（ベクトル）
//=====================================================================
D3DXVECTOR3 Lerp(D3DXVECTOR3 vecA, D3DXVECTOR3 vecB, float t)
{
	return vecA * (1.0f - t) + vecB * t;
}

//=====================================================================
// ベクトルの内積（Vector2）
//=====================================================================
float DotProduct(D3DXVECTOR2 vectorA, D3DXVECTOR2 vectorB)
{
	return vectorA.x * vectorB.x + vectorA.y * vectorB.y;
}

//=====================================================================
// ベクトルの内積（Vector3）
//=====================================================================
float DotProduct(D3DXVECTOR3 vectorA, D3DXVECTOR3 vectorB)
{
	return vectorA.x * vectorB.x + vectorA.y * vectorB.y + vectorA.z * vectorB.z;
}

//=====================================================================
// ベクトルの外積
//=====================================================================
D3DXVECTOR3 CrossProduct(D3DXVECTOR3 vectorA, D3DXVECTOR3 vectorB)
{
	return D3DXVECTOR3(
		vectorA.y * vectorB.z - vectorA.z * vectorB.y,
		vectorA.z * vectorB.x - vectorA.x * vectorB.z,
		vectorA.x * vectorB.y - vectorA.y * vectorB.z
	);
}

//=====================================================================
// ベクトルの正規化処理
//=====================================================================
D3DXVECTOR3 Normalize(D3DXVECTOR3 vector)
{
	float fMagnitude = Magnitude(vector);

	if (fMagnitude != 0)
	{
		return D3DXVECTOR3(vector.x / fMagnitude, vector.y / fMagnitude, vector.z / fMagnitude);
	}
	else
	{
		return D3DXVECTOR3_ZERO;
	}
}

//=====================================================================
// 方向ベクトル（二つのベクトルから）
//=====================================================================
D3DXVECTOR3 Direction(D3DXVECTOR3 from, D3DXVECTOR3 to)
{
	return Normalize(to - from);
}

//=====================================================================
// 方向ベクトル（角度から）
//=====================================================================
D3DXVECTOR3 Direction(float fAngle)
{
	return D3DXVECTOR3(sin(fAngle), cos(fAngle), 0.0f);
}

//=====================================================================
// ランダムな方向のベクトルを生成
//=====================================================================
D3DXVECTOR3 GetRandomVector(void)
{
	float fTheta = RandRange(0, 627) * 0.01f;
	float fPhi = RandRange(0, 627) * 0.01f;;

	return D3DXVECTOR3(
		sin(fPhi) * sin(fTheta),
		cos(fTheta),
		cosf(fPhi) * sin(fTheta)
	);
}

//=====================================================================
// ベクトルの大きさ（距離）を求める処理
//=====================================================================
float Magnitude(D3DXVECTOR3 vector)
{
	return sqrtf(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}

//=====================================================================
// 二つのベクトルの大きさ（距離）を求める処理
//=====================================================================
float Magnitude(D3DXVECTOR3 from, D3DXVECTOR3 to)
{
	return Magnitude(to - from);
}

//=====================================================================
// 二つの線の角度を求める処理
//=====================================================================
float Angle(float fromX, float fromY, float toX, float toY)
{
	return atan2f(toX - fromX, toY - fromY);
}

//=====================================================================
// 角度の範囲を修正する処理
//=====================================================================
float GetFixedRotation(float fRot)
{
	// 角度の範囲を-3.14~3.14に収める
	if (fRot > D3DX_PI)
	{
		fRot -= D3DX_PI * 2;
	}
	else if (fRot < -D3DX_PI)
	{
		fRot += D3DX_PI * 2;
	}

	return fRot;
}

D3DXVECTOR3 GetFixedRotation(D3DXVECTOR3 rot)
{
	// 角度の範囲を-3.14~3.14に収める
	if (rot.x > D3DX_PI)
	{
		rot.x -= D3DX_PI * 2;
	}
	else if (rot.x < -D3DX_PI)
	{
		rot.x += D3DX_PI * 2;
	}

	if (rot.y > D3DX_PI)
	{
		rot.y -= D3DX_PI * 2;
	}
	else if (rot.y < -D3DX_PI)
	{
		rot.y += D3DX_PI * 2;
	}

	if (rot.z > D3DX_PI)
	{
		rot.z -= D3DX_PI * 2;
	}
	else if (rot.z < -D3DX_PI)
	{
		rot.z += D3DX_PI * 2;
	}

	return rot;
}

//=====================================================================
// 3次元ベクトルから2次元ベクトルへの変換処理
//=====================================================================
D3DXVECTOR2 Vector3To2(D3DXVECTOR3 source)
{
	return D3DXVECTOR2(source.x, source.y);
}

//=====================================================================
// 2次元ベクトルから3次元ベクトルへの変換処理
//=====================================================================
D3DXVECTOR3 Vector2To3(D3DXVECTOR2 source, float fValueZ)
{
	return D3DXVECTOR3(source.x, source.y, fValueZ);
}

//=====================================================================
// 回転、位置からのマトリックス設定処理
//=====================================================================
void MatrixRotationPosition(D3DXMATRIX* pOut, D3DXVECTOR3 rot, D3DXVECTOR3 pos)
{
	MatrixRotationPosition(pOut, rot.x, rot.y, rot.z, pos.x, pos.y, pos.z);
}

void MatrixRotationPosition(D3DXMATRIX* pOut, float fRotX, float fRotY, float fRotZ, float fPosX, float fPosY, float fPosZ)
{
	D3DXMATRIX mtxRot, mtxTrans;

	// 出力用のマトリックスを初期化
	D3DXMatrixIdentity(pOut);

	// 回転を適用
	D3DXMatrixRotationYawPitchRoll(&mtxRot, fRotY, fRotX, fRotZ);
	D3DXMatrixMultiply(pOut, pOut, &mtxRot);

	// 位置を適用
	D3DXMatrixTranslation(&mtxTrans, fPosX, fPosY, fPosZ);
	D3DXMatrixMultiply(pOut, pOut, &mtxTrans);
}

//=====================================================================
// ベクトルとベクトルの交差判定
//=====================================================================
bool IsCrossingVector(
	D3DXVECTOR3 vec0,		// 境界線ベクトル始点
	D3DXVECTOR3 vec1,		// 境界線ベクトル終点
	D3DXVECTOR3 pos,		// オブジェクト位置
	D3DXVECTOR3 posOld,		// オブジェクト過去位置
	D3DXVECTOR3* vecHit		// 衝突点のベクトルへのポインタ
)
{
	D3DXVECTOR3 vecLine = vec1 - vec0;		// 境界線ベクトル
	D3DXVECTOR3 vecMove = pos - posOld;		// 移動ベクトル
	D3DXVECTOR3 vecToPos = pos - vec0;		// vecLine始点からオブジェクト位置まで
	D3DXVECTOR3 vecToOld = posOld - vec0;	// vecLine始点からオブジェクト過去位置まで
	D3DXVECTOR3 vecNor = Normalize(D3DXVECTOR3(-vecLine.y, vecLine.x, 0));	// 法線ベクトル

	if (CrossProduct(vecLine, vecToOld).y >= 0 && CrossProduct(vecLine, vecToPos).y < 0)
	{
		if (vecHit != NULL)
		{
			float c0 = CrossProduct(vecToPos, vecMove).y;
			float c1 = CrossProduct(vecLine, vecMove).y;

			*vecHit = vecLine * (c0 / c1);
		}

		return true;
	}
	

	return false;
}

//=====================================================================
// スクリーン座標からワールド座標への変換処理
//=====================================================================
D3DXVECTOR3 ScreenToWorld(D3DXVECTOR2 posScreen, D3DXVECTOR3 posPlane, D3DXVECTOR3 norPlane)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView, mtxProj, mtxViewport;
	D3DXMATRIX mtxInv, mtxViewInv, mtxProjInv, mtxViewportInv;
	D3DXVECTOR3 worldPos;
	D3DXVECTOR3 posNear = D3DXVECTOR3(posScreen.x, posScreen.y, 0.0f);
	D3DXVECTOR3 posFar = D3DXVECTOR3(posScreen.x, posScreen.y, 1.0f);
	D3DXVECTOR3 vecRay;
	D3DXVECTOR3 vecNearToPlane = posPlane - posNear;

	// ワールド座標＝スクリーン座標×逆ビューポート行列×逆プロジェクション行列×逆ビュー行列

	// ビュー行列を取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// プロジェクション行列を取得
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProj);

	// ビューポート行列の取得
	D3DXMatrixIdentity(&mtxViewport);
	mtxViewport._11 = SCREEN_WIDTH;
	mtxViewport._22 = -SCREEN_HEIGHT;
	mtxViewport._41 = SCREEN_WIDTH;
	mtxViewport._42 = SCREEN_HEIGHT;

	// 各行列の逆行列を求める
	D3DXMatrixInverse(&mtxViewInv, NULL, &mtxView);
	D3DXMatrixInverse(&mtxProjInv, NULL, &mtxProj);
	D3DXMatrixInverse(&mtxViewportInv, NULL, &mtxViewport);
	mtxInv = mtxViewportInv * mtxProjInv * mtxView;	// １つにまとめる

	// ベクトルに行列を掛け合わせて最近点と最遠点を求める
	D3DXVec3TransformCoord(&posNear, &posNear, &mtxInv);
	D3DXVec3TransformCoord(&posFar, &posFar, &mtxInv);

	// 最近点と最遠点から光線ベクトルを求める
	vecRay = Normalize(posFar - posNear);

	if (vecRay < 0)
	{// 光線ベクトルが下向き＝平面と接しているなら

		// 射影長を使ってvecRayから衝突点までの距離を求める
		float fLengthProjRay = DotProduct(vecRay, norPlane);
		float fLengthProjToPlane = DotProduct(vecNearToPlane, norPlane);

		// 最近点＋光線ベクトル（方向）×衝突点までの距離
		worldPos = posNear + vecRay * (fLengthProjToPlane / fLengthProjRay);
	}
	else
	{// 平面と接していないので最遠点を指す
		worldPos = posFar;
	}

	return worldPos;
}

//=====================================================================
// 指定した範囲の乱数（整数）を取得する処理
//=====================================================================
int RandRange(int nMin, int nMax)
{
	return (rand() % (nMax - nMin)) + nMin;
}

//=====================================================================
// バイナリファイル読み込み処理
//=====================================================================
bool LoadBin(const char* pFilePath, void* pBuffer, size_t ElementSize, size_t ElementCount)
{
	FILE* pFile;

	pFile = fopen(pFilePath, "rb");

	if (pFile != NULL)
	{
		fread(pBuffer, ElementSize, ElementCount, pFile);

		fclose(pFile);

		return true;
	}
	else
	{
		return false;
	}
}

//=====================================================================
// バイナリファイル書き出し処理
//=====================================================================
bool SaveBin(const char* pFilePath, void* pBuffer, size_t ElementSize, size_t ElementCount)
{
	FILE* pFile;

	pFile = fopen(pFilePath, "wb");

	if (pFile != NULL)
	{
		fwrite(pBuffer, ElementSize, ElementCount, pFile);

		fclose(pFile);

		return true;
	}
	else
	{
		return false;
	}
}
