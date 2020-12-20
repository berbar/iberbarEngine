
#include <iberbar/Poster/TextureScale9.h>



iberbar::Poster::CTextureScale9::CTextureScale9( CSurface& surface, const UTextureScale9Paddings& paddings )
	: m_paddings( paddings )
{
	m_matOrigin = surface.GetMat();
	cv::Size matOriginSize = m_matOrigin.size();
	CRect2i rects9[9];
	RectSplitScale9( rects9, CRect2i( 0, 0, matOriginSize.width, matOriginSize.height ), m_paddings );
	for ( int i = 0; i < 9; i++ )
		m_mats[i] = cv::Mat( m_matOrigin, ToCvRect( rects9[i] ) );
}

void iberbar::Poster::RectSplitScale9( CRect2i pOutRects9[9], const CRect2i& rect, const UTextureScale9Paddings& paddings )
{
	int x[4] = { rect.l, rect.l + paddings.left, rect.r - paddings.right,  rect.r };
	int y[4] = { rect.t, rect.t + paddings.top,  rect.b - paddings.bottom, rect.b };

	pOutRects9[0] = CRect2i( x[0], y[0], x[1], y[1] );
	pOutRects9[1] = CRect2i( x[1], y[0], x[2], y[1] );
	pOutRects9[2] = CRect2i( x[2], y[0], x[3], y[1] );

	pOutRects9[3] = CRect2i( x[0], y[1], x[1], y[2] );
	pOutRects9[4] = CRect2i( x[1], y[1], x[2], y[2] );
	pOutRects9[5] = CRect2i( x[2], y[1], x[3], y[2] );

	pOutRects9[6] = CRect2i( x[0], y[2], x[1], y[3] );
	pOutRects9[7] = CRect2i( x[1], y[2], x[2], y[3] );
	pOutRects9[8] = CRect2i( x[2], y[2], x[3], y[3] );
}



//
//void RectSplitScale9Cv( cv::Rect pOutRects9[9], const CRect2i& rect, const UTextureScale9Paddings& paddings )
//{
//	int x[4] = { rect.l, paddings.left, rect.r - paddings.right,  rect.r };
//	int y[4] = { rect.t, paddings.top,  rect.b - paddings.bottom, rect.b };
//
//	pOutRects9[0] = CRect2i( x[0], y[0], x[1], y[1] );
//	pOutRects9[1] = CRect2i( x[1], y[0], x[2], y[1] );
//	pOutRects9[2] = CRect2i( x[2], y[0], x[3], y[1] );
//
//	pOutRects9[0] = CRect2i( x[0], y[1], x[1], y[2] );
//	pOutRects9[1] = CRect2i( x[1], y[1], x[2], y[2] );
//	pOutRects9[2] = CRect2i( x[2], y[1], x[3], y[2] );
//
//	pOutRects9[0] = CRect2i( x[0], y[2], x[1], y[3] );
//	pOutRects9[1] = CRect2i( x[1], y[2], x[2], y[3] );
//	pOutRects9[2] = CRect2i( x[2], y[2], x[3], y[3] );
//}