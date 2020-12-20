

#ifndef __PICK3D_H__
#define __PICK3D_H__

#include <d3dx9math.h>
#include <iberbar\Base\Rect.h>


namespace iberbar
{
	void PickRayFromClient(
		D3DXVECTOR3* pOutRayPosition, D3DXVECTOR3* pOutRayDirection,
		const D3DXMATRIX* pMatProj,
		const CSize* pViewport,
		const CPoint* pClientPoint );

	void TransformRay(
		D3DXVECTOR3* pOutRayPosition, D3DXVECTOR3* pOutRayDirection,
		const D3DXMATRIX* pMatView );

	void Coordinate3dToClient(
		CPoint* pOutClientPoint,
		const iberbar::CSize* pViewport,
		const D3DXVECTOR3* pEye,
		const D3DXVECTOR3* pCoord,
		const D3DXMATRIX* pMatView,
		const D3DXMATRIX* pMatProj );
	
}

#endif