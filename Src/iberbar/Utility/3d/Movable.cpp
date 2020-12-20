
#include "Movable.h"






//------------------------------------------------------------------------------------------
CMovable3d::CMovable3d(void)
{
}


//------------------------------------------------------------------------------------------
void CMovable3d::SetPos( const D3DXVECTOR3& pos )
{
	m_ptPosition = pos;
}


//------------------------------------------------------------------------------------------
void CMovable3d::SetDirectionBoth( const D3DXVECTOR3& vFace, const D3DXVECTOR3& vUp)
{
	D3DXVECTOR3 lc_vLeft;
	D3DXVec3Cross( &lc_vLeft, &vFace, &vUp );
	D3DXVec3Cross( &m_vecViewUp, &lc_vLeft, &vFace );
	D3DXVec3Normalize( &m_vecViewFace, &vFace );
	D3DXVec3Normalize( &m_vecViewUp, &m_vecViewUp );

	m_vecMoveFace = m_vecViewFace;
	m_vecMoveUp   = m_vecViewUp;
}


//------------------------------------------------------------------------------------------
void CMovable3d::SetDirectionView( const D3DXVECTOR3& vFace, const D3DXVECTOR3& vUp )
{
	D3DXVECTOR3 lc_vLeft;
	D3DXVec3Cross( &lc_vLeft, &vFace, &vUp );
	D3DXVec3Cross( &m_vecViewUp, &lc_vLeft, &vFace );
	D3DXVec3Normalize( &m_vecViewFace, &vFace );
	D3DXVec3Normalize( &m_vecViewUp, &m_vecViewUp );
}


//------------------------------------------------------------------------------------------
void CMovable3d::SetDirectionMove( const D3DXVECTOR3& vFace, const D3DXVECTOR3& vUp )
{
	D3DXVECTOR3 lc_vLeft;
	D3DXVec3Cross( &lc_vLeft, &vFace, &vUp );
	D3DXVec3Cross( &m_vecMoveUp, &lc_vLeft, &vFace );
	D3DXVec3Normalize( &m_vecMoveFace, &vFace );
	D3DXVec3Normalize( &m_vecMoveUp, &m_vecMoveUp );
}


//------------------------------------------------------------------------------------------
void CMovable3d::GetDirectionMove( D3DXVECTOR3* pFace, D3DXVECTOR3* pLeft, D3DXVECTOR3* pUp ) const
{
	if ( pFace )
		*pFace = m_vecMoveFace;
	if ( pLeft )
		D3DXVec3Cross( pLeft, &m_vecMoveFace, &m_vecMoveUp );
	if ( pUp )
		*pUp = m_vecMoveUp;
}


//------------------------------------------------------------------------------------------
void CMovable3d::GetDirectionView( D3DXVECTOR3* pFace, D3DXVECTOR3* pLeft, D3DXVECTOR3* pUp ) const
{
	if ( pFace )
		*pFace = m_vecViewFace;
	if ( pLeft )
		D3DXVec3Cross( pLeft, &m_vecViewFace, &m_vecViewUp );
	if ( pUp )
		*pUp = m_vecViewUp;
}


//------------------------------------------------------------------------------------------
void CMovable3d::TranslateOnXYZ( FLOAT fx, FLOAT fy, FLOAT fz )
{
	m_ptPosition.x += fx;
	m_ptPosition.y += fy;
	m_ptPosition.z += fz;
}


//------------------------------------------------------------------------------------------
void CMovable3d::MoveFront( FLOAT fDistance )
{
	D3DXVec3Normalize( &m_vecMoveFace, &m_vecMoveFace );
	D3DXVECTOR3 lc_vecPos = m_ptPosition + m_vecMoveFace * fDistance;

	m_ptPosition = lc_vecPos;
}


//------------------------------------------------------------------------------------------
void CMovable3d::MoveLeft( FLOAT fDistance )
{
	D3DXVECTOR3 lc_vLeft;
	D3DXVec3Cross( &lc_vLeft, &m_vecMoveFace, &m_vecMoveUp );
	D3DXVec3Normalize( &lc_vLeft, &lc_vLeft );
	D3DXVECTOR3 lc_vecPos = m_ptPosition + lc_vLeft * fDistance;

	m_ptPosition = lc_vecPos;
}


//------------------------------------------------------------------------------------------
void CMovable3d::MoveUp( FLOAT fDistance )
{
	D3DXVec3Normalize( &m_vecMoveUp, &m_vecMoveUp );
	D3DXVECTOR3 lc_vecPos = m_ptPosition + m_vecMoveUp * fDistance;

	m_ptPosition = lc_vecPos;
}


//------------------------------------------------------------------------------------------
void CMovable3d::RollBoth( FLOAT fAngle )
{
	RollView( fAngle );
	RollMove( fAngle );
}


//------------------------------------------------------------------------------------------
void CMovable3d::RollMove( FLOAT fAngle )
{
	D3DXMATRIX lc_matrix;  
	D3DXMatrixRotationAxis( &lc_matrix, &m_vecMoveFace, D3DXToRadian(fAngle) );
	D3DXVec3TransformCoord( &m_vecMoveUp, &m_vecMoveUp, &lc_matrix );
}


//------------------------------------------------------------------------------------------
void CMovable3d::RollView( float fAngle )
{
	D3DXMATRIX lc_matrix;  
	D3DXMatrixRotationAxis( &lc_matrix, &m_vecViewFace, D3DXToRadian(fAngle) );
	D3DXVec3TransformCoord( &m_vecViewUp, &m_vecViewUp, &lc_matrix );
}


//------------------------------------------------------------------------------------------
void CMovable3d::TurnUpBoth( FLOAT fAngle )
{
	TurnUpMove( fAngle );
	TurnUpView( fAngle );
}


//------------------------------------------------------------------------------------------
void CMovable3d::TurnUpMove( FLOAT fAngle )
{
	D3DXVECTOR3 lc_vLeft;
	D3DXVec3Cross( &lc_vLeft, &m_vecMoveFace, &m_vecMoveUp );

	D3DXMATRIX lc_matrix;  
	D3DXMatrixRotationAxis( &lc_matrix, &lc_vLeft, D3DXToRadian(fAngle) );
	D3DXVec3TransformCoord( &m_vecMoveUp, &m_vecMoveUp, &lc_matrix );
	D3DXVec3TransformCoord( &m_vecMoveFace, &m_vecMoveFace, &lc_matrix );
}


//------------------------------------------------------------------------------------------
void CMovable3d::TurnUpView( FLOAT fAngle )
{
	D3DXVECTOR3 lc_vLeft;
	D3DXVec3Cross( &lc_vLeft, &m_vecViewFace, &m_vecViewUp );

	D3DXMATRIX lc_matrix;  
	D3DXMatrixRotationAxis( &lc_matrix, &lc_vLeft, D3DXToRadian(fAngle) );
	D3DXVec3TransformCoord( &m_vecViewUp, &m_vecViewUp, &lc_matrix );
	D3DXVec3TransformCoord( &m_vecViewFace, &m_vecViewFace, &lc_matrix );
}


//------------------------------------------------------------------------------------------
void CMovable3d::TurnRightBoth( FLOAT fAngle )
{
	TurnRightMove( fAngle );
	TurnRightView( fAngle );
}


//------------------------------------------------------------------------------------------
void CMovable3d::TurnRightMove( FLOAT fAngle )
{
	D3DXMATRIX lc_matrix;  
	D3DXMatrixRotationAxis( &lc_matrix, &m_vecMoveUp, D3DXToRadian(fAngle) );
	D3DXVec3TransformCoord( &m_vecMoveFace, &m_vecMoveFace, &lc_matrix );
}


//------------------------------------------------------------------------------------------
void CMovable3d::TurnRightViewKeepElevation( FLOAT fAngle )
{
	TurnRightMove( fAngle );
	D3DXMATRIX lc_matrix;  
	D3DXMatrixRotationAxis( &lc_matrix, &m_vecMoveUp, D3DXToRadian(fAngle) );
	D3DXVec3TransformCoord( &m_vecViewFace, &m_vecViewFace, &lc_matrix );
	D3DXVec3TransformCoord( &m_vecViewUp, &m_vecViewUp, &lc_matrix );
}


//------------------------------------------------------------------------------------------
void CMovable3d::TurnRightView( FLOAT fAngle )
{
	D3DXMATRIX lc_matrix;  
	D3DXMatrixRotationAxis( &lc_matrix, &m_vecViewUp, D3DXToRadian(fAngle) );
	D3DXVec3TransformCoord( &m_vecViewFace, &m_vecViewFace, &lc_matrix );
}



