#pragma once

//#include <iberbar/Poster/Point.h>
//#include <iberbar/Poster/Size.h>
//#include <iberbar/Poster/Rect.h>
//#include <iberbar/Poster/Circle.h>
//#include <iberbar/Poster/Color.h>
#include <iberbar/Utility/Point.h>
#include <iberbar/Utility/Size.h>
#include <iberbar/Utility/Rect.h>
#include <iberbar/Utility/Circle.h>
#include <iberbar/Utility/Color.h>
#include <opencv2/core/types.hpp>

namespace iberbar
{
	namespace Poster
	{
		inline cv::Rect2i ToCvRect( const CRect2i& rect )
		{
			return cv::Rect2i( rect.l, rect.t, rect.Width(), rect.Height() );
		}
		inline cv::Size2i ToCvSize( const CSize2i& size )
		{
			return cv::Size( size.w, size.h );
		}
		inline cv::Point2i ToCvPoint( const CPoint2i& pt )
		{
			return cv::Point2i( pt.x, pt.y );
		}
		inline cv::Scalar ToCvScalar( const CColor4B& color )
		{
			return cv::Scalar( color.b, color.g, color.r, color.a );
		}
		inline cv::Scalar_<byte> ToCvScalar4B( const CColor4B& color )
		{
			return cv::Scalar_<byte>( color.b, color.g, color.r, color.a );
		}

		//inline cv::Rect2i ToCvRect( const URect& rect )
		//{
		//	return cv::Rect2i( rect.l, rect.t, rect.Width(), rect.Height() );
		//}
		//inline cv::Size2i ToCvSize( const USize& size )
		//{
		//	return cv::Size( size.w, size.h );
		//}
		//inline cv::Point2i ToCvPoint( const UPoint& pt )
		//{
		//	return cv::Point2i( pt.x, pt.y );
		//}
		//inline cv::Scalar ToCvScalar( const UColor& color )
		//{
		//	return cv::Scalar( color.b, color.g, color.r, color.a );
		//}
	}
}
