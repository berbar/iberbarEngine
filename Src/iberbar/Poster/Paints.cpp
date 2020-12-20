
#include <opencv2\imgproc.hpp>
#include "Paints.h"
#include <opencv2\imgcodecs.hpp>


void alphaBlend( cv::Mat& foreground, cv::Mat& background, cv::Mat& outImage )
{
	// 计算像素数量 
	int numberOfPixels = foreground.rows * foreground.cols;

	float sb;
	float db;

	// Get floating point pointers to the data matrices
	byte* fptr = foreground.data;
	byte* bptr = background.data;
	//float* aptr = reinterpret_cast<float*>(alpha.data);
	byte* outImagePtr = outImage.data;

	// 一次性循环所有像素
	for ( int i = 0; i < numberOfPixels; i++ )
	{
		sb = (float)fptr[3] / 255.0f;
		db = 1.0f - sb;
		outImagePtr[0] = ((float)fptr[0] * sb + (float)bptr[0] * db);
		outImagePtr[1] = ((float)fptr[1] * sb + (float)bptr[1] * db);
		outImagePtr[2] = ((float)fptr[2] * sb + (float)bptr[2] * db);
		outImagePtr[3] = ((float)fptr[3] * sb + (float)bptr[3] * db);
		fptr += 4;
		bptr += 4;
		outImagePtr += 4;
		//*outImagePtr = (*fptr) * (*aptr) + (*bptr) * (1 - *aptr);
	}

	cv::imwrite( "./Images/Result.png", outImage );
}


void iberbar::Poster::SafeDrawMat( cv::Mat matSrc, cv::Mat matDest, const CRect2i& rcDest, const CColor4B* color )
{
	cv::Size srcSize = matSrc.size();
	SafeDrawMat( matSrc, CRect2i( 0, 0, srcSize.width, srcSize.height ), matDest, rcDest, color );
}

void iberbar::Poster::SafeDrawMat( cv::Mat matSrc, const CRect2i& srcRect, cv::Mat matDest, const CRect2i& destRect, const CColor4B* color )
{
	if ( color != nullptr && color->a == 0 )
		return;

	cv::Size destSizeMax = matDest.size();
	cv::Size srcSizeMax = matSrc.size();

	if ( destRect.l > destSizeMax.width ||
		destRect.t > destSizeMax.height ||
		destRect.r < 0 ||
		destRect.b < 0 )
		return;

	if ( srcRect.l > srcSizeMax.width ||
		srcRect.t > srcSizeMax.height ||
		srcRect.r < 0 ||
		srcRect.b < 0 )
		return;

	CRect2i destRectNew = destRect;
	CRect2i srcRectNew = srcRect;
	cv::Size srcSize( srcRect.Width(), srcRect.Height() );
	cv::Size destSize( destRect.Width(), destRect.Height() );

	if ( destRect.l < 0 )
	{
		destRectNew.l = 0;
		srcRectNew.l = srcRect.l + (0 - destRect.l) * srcSize.width / destSize.width;
	}

	if ( destRect.t < 0 )
	{
		destRectNew.t = 0;
		srcRectNew.t = srcRect.t + (0 - destRect.t) * srcSize.height / destSize.height;
	}

	if ( destRect.r > destSizeMax.width )
	{
		destRectNew.r = destSizeMax.width;
		srcRectNew.r = srcRect.r - (destRect.r - destSizeMax.width) * srcSize.width / destSize.width;
	}

	if ( destRect.b > destSizeMax.height )
	{
		destRectNew.b = destSizeMax.height;
		srcRectNew.b = srcRect.b - (destRect.t - destSizeMax.height) * srcSize.height / destSize.height;
	}

	cv::Size srcSizeNew( srcRect.Width(), srcRect.Height() );
	cv::Size destSizeNew( destRectNew.Width(), destRectNew.Height() );
	cv::Mat matSrcNew = cv::Mat( matSrc, ToCvRect( srcRectNew ) );

	if ( srcSizeNew.width == destSizeNew.width && srcSizeNew.height == destSizeNew.height )
	{
	}
	else
	{
		cv::resize( matSrcNew, matSrcNew, destSizeNew );
	}

	float sb;
	float db;

	byte* fptr = matSrcNew.data;
	byte* bptr = matDest.data;
	//const byte* optr = (const byte*)color;

	// 一次性循环所有像素
	if ( color == nullptr )
	{
		for ( int i = 0; i < matSrcNew.rows; i++ )
		{
			bptr = matDest.data + (matDest.cols * (i + destRect.t) + destRect.l) * 4;
			for ( int j = 0; j < matSrcNew.cols; j++ )
			{
				sb = (float)fptr[3] / 255.0f;
				db = 1.0f - sb;
				bptr[0] = ((float)fptr[0] * sb + (float)bptr[0] * db);
				bptr[1] = ((float)fptr[1] * sb + (float)bptr[1] * db);
				bptr[2] = ((float)fptr[2] * sb + (float)bptr[2] * db);
				bptr[3] = ((float)fptr[3] * sb + (float)bptr[3] * db);
				fptr += 4;
				bptr += 4;
			}

		}
	}
	else
	{
		float pixel[4];
		CColor4F color4f( *color );
		for ( int i = 0; i < matSrcNew.rows; i++ )
		{
			bptr = matDest.data + (matDest.cols * (i + destRect.t) + destRect.l) * 4;
			for ( int j = 0; j < matSrcNew.cols; j++ )
			{
				sb = (float)fptr[3] / 255.0f;
				db = 1.0f - sb;

				pixel[0] = (float)fptr[0] * color4f.b;
				pixel[1] = (float)fptr[1] * color4f.g;
				pixel[2] = (float)fptr[2] * color4f.r;
				pixel[3] = (float)fptr[3] * color4f.a;
				bptr[0] = ((float)pixel[0] * sb + (float)bptr[0] * db);
				bptr[1] = ((float)pixel[1] * sb + (float)bptr[1] * db);
				bptr[2] = ((float)pixel[2] * sb + (float)bptr[2] * db);
				bptr[3] = ((float)pixel[3] * sb + (float)bptr[3] * db);
				fptr += 4;
				bptr += 4;
			}

		}
	}


	//alphaBlend( matSrcNew, matDestNew, matDestNew );
	//cv::add( matSrcNew, matDestNew, matDestNew );
	//cv::addWeighted( matDestNew, 1, matSrcNew, 1, 0, matDestNew );
	//matDestNew = matSrcNew + matDestNew;
}


//void iberbar::Poster::SafeDrawMatInCircle( cv::Mat matSrc, cv::Mat matDest, const UPoint& pt, int radius )
//{
//	cv::Mat matCircle = cv::Mat( cv::Size( radius, radius ), CV_8UC3 );
//	cv::circle( matCircle, pt, radius, cv::Scalar( 0xff, 0xff, 0xff, 0xff ), -1 );
//	matSrc.copyTo( , matCircle );
//	
//	cv::addWeighted( matCircle, 1, matSrc, 1, 0, matDest );
//}

cv::Mat iberbar::Poster::BuildMaskMat_Circle( int radius )
{
	int w = radius << 1;
	cv::Mat matCircle = cv::Mat::zeros( cv::Size( w, w ), CV_8U );
	cv::circle( matCircle, cv::Point( radius, radius ), radius, cv::Scalar( 0xff, 0xff, 0xff, 0xff ), -1 );
	return matCircle;
}

cv::Mat iberbar::Poster::BuildMaskMat_RectangleWithCorners( const CSize2i& size, int cornerRadius )
{
	cv::Scalar color( 0xff, 0xff, 0xff, 0xff );
	int deameter = cornerRadius << 1;
	cv::Mat matMask = cv::Mat::zeros( ToCvSize( size ), CV_8U );
	//cv::circle( matMask, cv::Point( cornerRadius, cornerRadius ), cornerRadius, cv::Scalar( 0xff, 0xff, 0xff, 0xff ), -1 );

	cv::Rect rc2[2] = {
		cv::Rect( cv::Point( cornerRadius, 0 ), cv::Size( size.w - deameter, size.h ) ),
		cv::Rect( cv::Point( 0, cornerRadius ), cv::Size( size.w, size.h - deameter ) )
	};
	cv::rectangle( matMask, rc2[0], color, -1 );
	cv::rectangle( matMask, rc2[1], color, -1 );

	cv::Mat matCircle = cv::Mat::zeros( cv::Size( deameter, deameter ), CV_8U );
	cv::circle( matCircle, cv::Point( cornerRadius, cornerRadius ), cornerRadius, cv::Scalar( 0xff, 0xff, 0xff, 0xff ), -1 );

	cv::Mat matCornerSrc;
	cv::Mat matCornerDest;
	cv::Size cornerSize = cv::Size( cornerRadius, cornerRadius );

	matCornerSrc = cv::Mat( matCircle, cv::Rect( cv::Point( 0, 0 ), cornerSize ) );
	matCornerDest = cv::Mat( matMask, cv::Rect( cv::Point( 0, 0 ), cornerSize ) );
	matCornerSrc.copyTo( matCornerDest );

	matCornerSrc = cv::Mat( matCircle, cv::Rect( cv::Point( cornerRadius, 0 ), cornerSize ) );
	matCornerDest = cv::Mat( matMask, cv::Rect( cv::Point( size.w - cornerRadius, 0 ), cornerSize ) );
	matCornerSrc.copyTo( matCornerDest );

	matCornerSrc = cv::Mat( matCircle, cv::Rect( cv::Point( cornerRadius, cornerRadius ), cornerSize ) );
	matCornerDest = cv::Mat( matMask, cv::Rect( cv::Point( size.w - cornerRadius, size.h - cornerRadius ), cornerSize ) );
	matCornerSrc.copyTo( matCornerDest );

	matCornerSrc = cv::Mat( matCircle, cv::Rect( cv::Point( 0, cornerRadius ), cornerSize ) );
	matCornerDest = cv::Mat( matMask, cv::Rect( cv::Point( 0, size.h - cornerRadius ), cornerSize ) );
	matCornerSrc.copyTo( matCornerDest );

	return matMask;
}
