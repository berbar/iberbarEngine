
#include <iberbar/GameEngine/FontManager.h>
#include <iberbar/GameEngine/BaseResourceManager.h>
#include <iberbar/Font/FontDeviceFreeType.h>
#include <iberbar/RHI/Device.h>


iberbar::Game::CFontManager::CFontManager( RHI::IDevice* pRHIDevice )
	: m_pRHIDevice( pRHIDevice )
	, m_pFontDevice( nullptr )
	, m_FontFaceList()
	, m_FontList()
{
}


iberbar::Game::CFontManager::~CFontManager()
{
	for ( size_t i = 0, s = m_FontFaceList.size(); i < s; i++ )
	{
		UNKNOWN_SAFE_RELEASE_NULL( m_FontFaceList[ i ].pFace );
	}
	for ( size_t i = 0, s = m_FontList.size(); i < s; i++ )
	{
		UNKNOWN_SAFE_RELEASE_NULL( m_FontList[ i ] );
	}
	SAFE_DELETE( m_pFontDevice );
}


iberbar::CResult iberbar::Game::CFontManager::Initial()
{
	m_pFontDevice = new CFontDeviceFt();
	return m_pFontDevice->Initialize();
}


iberbar::CResult iberbar::Game::CFontManager::GetOrCreateFont( Renderer::CFont** ppFont, const UFontDesc& FontDesc, Renderer::UFontCharVocabularyType nVocabularyType )
{
	//return MakeResult( ResultCode::Bad, "" );
	if ( GetFont( ppFont, FontDesc ) == true )
		return CResult();

	CResult ret;

	TSmartRefPtr< CFontFaceFt > pFontFace;
	ret = GetOrCreateFontFace( &pFontFace, FontDesc.FamilyName.c_str() );
	if ( ret.IsOK() == false )
		return ret;

	Renderer::CFont* pFontTemp = new Renderer::CFont(
		m_pRHIDevice,
		pFontFace,
		FontDesc.FamilyName.c_str(),
		FontDesc.Size,
		FontDesc.Weight,
		FontDesc.Italic,
		nVocabularyType
	);

	m_FontList.push_back( pFontTemp );

	if ( ppFont )
	{
		if ( *ppFont )
			(*ppFont)->Release();
		(*ppFont) = pFontTemp;
		(*ppFont)->AddRef();
	}

	return CResult();
}


bool iberbar::Game::CFontManager::GetFont( Renderer::CFont** ppFont, const UFontDesc& FontDesc )
{
 	Renderer::CFont* pFontTemp = nullptr;
	const UFontDesc* pFontDescTemp = nullptr;

	for ( size_t i = 0, s = m_FontList.size(); i < s; i++ )
	{
		pFontDescTemp = &m_FontList[ i ]->GetFontDesc();
		if ( strcmp( pFontDescTemp->FamilyName.c_str(), FontDesc.FamilyName.c_str() ) == 0 &&
			pFontDescTemp->Size == FontDesc.Size &&
			pFontDescTemp->Weight == FontDesc.Weight &&
			pFontDescTemp->Italic == FontDesc.Italic )
		{
			pFontTemp = m_FontList[ i ];
			break;
		}
	}

	if ( pFontTemp == nullptr )
		return false;

	if ( ppFont )
	{
		if ( *ppFont )
			(*ppFont)->Release();
		(*ppFont) = pFontTemp;
		(*ppFont)->AddRef();
	}

	return true;
}


bool iberbar::Game::CFontManager::GetFontDefault( Renderer::CFont** ppFont )
{
	if ( m_FontList.empty() == true )
		return false;

	Renderer::CFont* pFontTemp = m_FontList[ 0 ];

	if ( ppFont )
	{
		if ( *ppFont )
			(*ppFont)->Release();
		(*ppFont) = pFontTemp;
		(*ppFont)->AddRef();
	}

	return true;
}


iberbar::CResult iberbar::Game::CFontManager::GetOrCreateFontFace( CFontFaceFt** ppOutFace, const char* strFile )
{
	CFontFaceFt* pFaceTemp = nullptr;

	for ( size_t i = 0, s = m_FontFaceList.size(); i < s; i++ )
	{
		if ( strcmp( m_FontFaceList[ i ].strName.c_str(), strFile ) == 0 )
		{
			pFaceTemp = m_FontFaceList[ i ].pFace;
			break;
		}
	}

	if ( pFaceTemp == nullptr )
	{
		std::string strFileNew = CResourceFileSystem::GetResoucePath( strFile );

		CResult ret = m_pFontDevice->CreateFace( &pFaceTemp, strFileNew.c_str() );
		if ( ret.IsOK() == false )
			return ret;

		_FontFaceNode node { pFaceTemp, strFile };
		m_FontFaceList.push_back( node );
	}

	if ( ppOutFace )
	{
		if ( *ppOutFace )
			(*ppOutFace)->Release();
		(*ppOutFace) = pFaceTemp;
		(*ppOutFace)->AddRef();
	}


	return CResult();
}
