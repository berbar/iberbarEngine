#pragma once

#include <iberbar/Font/FontBase.h>




namespace iberbar
{
	class CFontDeviceFt;
	class CFontDeviceFtCore;
	class CFontFaceFt;
	class CFontFaceFtCore;
	class CBitsBuffer;

	class __iberbarFontApi__ CFontDeviceFt
	{
	public:
		CFontDeviceFt();
		~CFontDeviceFt();

	public:
		CResult Initialize();
		CResult CreateFace( CFontFaceFt** ppOutFace, const char* strFile );

	private:
		CFontDeviceFtCore* m_pCore;
	};

	class __iberbarFontApi__ CFontFaceFt
		: public CRef
	{
	public:
		CFontFaceFt( CFontFaceFtCore* pFaceCore );
		~CFontFaceFt();

	public:
		CResult SetFontSize( int nSize );
		CResult SetFontWeight( int nWeight );
		CResult CreateCharBitmap( wchar_t nChar, UFontCharBitmapDesc* pDesc, UFontBitsFormat nBitsFormat );

	private:
		CFontFaceFtCore* m_pCore;
		int m_nFontSizeNow;
		int m_nFontWeightNow;
		CBitsBuffer* m_pByteBuffer;
	};
}
