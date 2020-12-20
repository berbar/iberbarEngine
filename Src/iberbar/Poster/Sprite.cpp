
#include "Sprite.h"
#include "Paints.h"
#include "Surface.h"
#include "TextureScale9.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>

using namespace iberbar::Poster;



void iberbar::Poster::DrawTextureScale9( CSurface* surface, const CRect2i& dest, CTextureScale9* texture )
{
	assert( texture );
	assert( surface );

	cv::Mat matOrigin = texture->MatOrigin();
	cv::Size matOriginSize = matOrigin.size();
	cv::Mat* mats9 = texture->Mats();
	const UTextureScale9Paddings& paddings = texture->Paddings();
	cv::Mat matTarget = surface->GetMat();

	CSize2i destSize = dest.Size();
	if ( destSize.w < matOriginSize.width )
	{
		assert( "暂不支持尺寸" );
	}
	if ( destSize.h < matOriginSize.height )
	{
		assert( "暂不支持尺寸" );
	}

	// 划分区域
	CRect2i rects9[9];
	RectSplitScale9( rects9, dest, paddings );

	// 绘制无需拉伸的四角
	int index0[4] = { 0, 2, 6, 8 };
	for ( int i = 0; i < 4; i++ )
	{
		int j = index0[i];
		SafeDrawMat( mats9[j], matTarget, rects9[j] );
	}

	// 绘制需要拉伸的区域
	int index1[5] = { 1, 3, 4, 5, 7 };
	cv::Size matSrcSize;
	cv::Size matDestSize;
	cv::Mat matSrcTemp;
	cv::Rect rcDestNew;
	for ( int i = 0; i < 5; i++ )
	{
		int j = index1[i];
		SafeDrawMat( mats9[j], matTarget, rects9[j] );
	}
}


void iberbar::Poster::DrawTexture( CSurface* surface, const CRect2i& dest, CSurface* texture, __maybenull const CRect2i* src )
{
	assert( texture );
	assert( surface );

	if ( src == nullptr )
		SafeDrawMat( texture->GetMat(), surface->GetMat(), dest );
	else
		SafeDrawMat( texture->GetMat(), *src, surface->GetMat(), dest );
}


void iberbar::Poster::DrawTexture( CSurface* surface, const CRect2i& dest, CSurface* texture, const CColor4B& color, __maybenull const CRect2i* src )
{
	assert( texture );
	assert( surface );

	if ( src == nullptr )
		SafeDrawMat( texture->GetMat(), surface->GetMat(), dest, &color );
	else
		SafeDrawMat( texture->GetMat(), *src, surface->GetMat(), dest, &color );
}


void iberbar::Poster::DrawTextureInCircle( CSurface* surface, const CCircle& dest, CSurface* texture, __maybenull const CRect2i* src )
{
	CSize2i texSize = texture->GetSize();
	
	int destDeameter = dest.Diameter();
	cv::Mat matSrc = (src == nullptr) ? texture->GetMat() : cv::Mat( texture->GetMat(), ToCvRect( *src ) );
	cv::Size matDestSize = cv::Size( destDeameter, destDeameter );
	cv::Size matSrcSize = matSrc.size();
	if ( matDestSize != matSrcSize )
	{
		cv::resize( matSrc, matSrc, matDestSize );
	}
	cv::Mat matMask = BuildMaskMat_Circle( dest.r );
	cv::Mat matSrcNew;
	matSrc.copyTo( matSrcNew, matMask );
	SafeDrawMat( matSrcNew, surface->GetMat(), dest.GetBounding() );
}

void iberbar::Poster::DrawTextureInCornersBox( CSurface* surface, const CRect2i& dest, int radius, CSurface* texture, __maybenull const CRect2i* src )
{
	CSize2i texSize = texture->GetSize();

	cv::Mat matSrc = (src == nullptr) ? texture->GetMat() : cv::Mat( texture->GetMat(), ToCvRect( *src ) );
	cv::Size matDestSize( dest.Width(), dest.Height() );
	cv::Size matSrcSize = matSrc.size();
	if ( matDestSize != matSrcSize )
	{
		cv::resize( matSrc, matSrc, matDestSize );
	}
	cv::Mat matMask = BuildMaskMat_RectangleWithCorners( dest.Size(), radius );
	cv::Mat matSrcNew;
	matSrc.copyTo( matSrcNew, matMask );
	SafeDrawMat( matSrcNew, surface->GetMat(), CRect2i( dest ) );
}


void iberbar::Poster::DrawLine( CSurface* surface, const CPoint2i* points, int pointsCount, const CColor4B& color, const ULineRenderOptions* options )
{
	assert( points );
	assert( surface );
	assert( pointsCount > 1 );

	int width = 1;
	int type = cv::LINE_8;
	if ( options != nullptr )
	{
		width = options->width;
	}
	cv::line( surface->GetMat(), ToCvPoint( points[0] ), ToCvPoint( points[1] ), ToCvScalar( color ), width, type );
}

void iberbar::Poster::DrawLine2( CSurface* surface, const CPoint2i& point1, const CPoint2i& point2, const CColor4B& color, const ULineRenderOptions* options )
{
	int width = 1;
	int type = cv::LINE_8;
	if ( options != nullptr )
	{
		width = options->width;
	}
	cv::line( surface->GetMat(), ToCvPoint( point1 ), ToCvPoint( point2 ), ToCvScalar( color ), width, type );
}


void  iberbar::Poster::DrawRectagle( CSurface* surface, const CRect2i& rect, const CColor4B& color, int thickness )
{
	assert( surface );
	cv::rectangle( surface->GetMat(), ToCvRect( rect ), ToCvScalar( color ), thickness );
}


