

#include "iberbar\Base\3d\Camera\Camera.h"
#include "iberbar\Base\Input\DXInput.h"


iberbar::CBaseCamera3d::CBaseCamera3d( void )
	: m_eCmrType( BCMR_TYPE_COMMON )
{
	m_bWorking = FALSE;
	m_fovy = D3DX_PI / 4;
	m_fAspect = 1.0f;
	m_fFocusDistance = 0.0f;
	m_fZnear = 1.0f;
	m_fZfar = 10000.0f;
}

void iberbar::CBaseCamera3d::SetPerspectiveMatrix( float fovy, float fAspect, float zn, float zf )
{
	m_fovy = fovy;
	m_fAspect = fAspect;
	m_fZnear = zn;
	m_fZfar = zf;
}

void iberbar::CBaseCamera3d::GetViewMatrix( D3DXMATRIX* pOutMatrix )
{
	D3DXVECTOR3 lc_vecLeft;
	D3DXVec3Cross( &lc_vecLeft, &m_vecViewFace, &m_vecViewUp );
	D3DXVec3Cross( &m_vecViewUp, &lc_vecLeft, &m_vecViewFace );
	D3DXVec3Normalize( &m_vecViewFace, &m_vecViewFace );
	D3DXVec3Normalize( &lc_vecLeft, &lc_vecLeft );
	D3DXVec3Normalize( &m_vecViewUp, &m_vecViewUp );

	D3DXVECTOR3 lc_pos, lc_lookat;
	lc_pos = m_ptPosition + m_vecViewFace * m_fFocusDistance;
	lc_lookat = lc_pos + m_vecViewFace;

	D3DXMATRIX lc_matView;
	D3DXMatrixLookAtLH( pOutMatrix, &lc_pos, &lc_lookat, &m_vecViewUp );
}

void iberbar::CBaseCamera3d::GetPerspectiveMatrix( D3DXMATRIXA16* pOutProj ) const
{
	D3DXMatrixPerspectiveFovLH( pOutProj, m_fovy, m_fAspect, m_fZnear, m_fZfar );
}














// CB3DCameraFPSW::CB3DCameraFPSW(VOID)
// : m_fSpeedMove( 0.0f )
// , m_fSensitivity( 0.0f )
// {
// 	m_eCmrType = BCMR_TYPE_FPSW;
// 	m_bKeyPressed[0] = false;
// 	m_bKeyPressed[1] = false;
// 	m_bKeyPressed[2] = false;
// 	m_bKeyPressed[3] = false;
// 	ZeroMemory( m_iKeyMove, 16 );
// 	ZeroMemory( m_iKeyTurn, 8 );
// }
// 
// CB3DCameraFPSW::~CB3DCameraFPSW()
// {
// }
// 
// VOID CB3DCameraFPSW::SetKeyMove( INT iKeyFront, INT iKeyBack, INT iKeyLeft, INT iKeyRight )
// {
// 	m_iKeyMove[0] = iKeyFront;
// 	m_iKeyMove[1] = iKeyBack;
// 	m_iKeyMove[2] = iKeyLeft;
// 	m_iKeyMove[3] = iKeyRight;
// 	m_bKeyPressed[0] = false;
// 	m_bKeyPressed[1] = false;
// 	m_bKeyPressed[2] = false;
// 	m_bKeyPressed[3] = false;
// }
// 
// VOID CB3DCameraFPSW::SetKeyTurn( INT iKeyLeft, INT iKeyRight )
// {
// 	m_iKeyTurn[0] = iKeyLeft;
// 	m_iKeyTurn[1] = iKeyRight;
// }
// 
// INT CB3DCameraFPSW::GetKey( INT iKeyIndex ) const
// {
// 	switch( iKeyIndex )
// 	{
// 	case B3D_CAMERA_FPS_KEY_MOVEFRONT:
// 	case B3D_CAMERA_FPS_KEY_MOVEBACK:
// 	case B3D_CAMERA_FPS_KEY_MOVELEFT:
// 	case B3D_CAMERA_FPS_KEY_MOVERIGHT:
// 		return m_iKeyMove[iKeyIndex];
// 
// 	case B3D_CAMERA_FPS_KEY_TURNLEFT:
// 	case B3D_CAMERA_FPS_KEY_TURNRIGHT:
// 		return m_iKeyTurn[iKeyIndex-6];
// 	}
// 
// 	return -1;
// }
// 
// BOOL CB3DCameraFPSW::Update( const CBDirectInput* pBInput, float fElaspeTime )
// {
// 	if ( !pBInput )
// 		return FALSE;
// 
// 	if ( pBInput->IsKeyPressed( m_iKeyMove[B3D_CAMERA_FPS_KEY_MOVEFRONT] ) )
// 	{
// 		MoveFront( m_fSpeedMove*fElaspeTime );
// 	}
// 
// 	if ( pBInput->IsKeyPressed( m_iKeyMove[B3D_CAMERA_FPS_KEY_MOVEBACK] ) )
// 	{
// 		MoveFront( -m_fSpeedMove*fElaspeTime );
// 	}
// 
// 	if ( pBInput->IsKeyPressed( m_iKeyMove[B3D_CAMERA_FPS_KEY_MOVELEFT] ) )
// 	{
// 		MoveLeft( m_fSpeedMove*fElaspeTime );
// 	}
// 
// 	if ( pBInput->IsKeyPressed( m_iKeyMove[B3D_CAMERA_FPS_KEY_MOVERIGHT] ) )
// 	{
// 		MoveLeft( -m_fSpeedMove*fElaspeTime );
// 	}
// 
// 	TurnRightViewKeepElevation( (float)(pBInput->GetMouseMoveX())*m_fSensitivity*fElaspeTime );
// 	//TurnUpView( -(FLOAT)(pBInput->GetMouseMoveY())*m_fSensitivity*fElaspeTime );
// 
// 	float lc_fAngleZ = -(float)(pBInput->GetMouseMoveY())*m_fSensitivity*fElaspeTime;
// 
// 	D3DXVECTOR3 lc_vLeft;
// 	D3DXVec3Cross( &lc_vLeft, &m_vecViewFace, &m_vecViewUp );
// 
// 	D3DXVECTOR3 lc_vecTemp;
// 	D3DXMATRIX  lc_matrix;
// 	D3DXMatrixRotationAxis( &lc_matrix, &lc_vLeft, D3DXToRadian(lc_fAngleZ) );
// 	D3DXVec3TransformCoord( &lc_vecTemp, &m_vecViewUp, &lc_matrix );
// 
// 	if ( lc_vecTemp.y >= 0.0f )
// 	{
// 		m_vecViewUp = lc_vecTemp;
// 		D3DXVec3TransformCoord( &m_vecViewFace, &m_vecViewFace, &lc_matrix );
// 		AfterTransformation( B3D_MBOBJ_TRANS_TUV );
// 	}
// 
// 	return TRUE;
// }
















iberbar::CViewerCamera::CViewerCamera( void )
: m_bMouseZControlAngle( false )
, m_bMouseControlMove( false )
//, m_fDistanceFromTarget( 100.0f )
, m_fDistanceFromTarget( 0.0f )
, m_fDistanceFromTargetParam( 1.0f )
, m_fTargetHeight( 0.0f )
, m_fSpeedMove( 0.0f )
, m_fSensitivityMove( 0.0f )
, m_fSensitivityRotation( 0.0f )
, m_btMainKeyMove( 1 )
, m_btMainKeyChangeAngleZ( 1 )
, m_btMainKeyChangeAngleAll( 1 )
, m_iAssistKeyMove( -1 )
, m_iAssistKeyChangeAngleAll( DIK_LCONTROL )
, m_iAssistKeyChangeAngleZ( 0 )
{
	m_eCmrType = BCMR_TYPE_OVERLOOK;
	m_iKeyMove[0] = 0;
	m_iKeyMove[1] = 0;
	m_iKeyMove[2] = 0;
	m_iKeyMove[3] = 0;
}

iberbar::CViewerCamera::~CViewerCamera()
{
}

void iberbar::CViewerCamera::SetPos( const D3DXVECTOR3& pos )
{
	D3DXVECTOR3 lc_ptOffset = pos - m_ptPosition;

	m_ptPosition = pos;
	m_fTargetHeight = m_fTargetHeight - lc_ptOffset.y;
}

void iberbar::CViewerCamera::SetTargetHeight( float fHeight )
{
	m_ptPosition.y  = m_fTargetHeight - fHeight;
	m_fTargetHeight = fHeight;
}

void iberbar::CViewerCamera::SetDistanceFromTarget( float fDistance )
{
	if ( fDistance < 0.0001f )
		fDistance = 0.0001f;

	float lc_fTemp = m_fDistanceFromTarget;
	m_fDistanceFromTarget = fDistance;

	D3DXVECTOR3 lc_faceOffset;
	D3DXVec3Normalize( &lc_faceOffset, &m_vecViewFace );
	lc_faceOffset = (fDistance - lc_fTemp) * lc_faceOffset;

	m_ptPosition = m_ptPosition - lc_faceOffset;
}

void iberbar::CViewerCamera::SetDistanceFromTargetParam( float fParam )
{
	m_fDistanceFromTargetParam = fParam;
}


D3DXVECTOR3 iberbar::CViewerCamera::getEye()
{
	D3DXVECTOR3 lc_vViewFace;
	D3DXVECTOR3 lc_vecLeft;
	D3DXVec3Cross( &lc_vecLeft, &m_vecViewFace, &m_vecViewUp );
	D3DXVec3Normalize( &lc_vViewFace, &m_vecViewFace );

	D3DXVECTOR3 lc_pos;
	lc_pos = m_ptPosition + lc_vViewFace * m_fFocusDistance;
	return lc_pos;
}


D3DXVECTOR3 iberbar::CViewerCamera::getLookat()
{
	D3DXVECTOR3 lc_vecLeft;
	D3DXVec3Cross( &lc_vecLeft, &m_vecViewFace, &m_vecViewUp );
	D3DXVec3Cross( &m_vecViewUp, &lc_vecLeft, &m_vecViewFace );
	D3DXVec3Normalize( &m_vecViewFace, &m_vecViewFace );
	D3DXVec3Normalize( &lc_vecLeft, &lc_vecLeft );
	D3DXVec3Normalize( &m_vecViewUp, &m_vecViewUp );

	D3DXVECTOR3 lc_pos, lc_lookat;
	lc_pos = m_ptPosition + m_vecViewFace * m_fFocusDistance;
	lc_lookat = lc_pos + m_vecViewFace;

	return lc_lookat;
}


bool iberbar::CViewerCamera::Update( const CDXInput* pBInput, float fSliceTime )
{
	if ( !pBInput || fSliceTime <= 0 )
		return false;

	// 因为更新的快，所以先记录数值
	LONG lc_lZ = pBInput->GetMouseMoveZ();
	SetDistanceFromTarget( m_fDistanceFromTarget + (float)lc_lZ * m_fDistanceFromTargetParam );

	if ( m_bMouseControlMove )
	{
		if ( pBInput->IsButtonPressed(m_btMainKeyMove) &&
			( m_iAssistKeyMove<0 || pBInput->IsKeyPressed(m_iAssistKeyMove)) &&
			!pBInput->IsKeyPressed(m_iAssistKeyChangeAngleAll) )
		{
			LONG lc_fMouseMoveX = pBInput->GetMouseMoveX();
			LONG lc_lMouseMoveY = pBInput->GetMouseMoveY();
			LONG lc_fMouseMoveZ = pBInput->GetMouseMoveZ();

			float lc_fMove = fSliceTime * m_fSpeedMove * m_fSensitivityMove * (m_fDistanceFromTarget/100.0f);

			MoveFront( lc_fMove * (float)lc_lMouseMoveY );
			MoveLeft( lc_fMove * (float)lc_fMouseMoveX );

			return true;
		}
	}
	else
	{
		if ( pBInput->IsKeyPressed( m_iKeyMove[B3D_CAMERA_FPS_KEY_MOVEFRONT] ) )
			MoveFront( m_fSpeedMove*fSliceTime );

		if ( pBInput->IsKeyPressed( m_iKeyMove[B3D_CAMERA_FPS_KEY_MOVEBACK] ) )
			MoveFront( -m_fSpeedMove*fSliceTime );

		if ( pBInput->IsKeyPressed( m_iKeyMove[B3D_CAMERA_FPS_KEY_MOVELEFT] ) )
			MoveLeft( m_fSpeedMove*fSliceTime );

		if ( pBInput->IsKeyPressed( m_iKeyMove[B3D_CAMERA_FPS_KEY_MOVERIGHT] ) )
			MoveLeft( -m_fSpeedMove*fSliceTime );
	}

	const D3DXVECTOR3 lc_faceViewPre = m_vecViewFace;
	BOOL lc_bChanged = FALSE;
	LONG lc_lMouseMoveZ = pBInput->GetMouseMoveZ();

	if ( lc_lMouseMoveZ != 0 &&
		m_bMouseZControlAngle &&
		( m_iAssistKeyChangeAngleZ<0 || pBInput->IsKeyPressed(m_iAssistKeyChangeAngleZ)) )
	{
		TurnUpView( - (float)(lc_lMouseMoveZ / 120) * m_fSensitivityRotation );
		lc_bChanged = TRUE;
	}
	else if ( pBInput->IsButtonPressed(m_btMainKeyChangeAngleAll) &&
		( m_iAssistKeyChangeAngleAll<0 || pBInput->IsKeyPressed(m_iAssistKeyChangeAngleAll)) )
	{
		LONG lc_lMouseMoveX = pBInput->GetMouseMoveX();
		LONG lc_lMouseMoveY = pBInput->GetMouseMoveY();
		TurnRightViewKeepElevation( (float)lc_lMouseMoveX * m_fSensitivityRotation * m_fSensitivityMove );
		TurnUpView( - (float)lc_lMouseMoveY * m_fSensitivityRotation * m_fSensitivityMove );
		lc_bChanged = TRUE;
	}

	if ( lc_bChanged == TRUE )
	{
		D3DXVECTOR3 lc_faceTemp, lc_ptTarget;
		D3DXVec3Normalize( &lc_faceTemp, &lc_faceViewPre );
		lc_faceTemp = lc_faceTemp * m_fDistanceFromTarget;
		lc_ptTarget = m_ptPosition + lc_faceTemp;

		D3DXVec3Normalize( &lc_faceTemp, &m_vecViewFace );
		m_ptPosition = lc_ptTarget - lc_faceTemp * m_fDistanceFromTarget;
	}

	return true;
}