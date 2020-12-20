#pragma once

#include <iberbar/Poster/Surface.h>

namespace iberbar
{
	namespace Poster
	{
		struct __iberbarExports__ UTextureScale9Paddings
		{
		public:
			int left;
			int top;
			int right;
			int bottom;

		public:
			UTextureScale9Paddings()
				: left( 0 )
				, top( 0 )
				, right( 0 )
				, bottom( 0 )
			{
			}

			UTextureScale9Paddings( int l, int t, int r, int b )
				: left( l )
				, top( t )
				, right( r )
				, bottom( b )
			{
			}
		};

		class __iberbarExports__ CTextureScale9
			: public CRef
		{
		private:
			cv::Mat m_matOrigin;
			cv::Mat m_mats[9];
			UTextureScale9Paddings m_paddings;
		public:
			CTextureScale9( CSurface& surface, const UTextureScale9Paddings& paddings );

			inline cv::Mat MatOrigin() { return m_matOrigin; }

			inline cv::Mat* Mats() { return m_mats; }

			inline const UTextureScale9Paddings& Paddings() { return m_paddings; }
		};

		IBERBAR_UNKNOWN_PTR_DECLARE( CTextureScale9 );

		/*
			划分出九宫格区域
		*/
		void __iberbarExports__ RectSplitScale9( CRect2i pOutRects9[9], const CRect2i& rect, const UTextureScale9Paddings& paddings );
	}
}