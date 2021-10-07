

#pragma once

#include <iberbar/Font/Types.h>


namespace iberbar
{


	struct UFontCharBitmapDesc
	{
		int   nCharWidth; // �ַ��߼���ȣ������Ű�,С��nBmpWidth��б����Ҫ��Ⱦ�Ŀ�ȴ����߼����
		int   nCharHeight;
		int nDeltaX;
		int nDeltaY;
		int   nBmpWidth;  // ������
		int   nBmpHeight; // ����߶�
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
		void* pBitsFill;         // new �����ݣ���Ҫ�ֶ� delete
		void* pBitsOutline;  // new �����ݣ���Ҫ�ֶ� delete

		int nOutlineSize;
	};
}