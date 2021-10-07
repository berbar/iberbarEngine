

#pragma once

#include <iberbar/Font/Types.h>


namespace iberbar
{


	struct UFontCharBitmapDesc
	{
		int   nCharWidth; // 字符逻辑宽度，用于排版,小于nBmpWidth，斜体需要渲染的宽度大于逻辑宽度
		int   nCharHeight;
		int nDeltaX;
		int nDeltaY;
		int   nBmpWidth;  // 纹理宽度
		int   nBmpHeight; // 纹理高度
		void* pBitsFill;      // ARGB_8888 32bit
		int   bOutline;

		UFontCharBitmapDesc()
			: nCharWidth( 0 )
			, nCharHeight( 0 )
			, nDeltaX( 0 )
			, nDeltaY( 0 )
			, nBmpWidth( 0 )
			, nBmpHeight( 0 )
			, pBitsFill( nullptr )
			, bOutline( 0 )
		{
		}
	};

	struct UFontConstLabelBitmapDesc
	{
		int   nBitsWidth;
		int   nBitsHeight;
		void* pBitsFill;         // new 的数据，需要手动 delete
		void* pBitsOutline;  // new 的数据，需要手动 delete

		int nOutlineSize;
	};
}