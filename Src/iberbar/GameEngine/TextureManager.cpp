

#include <iberbar/GameEngine/TextureManager.h>
#include <iberbar/GameEngine/BaseResourceManager.h>
#include <iberbar/RHI/Device.h>



iberbar::Game::CTextureManager::CTextureManager( RHI::IDevice* pDevice )
	: m_pDevice( pDevice )
	, m_TextureList()
{
	m_pDevice->AddRef();
}


iberbar::Game::CTextureManager::~CTextureManager()
{
	UNKNOWN_SAFE_RELEASE_NULL( m_pDevice );
	for ( size_t i = 0, s = m_TextureList.size(); i < s; i++ )
	{
		UNKNOWN_SAFE_RELEASE_NULL( m_TextureList[ i ].pTexture );
	}
	m_TextureList.clear();
}


bool iberbar::Game::CTextureManager::GetTextureA( const char* strName, RHI::ITexture** ppOutTexture )
{
	for ( size_t i = 0, s = m_TextureList.size(); i < s; i++ )
	{
		if ( strcmp( m_TextureList[ i ].strName.c_str(), strName ) == 0 )
		{
			//pTexture = m_TextureList[ i ].pTexture;
			if ( ppOutTexture )
			{
				if ( *ppOutTexture )
					(*ppOutTexture)->Release();
				(*ppOutTexture) = m_TextureList[ i ].pTexture;
				(*ppOutTexture)->AddRef();
			}
			return true;
		}
	}
	return false;
}


iberbar::CResult iberbar::Game::CTextureManager::AddTexture( const char* strName, const void* pFileMemory, size_t nFileMemorySize )
{
	if ( GetTextureA( strName ) == true )
	{
		return CResult();
	}

	TSmartRefPtr<RHI::ITexture> pTexture = nullptr;
	m_pDevice->CreateTexture( &pTexture );

	CResult ret = pTexture->CreateFromFileInMemory( pFileMemory, nFileMemorySize );
	if ( ret.IsOK() == false )
		return ret;
	UTextureNode Node;
	Node.pTexture = pTexture;
	Node.pTexture->AddRef();
	Node.strName = strName;
	m_TextureList.push_back( Node );

	return CResult();
}


iberbar::CResult iberbar::Game::CTextureManager::GetOrCreateTextureA( const char* strName, RHI::ITexture** ppOutTexture )
{
	TSmartRefPtr<RHI::ITexture> pTexture = nullptr;
	for ( size_t i = 0, s = m_TextureList.size(); i < s; i++ )
	{
		if ( strcmp( m_TextureList[ i ].strName.c_str(), strName ) == 0 )
		{
			pTexture = m_TextureList[ i ].pTexture;
			break;
		}
	}

	if ( pTexture == nullptr )
	{
		m_pDevice->CreateTexture( &pTexture );

		std::string strFileFull = CResourceFileSystem::GetResoucePath( strName );
		CResult ret = pTexture->CreateFromFileA( strFileFull.c_str() );
		if ( ret.IsOK() == false )
			return ret;
		UTextureNode Node;
		Node.pTexture = pTexture;
		Node.pTexture->AddRef();
		Node.strName = strName;
		m_TextureList.push_back( Node );
	}

	if ( ppOutTexture )
	{
		if ( *ppOutTexture )
			(*ppOutTexture)->Release();
		(*ppOutTexture) = pTexture;
		(*ppOutTexture)->AddRef();
	}

	return CResult();
}