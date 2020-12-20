

#include "Pick.h"




void iberbar::PickRayFromClient(
	D3DXVECTOR3* pOutRayPosition, D3DXVECTOR3* pOutRayDirection,
	const D3DXMATRIX* pMatProj,
	const CSize* pViewport,
	const CPoint* pClientPoint )
{
	assert( pOutRayPosition );
	assert( pOutRayDirection );
	assert( pMatProj );
	assert( pViewport );
	assert( pClientPoint );

	// 暂时只考虑viewport的x和y都等于0的情况
	*pOutRayPosition = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	*pOutRayDirection = D3DXVECTOR3(
		(((2.0f*pClientPoint->x/pViewport->cx)-1.0f)/(*pMatProj)(0,0)),
		(((-2.0f*pClientPoint->y/pViewport->cy)+1.0f)/(*pMatProj)(1,1)),
		1.0f );
}

void iberbar::TransformRay(
	D3DXVECTOR3* pOutRayPosition, D3DXVECTOR3* pOutRayDirection,
	const D3DXMATRIX* pMat )
{
	assert( pMat );
	assert( pOutRayPosition );
	assert( pOutRayDirection );

	D3DXMATRIX lc_matViewInverse;
	D3DXMatrixInverse( &lc_matViewInverse, NULL, pMat );
	D3DXVec3TransformCoord( pOutRayPosition, pOutRayPosition, &lc_matViewInverse );
	D3DXVec3TransformNormal( pOutRayDirection, pOutRayDirection, &lc_matViewInverse );
	D3DXVec3Normalize( pOutRayDirection, pOutRayDirection );
}


void iberbar::Coordinate3dToClient(
	CPoint* pOutClientPoint,
	const iberbar::CSize* pViewport,
	const D3DXVECTOR3* pEye,
	const D3DXVECTOR3* pCoord,
	const D3DXMATRIX* pMatView,
	const D3DXMATRIX* pMatProj )
{
	D3DXVECTOR3 lc_point;
	D3DXVec3TransformCoord( &lc_point, pCoord, pMatView );
	D3DXVec3TransformCoord( &lc_point, &lc_point, pMatProj );
	pOutClientPoint->x = (lc_point.x+1)/2 * (float)pViewport->cx;
	pOutClientPoint->y = (1-lc_point.y)/2 * (float)pViewport->cy;
}

