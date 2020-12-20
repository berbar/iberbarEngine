
//*=============================================================================
//*  Edited at 2014.9 -- it's old version
//*  Need to improve it.
//*=============================================================================



#pragma once

#include <iberbar\Base\3d\Movable.h>

#define B3D_CAMERA_FPS_KEY_MOVEFRONT 0
#define B3D_CAMERA_FPS_KEY_MOVEBACK  1
#define B3D_CAMERA_FPS_KEY_MOVELEFT  2
#define B3D_CAMERA_FPS_KEY_MOVERIGHT 3
#define B3D_CAMERA_FPS_KEY_TURNUP    4
#define B3D_CAMERA_FPS_KEY_TURNFDOWN 5
#define B3D_CAMERA_FPS_KEY_TURNLEFT  6
#define B3D_CAMERA_FPS_KEY_TURNRIGHT 7


//---------------------------------------------------------------------
//  name : CB3DCameraBase
//  desc : 以下 camera 的基类
//---------------------------------------------------------------------
namespace iberbar
{
	class CDXInput;

	class CBaseCamera3d
		: public CMovable3d
	{
	protected:
		enum BCMR_TYPE {
			BCMR_TYPE_COMMON,
			BCMR_TYPE_FPSW,
			BCMR_TYPE_FPSF,
			BCMR_TYPE_OVERLOOK
		};


	public:
		CBaseCamera3d( void );

	public:
		void   SetFocusDistance( float fDis )
		{
			if ( fDis < 0 ) return;
			m_fFocusDistance = fDis;
		}
		void   MoveFrontFocus( float fDis )
		{
			m_fFocusDistance += fDis;
			m_fFocusDistance = ( ( m_fFocusDistance<0 ) ? 0:m_fFocusDistance );
		}
		void   SetPerspectiveMatrix( float fovy, float fAspect, float zn, float zf );

	public:
		float  GetFocusDistance() const{ return m_fFocusDistance; }
		float  GetFovy()          const{ return m_fovy; }
		float  GetAspect()        const{ return m_fAspect; }
		float  GetZNear()         const{ return m_fZnear; }
		float  GetZFar()          const{ return m_fZfar; }
		BCMR_TYPE GetCameraType() const{ return m_eCmrType; }

	public:
		void   GetViewMatrix( D3DXMATRIX* pOutMatrix );
		void   GetPerspectiveMatrix( D3DXMATRIXA16* pOutProj ) const;


	protected:
		BCMR_TYPE  m_eCmrType;
		bool       m_bWorking;   // 只有正在工作的所拍摄到的才被渲染
		float      m_fFocusDistance;
		float      m_fovy;
		float      m_fAspect;
		float      m_fZnear;
		float      m_fZfar;
	};











	//---------------------------------------------------------------------
	//  name : CB3DCameraOverlookM
	//  desc : 继承于 CB3DCamera,可接受消息并处理
	//         第三人称视觉，移动
	//---------------------------------------------------------------------
// #ifndef _B3D_CAMERA_FPS
// #define _B3D_CAMERA_FPS
// 

// 
// 	typedef class CB3DCameraFPSW *LPB3DCAMERA_FPSW;
// 	//typedef class CB3DCameraFPSF *LPB3DCAMERA_FPSF;
// 
// 	class CB3DCameraFPSW
// 		: public CB3DCamera
// 	{
// 	protected:
// 		bool m_bKeyPressed[ 4 ];
// 		INT     m_iKeyMove[ 4 ];
// 		INT     m_iKeyTurn[ 2 ];
// 		float   m_fSpeedMove;
// 		float   m_fSensitivity;
// 
// 	public:
// 		VOID   SetKeyMove( INT iKeyFront, INT iKeyBack, INT iKeyLeft, INT iKeyRight );
// 		VOID   SetKeyTurn( INT iKeyLeft, INT iKeyRight );
// 
// 		VOID   SetSpeedMove( float fSpeed )    { m_fSpeedMove = fSpeed; }
// 		VOID   SetSpeddTurn( float fSensitivity )    { m_fSensitivity = fSensitivity; }
// 
// 	public:
// 		INT GetKey( INT iKeyIndex )  const;
// 
// 		inline float GetSpeedMove()    const{ return m_fSpeedMove; }
// 		inline float GetSpeedTurn()    const{ return m_fSensitivity; }
// 
// 	public:
// 		BOOL   Update( const CBDirectInput* pBInput, float fElaspeTime );
// 
// 	public:
// 		CB3DCameraFPSW( VOID );
// 		~CB3DCameraFPSW();
// 	};
// 
// #endif










#ifndef _B3D_CAMERA_OVERLOOK
#define _B3D_CAMERA_OVERLOOK

#define BCMR_ANGLE_DR  225.0f
#define BCMR_ANGLE_DH  315.0f
#define BCMR_ANGLE_DL   90.0f

	class CViewerCamera
		: public CBaseCamera3d
	{
	public:
		struct Keyboard
		{
			bool bLButtonDown;
		};

	protected:
		INT    m_iKeyMove[ 4 ];
		bool  m_bMouseControlMove;
		bool  m_bMouseZControlAngle;

		float   m_fSpeedMove;
		float   m_fSensitivityMove;
		float   m_fSensitivityRotation;

		float   m_fTargetHeight;
		float   m_fDistanceFromTarget;
		float   m_fDistanceFromTargetParam;

		BYTE    m_btMainKeyChangeAngleZ;
		BYTE    m_btMainKeyChangeAngleAll;
		BYTE    m_btMainKeyMove;

		INT     m_iAssistKeyChangeAngleZ;
		INT     m_iAssistKeyChangeAngleAll;
		INT     m_iAssistKeyMove;


	public:
		void   SetKeyMove( INT iKeyFront, INT iKeyBack, INT iKeyLeft, INT iKeyRight );
		void   SetMouseControlMove( bool bControl )   { m_bMouseControlMove  = bControl; }
		void   SetMouseControlAngle( bool bControl )  { m_bMouseZControlAngle = bControl; }
	public:
		INT    GetKeyMove( INT iKeyIndex )  const;
		bool   IsMouseControlMove()         const  { return m_bMouseControlMove; }
		bool   IsMouseControlAngle()        const  { return m_bMouseZControlAngle; }


	public:
		void   SetSpeedMove( float fSpeed )                    { m_fSpeedMove           = fSpeed; }
		void   SetSensitivityMove( float fSensitivity )        { m_fSensitivityMove     = fSensitivity; }
		void   SetSensitivityRotation( float fSensitivity )    { m_fSensitivityRotation = fSensitivity; }
	public:
		inline float GetSpeedMove()              const  { return m_fSpeedMove; }
		inline float GetSensitivityRotation()    const  { return m_fSensitivityRotation; }


	public:
		void   SetPos( const D3DXVECTOR3& pos );
		void   SetTargetHeight( float fHeight );
		void   SetDistanceFromTarget( float fDistance );
		void   SetDistanceFromTargetParam( float fParam );
		//VOID   SetAngleRotation( FLOAT fAngle )         { m_fAngleRotation      = fAngle;    }
		//VOID   SetAngleHorizontal( FLOAT fAngle )       { m_fAngleHorizontal    = fAngle;    }
		//VOID   SetAngleLateral( FLOAT fAngle )          { m_fAngleLateral       = fAngle;    }
	public:
		inline float GetTargetHeight()     const{ return m_fTargetHeight; }
		inline float GetTargetDistance()   const{ return m_fDistanceFromTarget; }
			//inline FLOAT GetAngleRotation()    CONST  { return m_fAngleRotation;      }
			//inline FLOAT GetAngleHorizontal()  CONST  { return m_fAngleHorizontal;    }
			//inline FLOAT GetAngleLateral()     CONST  { return m_fAngleLateral;       }

		D3DXVECTOR3 getEye();
		D3DXVECTOR3 getLookat();

	public:
		bool  Update( const CDXInput* pBInput, float fSliceTime );


	public:
		CViewerCamera( void );
		~CViewerCamera();
	};

}

#endif