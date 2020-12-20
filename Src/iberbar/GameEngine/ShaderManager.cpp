

#include <iberbar/GameEngine/ShaderManager.h>
#include <iberbar/GameEngine/BaseResourceManager.h>
#include <iberbar/RHI/Device.h>



iberbar::Game::CShaderManager::CShaderManager( RHI::IDevice* pRHIDevice )
	: m_pRHIDevice( pRHIDevice )
	, m_nApiType( pRHIDevice->GetApiType() )
	, m_strRootDir( "" )
	, m_ShaderNodeList()
{
	m_pRHIDevice->AddRef();
}


iberbar::Game::CShaderManager::~CShaderManager()
{
	for ( int i = 0; i < m_ShaderNodeList.size(); i++ )
	{
		UNKNOWN_SAFE_RELEASE_NULL( m_ShaderNodeList[ i ].pShader );
	}
	m_ShaderNodeList.clear();
	UNKNOWN_SAFE_RELEASE_NULL( m_pRHIDevice );
}


iberbar::CResult iberbar::Game::CShaderManager::GetOrCreateShader( const char* strName, RHI::IShader** ppOutShader )
{
	std::string strVS;
	std::string strPS;
	CombineShaderFilePath( strName, strVS, strPS );

	TSmartRefPtr<RHI::IShader> pShader;

	if ( FindShader( strName, &pShader ) == false )
	{
		CResult ret;

		ret = m_pRHIDevice->CreateShader( &pShader );
		if ( ret.IsOK() == false )
			return ret;

		ret = pShader->LoadFromFile( strVS.c_str(), strPS.c_str() );
		if ( ret.IsOK() == false )
			return ret;

		_ShaderNode node;
		node.strName = strName;
		node.pShader = pShader;
		node.pShader->AddRef();
		m_ShaderNodeList.push_back( node );
	}

	if ( ppOutShader != nullptr )
	{
		UNKNOWN_SAFE_RELEASE_NULL( *ppOutShader );
		(*ppOutShader) = pShader;
		(*ppOutShader)->AddRef();
	}

	return CResult();
}


bool iberbar::Game::CShaderManager::FindShader( const char* strName, RHI::IShader** ppOutShader )
{
	for ( size_t i = 0; i < m_ShaderNodeList.size(); i++ )
	{
		if ( strcmp( strName, m_ShaderNodeList[ i ].strName.c_str() ) == 0 )
		{
			if ( ppOutShader != nullptr )
			{
				UNKNOWN_SAFE_RELEASE_NULL( *ppOutShader );
				(*ppOutShader) = m_ShaderNodeList[ i ].pShader;
				(*ppOutShader)->AddRef();
			}

			return true;
		}
	}

	return false;
}


void iberbar::Game::CShaderManager::CombineShaderFilePath( const char* strName, std::string& strVS, std::string& strPS )
{
	strVS += m_strRootDir + "/" + strName;
	strPS += m_strRootDir + "/" + strName;


	if ( IsOpenGLApi( m_nApiType ) )
	{
		strVS += "_VS.glsl";
		strPS += "_PS.glsl";
	}
#ifdef _WIN32
	else if ( IsD3DApi( m_nApiType ) )
	{
		strVS += "_VS.hlsl";
		strPS += "_PS.hlsl";
	}
#endif

	strVS = CResourceFileSystem::GetResoucePath( strVS );
	strPS = CResourceFileSystem::GetResoucePath( strPS );
}
