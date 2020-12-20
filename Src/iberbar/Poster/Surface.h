#pragma once

#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <iberbar/Poster/MathTypes.h>
#include <iberbar/Utility/Unknown.h>

namespace iberbar
{
	namespace Poster
	{
		/*
			32Œª£¨BGRA∏Ò Ω
		*/
		class __iberbarExports__ CSurface
			: public CRef
		{
		public:
			CSurface();
			CSurface( int w, int h );
			CSurface( int w, int h, const CColor4B& color );

		public:
			cv::Mat GetMat() { return m_mat; }

			CSize2i GetSize() const;

			CRect2i GetRect() const;

			void Load( const char* filepath );

			void Load( const char* filepath, int type );

			void LoadFromFileMemory( const byte* bytes, int bytesCount );

			void LoadFromPixels( const byte* bits, int w, int h );

			void Write( const char* filepath ) const;

		private:
			cv::Mat m_mat;
		};

		IBERBAR_UNKNOWN_PTR_DECLARE( CSurface );

		inline CSurface::CSurface()
		{
		}

		inline CSurface::CSurface( int w, int h )
			: m_mat( cv::Size( w, h ), CV_8UC4 )
		{
		}

		inline CSurface::CSurface( int w, int h, const CColor4B& color )
			: m_mat( cv::Size( w, h ), CV_8UC4, ToCvScalar( color ) )
		{
		}

		inline CSize2i CSurface::GetSize() const
		{
			cv::Size size = m_mat.size();
			return CSize2i( size.width, size.height );
		}

		inline CRect2i CSurface::GetRect() const
		{
			return CRect2i( CPoint2i( 0, 0 ), GetSize() );
		}

		inline void CSurface::Load( const char* filepath )
		{
			Load( filepath, CV_8UC4 );
		}

		inline void CSurface::Load( const char* filepath, int type )
		{
			m_mat = cv::imread( filepath, cv::IMREAD_COLOR );
			int matType = m_mat.type();
			if ( matType == type )
				return;
			if ( type == CV_8UC4 )
			{
				cv::cvtColor( m_mat, m_mat, cv::COLOR_BGR2BGRA );
				matType = m_mat.type();
			}
		}

		inline void CSurface::LoadFromFileMemory( const byte* bytes, int bytesCount )
		{
			std::vector<byte> bytesTemp( bytesCount );
			memcpy_s( &bytesTemp.front(), bytesCount, bytes, bytesCount );
			m_mat = cv::imdecode( bytesTemp, cv::IMREAD_COLOR );
			int matType = m_mat.type();
			if ( matType == CV_8UC4 )
				return;
			if ( CV_8UC4 == CV_8UC4 )
			{
				cv::cvtColor( m_mat, m_mat, cv::COLOR_BGR2BGRA );
				matType = m_mat.type();
			}
		}

		inline void CSurface::LoadFromPixels( const byte* bits, int w, int h )
		{
			m_mat = cv::Mat( cv::Size( w, h ), CV_8UC4 );
			int datasize = w * h * 4;
			memcpy_s( m_mat.data, datasize, bits, datasize );
		}

		inline void CSurface::Write( const char* filepath ) const
		{
			cv::imwrite( filepath, m_mat );
		}
	}
}