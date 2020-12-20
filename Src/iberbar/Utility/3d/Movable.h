//*=============================================================================
//*
//*  Name: b3dassist header
//*  Dest: The main writing some auxiliary class and the auxiliary structure,
//*        and contains an abstract base class
//*
//*  Edited at 2014.9 -- it's old version
//*  Need to improve it.
//*=============================================================================

#pragma once

#include <d3dx9math.h>



#define null NULL
#define BFLT_MAX FLT_MAX
#define BFLT_MIN 0.001f
#define STATIC_CONST  static const

#define BSQRT_2 1.4142f
#define BSQRT_3 1.7320f



class CMovable3d;








//*=======================================================================
//|
//|                   3维基础变换物体基类
//|
//*=======================================================================

#define B3D_MBOBJ_TRANS_MOVE   0
#define B3D_MBOBJ_TRANS_RM     1
#define B3D_MBOBJ_TRANS_RV     2
#define B3D_MBOBJ_TRANS_RB     3
#define B3D_MBOBJ_TRANS_TUM    4
#define B3D_MBOBJ_TRANS_TUV    5
#define B3D_MBOBJ_TRANS_TUB    6
#define B3D_MBOBJ_TRANS_TRM    7
#define B3D_MBOBJ_TRANS_TRV    8
#define B3D_MBOBJ_TRANS_TRB    9

class CMovable3d
{

public:
	CMovable3d( void );


public:
	void SetDirectionBoth( const D3DXVECTOR3& vFace, const D3DXVECTOR3& vUp );
	void SetDirectionMove( const D3DXVECTOR3& vFace, const D3DXVECTOR3& vUp );
	void SetDirectionView( const D3DXVECTOR3& vFace, const D3DXVECTOR3& vUp );
	void SetDirectionMoveSameAsView();
	void SetDirectionViewSameAsMove();
	void SetPos( const D3DXVECTOR3& pos );

	void GetPos( D3DXVECTOR3* pOutPos )    const   { *pOutPos = m_ptPosition; }
	void GetDirectionMove( D3DXVECTOR3* pFace, D3DXVECTOR3* pLeft, D3DXVECTOR3* pUp ) const;
	void GetDirectionView( D3DXVECTOR3* pFace, D3DXVECTOR3* pLeft, D3DXVECTOR3* pUp ) const;

	// 只根据移动向量来变换位置
	void TranslateOnXYZ( float fx, float fy, float fz );
	void MoveLeft( float fDistance );
	void MoveFront( float fDistance );
	void MoveUp( float fDistance );

	// 绕着前向量旋转
	void RollBoth( float fAngle );
	void RollMove( float fAngle );
	void RollView( float fAngle );

	// 绕着左向量旋转
	void TurnUpBoth( float fAngle );
	void TurnUpMove( float fAngle );
	void TurnUpView( float fAngle );

	// 绕着上向量旋转
	void TurnRightBoth( float fAngle );
	void TurnRightMove( float fAngle );
	void TurnRightView( float fAngle );
	void TurnRightViewKeepElevation( float fAngle ); //移动向量正常变换，而视觉向量绕着移动向量Up旋转

	void RotationXYZOnItsPos( float fAngleX, float fAngleY, float fAngleZ );
	void RotationAxis( const D3DXVECTOR3& pt1, const D3DXVECTOR3& pt2 );


protected:
	D3DXVECTOR3  m_ptPosition;   // 物体位置
	D3DXVECTOR3  m_vecMoveFace;  // 移动前向量
	D3DXVECTOR3  m_vecMoveUp;    // 移动上向量
	D3DXVECTOR3  m_vecViewFace;  // 视觉前向量
	D3DXVECTOR3  m_vecViewUp;    // 视觉上向量
};






