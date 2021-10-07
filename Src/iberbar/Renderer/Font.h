#pragma once

#include <iberbar/Renderer/Headers.h>
#include <iberbar/Utility/Result.h>
#include <iberbar/Font/Types.h>
#include <iberbar/Font/CharMapper.h>



namespace iberbar
{
	class CFontDeviceFt;
	class CFontFaceFt;

	namespace RHI
	{
		class IDevice;
		class ITexture;
	}

	namespace Renderer
	{
		class CFontTextureCache;

		enum class UFontCharVocabularyType
		{
			Unknown, // Î´ÖªµÄ
			Latin,   // Latin·ûºÅ
		};


		


		struct UFontCharBitmap
		{
			static const int sTextureIndex_UnInitialized = -1;
			static const int sTextureIndex_JustNoTexture = -2;


			UFontCharBitmap()
				: nChar( 0 )
				, nCharWidth( 0 )
				, nTextureIndex( sTextureIndex_UnInitialized )
				, nDeltaX( 0 )
				, nDeltaY( 0 )
				, TextureSize( 0, 0 )
				, rcTexCoord( 0, 0, 0, 0 )
			{
			}
			UFontCharBitmap( const UFontCharBitmap& Bitmap )
				: nChar( Bitmap.nChar )
				, nCharWidth( Bitmap.nCharWidth )
				, nTextureIndex( Bitmap.nTextureIndex )
				, nDeltaX( Bitmap.nDeltaX )
				, nDeltaY( Bitmap.nDeltaY )
				, TextureSize( Bitmap.TextureSize )
				, rcTexCoord( Bitmap.rcTexCoord )
			{
			}

			wchar_t nChar;
			int nCharWidth;
			int nTextureIndex;
			int nDeltaX;
			int nDeltaY;
			CSize2i TextureSize;
			CRect2f rcTexCoord;

			FORCEINLINE int GetCharWidth() const
			{
				return nCharWidth;
			}
		};


		class __iberbarRendererApi__ CFont
			: public CRef
		{
		public:
			CFont(
				RHI::IDevice* pRHIDevice,
				CFontFaceFt* pFace,
				const char* strName,
				int nSize,
				int nWeight,
				int nItalic,
				UFontCharVocabularyType nVocabularyType );
			~CFont();

		public:
			int LoadText( const wchar_t* strText );
			int LoadText( wchar_t nCharFirst, wchar_t nCharLast );

			FORCEINLINE const UFontDesc& GetFontDesc() const { return m_FontDesc; }
			FORCEINLINE const UFontCharBitmap* GetCharBitmap( wchar_t nChar ) const
			{
				UFontCharBitmap* pBitmap = m_pCharMapper->Get( nChar );
				if ( pBitmap == nullptr || pBitmap->nTextureIndex == UFontCharBitmap::sTextureIndex_UnInitialized )
					return nullptr;
				return pBitmap;
			}
			RHI::ITexture* GetTexture( int nIndex );

		private:
			bool AddCharBitmap( wchar_t nChar );

		private:
			CFontFaceFt* m_pFace;
			TFontCharMapper<UFontCharBitmap>* m_pCharMapper;
			CFontTextureCache* m_pTextureCache;
			UFontDesc m_FontDesc;
		};

	}
}
