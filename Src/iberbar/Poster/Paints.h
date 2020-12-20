#pragma once

#include <opencv2/core/mat.hpp>
#include <iberbar/Poster/MathTypes.h>

namespace iberbar
{
	namespace Poster
	{
		// 安全绘制Mat，检查边界，不抛出异常
		void SafeDrawMat( cv::Mat matSrc, cv::Mat matDest, const CRect2i& rcDest, const CColor4B* color = nullptr );

		// 安全绘制Mat，检查边界，不抛出异常
		void SafeDrawMat( cv::Mat matSrc, const CRect2i& rcSrc, cv::Mat matDest, const CRect2i& rcDest, const CColor4B* color = nullptr );

		cv::Mat BuildMaskMat_Circle( int radius );

		cv::Mat BuildMaskMat_RectangleWithCorners( const CSize2i& size, int cornerRadius );
	}
}


