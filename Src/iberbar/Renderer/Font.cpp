
#include <iberbar/Renderer/Font.h>
#include <iberbar/RHI/Device.h>
#include <iberbar/RHI/Texture.h>
#include <iberbar/Renderer/Renderer.h>
#include <iberbar/Renderer/RendererSprite.h>
#include <iberbar/Font/FreeType.h>
#include <iberbar/Font/FontDrawText.h>
#include <iberbar/Utility/RectClip2d.h>


namespace iberbar
{
	namespace Renderer
	{


		// Œ∆¿Ìª∫¥Ê
		class CFontTextureCache
		{
		public:
			struct UAllocDesc
			{
				int nCharW;
				void* pPixels;
				int nTextureIndex;
				CRect2f rectTextureCoord;
			};
		public:
			CFontTextureCache( RHI::IDevice* pDevice, int nCharHeight );
			~CFontTextureCache();
			void AllocRect( int nBitmapWidth, int nBitmapHeight, const void* pPixels, int* pOutTextureIndex, CRect2f* pOutTextureCoord );
			RHI::ITexture* GetTexture( int nIndex );
			CResult SaveToFilesA( const char* strFile );
#ifdef _WIN32
			CResult SaveToFilesW( const wchar_t* strFile );
#endif

		private:
			int m_nCharH;
			int m_nTextureOffsetX;
			int m_nTextureOffsetY;
			float m_nFix;
			RHI::IDevice* m_pDevice;
			std::vector< RHI::ITexture* > m_pTextures;

			static const int sm_nTextureMaxSize = 512;
		};
	}
}




iberbar::Renderer::CFontTextureCache::CFontTextureCache( RHI::IDevice* pDevice, int nCharHeight )
	: m_nCharH( nCharHeight )
	, m_nTextureOffsetX( 0 )
	, m_nTextureOffsetY( 0 )
	, m_nFix( 0.0f )
	, m_pDevice( pDevice )
	, m_pTextures()
{
	m_pDevice->AddRef();
	if ( m_pDevice->GetApiType() == RHI::UApiType::D3D9 )
	{
		m_nFix = 0.5f;
	}
}


iberbar::Renderer::CFontTextureCache::~CFontTextureCache()
{
	UNKNOWN_SAFE_RELEASE_NULL( m_pDevice );
	SafeReleaseStdVector( m_pTextures );
}


void iberbar::Renderer::CFontTextureCache::AllocRect( int nBitmapWidth, int nBitmapHeight, const void* pPixels, int* pOutTextureIndex, CRect2f* pOutTextureCoord )
{
	RHI::ITexture* pTexture = nullptr;
	int nTextureIndex = -1;

	int nOffsetX = m_nTextureOffsetX;
	int nOffsetY = m_nTextureOffsetY;

	if ( (m_nTextureOffsetX + nBitmapWidth) > sm_nTextureMaxSize )
	{
		nOffsetX = 0;
		nOffsetY += m_nCharH;
	}

	if ( (nOffsetY + m_nCharH) > sm_nTextureMaxSize || m_pTextures.size() == 0 )
	{
		nOffsetX = 0;
		nOffsetY = 0;
		m_pDevice->CreateTexture( &pTexture );
		pTexture->CreateEmpty( sm_nTextureMaxSize, sm_nTextureMaxSize );
		m_pTextures.push_back( pTexture );
	}
	else
	{
		pTexture = (*(m_pTextures.rbegin()));
	}

	nTextureIndex = (int)m_pTextures.size() - 1;

	pTexture->SetPixels( pPixels, nOffsetX, nOffsetY, nBitmapWidth, nBitmapHeight );

	(*pOutTextureIndex) = nTextureIndex;
	(*pOutTextureCoord) = CRect2f(
		(float)nOffsetX + m_nFix,
		(float)nOffsetY + m_nFix,
		(float)(nOffsetX + nBitmapWidth) + m_nFix,
		(float)(nOffsetY + nBitmapHeight) + m_nFix );
	(*pOutTextureCoord) = (*pOutTextureCoord) / (float)sm_nTextureMaxSize;

	m_nTextureOffsetX = nOffsetX + nBitmapWidth;
	m_nTextureOffsetY = nOffsetY;
}


FORCEINLINE iberbar::RHI::ITexture* iberbar::Renderer::CFontTextureCache::GetTexture( int nIndex )
{
	if ( nIndex < 0 || nIndex >= (int)m_pTextures.size() )
		return nullptr;
	return m_pTextures[ nIndex ];
}


iberbar::CResult iberbar::Renderer::CFontTextureCache::SaveToFilesA( const char* strFile )
{
	if ( m_pTextures.empty() == false )
	{
		size_t nSize = m_pTextures.size();
		for ( size_t i = 0; i < nSize; i++ )
		{
			m_pTextures[ i ]->SaveToFileA( strFile );
		}
	}

	return CResult();
}


#ifdef _WIN32
iberbar::CResult iberbar::Renderer::CFontTextureCache::SaveToFilesW( const wchar_t* strFile )
{
	if ( m_pTextures.empty() == false )
	{
		size_t nSize = m_pTextures.size();
		for ( size_t i = 0; i < nSize; i++ )
		{
			m_pTextures[ i ]->SaveToFileW( strFile );
		}
	}

	return CResult();
}
#endif






iberbar::Renderer::CFont::CFont(
	RHI::IDevice* pRHIDevice,
	CFontFaceFt* pFace,
	const char* strName,
	int nSize,
	int nWeight,
	int nItalic,
	UFontCharVocabularyType nVocabularyType )
	: m_pFace( pFace )
	, m_pCharMapper( new TFontCharMapper<UFontCharBitmap>() )
	, m_pTextureCache( new CFontTextureCache( pRHIDevice, nSize ) )
{
	m_pFace->AddRef();
	m_FontDesc.FamilyName = strName;
	m_FontDesc.Size = nSize;
	m_FontDesc.Weight = nWeight;
	m_FontDesc.Italic = (nItalic == 0) ? false : true;
}


iberbar::Renderer::CFont::~CFont()
{
	UNKNOWN_SAFE_RELEASE_NULL( m_pFace );
	SAFE_DELETE( m_pCharMapper );
	SAFE_DELETE( m_pTextureCache );
}


int iberbar::Renderer::CFont::LoadText( const wchar_t* strText )
{
	if ( StringIsNullOrEmpty( strText ) == true )
		return 0;

	m_pFace->SetFontSize( m_FontDesc.Size );
	m_pFace->SetFontWeight( m_FontDesc.Weight );

	const wchar_t* pTemp = strText;
	wchar_t nChar;
	int nLoadedCount = 0;
	while ( *pTemp != 0 )
	{
		nChar = *pTemp;
		if ( GetCharBitmap( nChar ) == nullptr )
		{
			if ( AddCharBitmap( nChar ) == true )
				nLoadedCount++;
		}
		else
		{
			nLoadedCount++;
		}
		pTemp++;
	}

	return nLoadedCount;
}


int iberbar::Renderer::CFont::LoadText( wchar_t nCharFirst, wchar_t nCharLast )
{
	if ( nCharLast < nCharFirst )
		return 0;

	m_pFace->SetFontSize( m_FontDesc.Size );
	m_pFace->SetFontWeight( m_FontDesc.Weight );

	int nLoadedCount = 0;
	for ( wchar_t nChar = nCharFirst; nChar <= nCharLast; nChar++ )
	{
		if ( GetCharBitmap( nChar ) == nullptr )
		{
			if ( AddCharBitmap( nChar ) == true )
				nLoadedCount++;
		}
		else
		{
			nLoadedCount++;
		}
	}

	return nLoadedCount;
}


iberbar::RHI::ITexture* iberbar::Renderer::CFont::GetTexture( int nIndex )
{
	return m_pTextureCache->GetTexture( nIndex );
}


bool iberbar::Renderer::CFont::AddCharBitmap( wchar_t nChar )
{
	int nTextureIndex = -1;
	CRect2f rcTextureCoord;
	CResult ret;
	UFontCharBitmapDesc BitmapDesc;
	BitmapDesc.bOutline = false;
	ret = m_pFace->CreateCharBitmap( nChar, &BitmapDesc, UFontBitsFormat::ARGB );
	if ( ret.IsOK() == true )
	{
		// ∑÷≈‰◊÷∑˚
		m_pTextureCache->AllocRect( BitmapDesc.nBmpWidth, BitmapDesc.nBmpHeight, BitmapDesc.pBitsFill, &nTextureIndex, &rcTextureCoord );

		UFontCharBitmap Bitmap;
		Bitmap.nChar = nChar;
		Bitmap.nCharWidth = BitmapDesc.nCharWidth;
		Bitmap.nTextureIndex = nTextureIndex;
		Bitmap.TextureSize = CSize2i( BitmapDesc.nBmpWidth, BitmapDesc.nBmpHeight );
		Bitmap.rcTexCoord = rcTextureCoord;

		m_pCharMapper->Add( nChar, Bitmap );

		return true;
	}

	return false;
}

