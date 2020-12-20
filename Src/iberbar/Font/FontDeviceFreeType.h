#pragma once

#include <iberbar/Font/Headers.h>
#include <iberbar/Font/FontBase.h>


namespace iberbar
{
	class CFontDeviceFreeType;
	class CFontFaceFreeType;
	class CFontBitsBufferFreeType;
	class CFreeTypeCore;
	class CFreeTypeFace;

	IBERBAR_UNKNOWN_PTR_DECLARE( CFontDeviceFreeType );
	IBERBAR_UNKNOWN_PTR_DECLARE( CFontFaceFreeType );

	class __iberbarFontApi__ CFontDeviceFreeType
		: public CRef
	{
	public:
		CFontDeviceFreeType();
		~CFontDeviceFreeType();

	public:
		CResult Initialize();
		CResult CreateFontFace( const UFontDesc& fontDesc, CFontFaceFreeType** ppFontFace );

	protected:
		CFreeTypeCore* m_pFreeTypeCore;
	};

	class __iberbarFontApi__ CFontFaceFreeType
		: public CRef
	{
		friend CFontDeviceFreeType;

	public:
		CFontFaceFreeType();
		~CFontFaceFreeType();

	private:
		CResult Create( CFreeTypeCore* pFreeTypeCore, const UFontDesc& fontDesc );

	public:
		CResult CreateCharBitmap( wchar_t ch, UFontCharBitmapDesc* pDesc, UFontBitsFormat bitsFormat );

		bool CreateConstLabelBitmap( const wchar_t* str, UFontCharBitmapDesc* pDesc );

		int MeasureCharWidth( wchar_t ch );

		const UFontDesc& GetFontDesc() const { return m_fontDesc; }

	private:
		CFreeTypeFace* m_face;
		CFontBitsBufferFreeType* m_buffer;
		UFontDesc m_fontDesc;
	};
}