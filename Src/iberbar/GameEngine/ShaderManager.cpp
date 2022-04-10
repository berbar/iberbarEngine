

#include <iberbar/GameEngine/ShaderManager.h>
#include <iberbar/GameEngine/BaseResourceManager.h>
#include <iberbar/RHI/ShaderState.h>
#include <iberbar/RHI/Device.h>
#include <iberbar/Utility/FileHelper.h>





iberbar::Game::CShaderLoader::CShaderLoader( RHI::IDevice* pRHIDevice )
	: m_pRHIDevice( pRHIDevice )
	, m_nApiType( m_pRHIDevice->GetApiType() )
	, m_strRootDir()
{
}


iberbar::CResult iberbar::Game::CShaderLoader::LoadShaderProgram( const char* pstrName, RHI::IShaderProgram** ppOutShaderProgram )
{
	CResult cResult;
	std::string strShaderPath;
	TSmartRefPtr<RHI::IShader> pVS = nullptr;
	TSmartRefPtr<RHI::IShader> pPS = nullptr;
	TSmartRefPtr<RHI::IShader> pGS = nullptr;
	TSmartRefPtr<RHI::IShader> pHS = nullptr;
	TSmartRefPtr<RHI::IShader> pDS = nullptr;
	
	strShaderPath = CombineShaderFilePath( RHI::EShaderType::VertexShader, pstrName );
	if ( FileApi::IsExistFileA( strShaderPath.c_str() ) == true )
	{
		m_pRHIDevice->CreateVertexShader( &pVS );
		cResult = pVS->LoadFromFile( strShaderPath.c_str() );
		if ( cResult.IsOK() == false )
			return cResult;
	}

	strShaderPath = CombineShaderFilePath( RHI::EShaderType::PixelShader, pstrName );
	if ( FileApi::IsExistFileA( strShaderPath.c_str() ) == true )
	{
		m_pRHIDevice->CreatePixelShader( &pPS );
		cResult = pPS->LoadFromFile( strShaderPath.c_str() );
		if ( cResult.IsOK() == false )
			return cResult;
	}

	strShaderPath = CombineShaderFilePath( RHI::EShaderType::HullShader, pstrName );
	if ( FileApi::IsExistFileA( strShaderPath.c_str() ) == true )
	{
		m_pRHIDevice->CreateHullShader( &pHS );
		cResult = pHS->LoadFromFile( strShaderPath.c_str() );
		if ( cResult.IsOK() == false )
			return cResult;
	}

	strShaderPath = CombineShaderFilePath( RHI::EShaderType::GeometryShader, pstrName );
	if ( FileApi::IsExistFileA( strShaderPath.c_str() ) == true )
	{
		m_pRHIDevice->CreateGeometryShader( &pGS );
		cResult = pGS->LoadFromFile( strShaderPath.c_str() );
		if ( cResult.IsOK() == false )
			return cResult;
	}

	strShaderPath = CombineShaderFilePath( RHI::EShaderType::DomainShader, pstrName );
	if ( FileApi::IsExistFileA( strShaderPath.c_str() ) == true )
	{
		m_pRHIDevice->CreateDomainShader( &pDS );
		cResult = pDS->LoadFromFile( strShaderPath.c_str() );
		if ( cResult.IsOK() == false )
			return cResult;
	}

	return m_pRHIDevice->CreateShaderProgram( ppOutShaderProgram, pVS, pPS, pHS, pGS, pDS );
}





iberbar::Game::CShaderManager::CShaderManager()
	: m_ShaderProgramNodeList()
{
}


iberbar::Game::CShaderManager::~CShaderManager()
{
	for ( int i = 0; i < m_ShaderProgramNodeList.size(); i++ )
	{
		UNKNOWN_SAFE_RELEASE_NULL( m_ShaderProgramNodeList[ i ].pShaderProgram );
	}
	m_ShaderProgramNodeList.clear();
}


void iberbar::Game::CShaderManager::AddShaderProgram( const char* pstrName, RHI::IShaderProgram* pShaderProgram )
{
	assert( pstrName );
	assert( pShaderProgram );

	if ( GetShaderProgram( pstrName, nullptr ) == true )
		return;

	_Node Node;
	Node.pShaderProgram = pShaderProgram;
	Node.pShaderProgram->AddRef();
	Node.strName = pstrName;
	m_ShaderProgramNodeList.push_back( Node );
}


bool iberbar::Game::CShaderManager::GetShaderProgram( const char* pstrName, RHI::IShaderProgram** ppOutShaderProgram )
{
	assert( pstrName );

	for ( int i = 0, s = m_ShaderProgramNodeList.size(); i < s; i++ )
	{
		if ( strcmp( m_ShaderProgramNodeList[ i ].strName.c_str(), pstrName ) == 0 )
		{
			if ( ppOutShaderProgram != nullptr )
			{
				UNKNOWN_SAFE_RELEASE_NULL( *ppOutShaderProgram );
				(*ppOutShaderProgram) = m_ShaderProgramNodeList[ i ].pShaderProgram;
				(*ppOutShaderProgram)->AddRef();
			}
			return true;
		}
	}

	return false;
}


std::string iberbar::Game::CShaderLoader::CombineShaderFilePath( RHI::EShaderType eShaderType, const char* strName )
{
	std::string strFilePath = m_strRootDir + "/" + strName;

	switch ( eShaderType )
	{
		case RHI::EShaderType::VertexShader:
			strFilePath += "_VS";
			break;
		case RHI::EShaderType::PixelShader:
			strFilePath += "_PS";
			break;
		case RHI::EShaderType::HullShader:
			strFilePath += "_HS";
			break;
		case RHI::EShaderType::GeometryShader:
			strFilePath += "_GS";
			break;
		case RHI::EShaderType::DomainShader:
			strFilePath += "_DS";
			break;
		case RHI::EShaderType::ComputeShader:
			strFilePath += "_CS";
			break;
		default:break;
	}

	if ( IsOpenGLApi( m_nApiType ) )
	{
		strFilePath += ".glsl";
	}
#ifdef _WIN32
	else if ( IsD3DApi( m_nApiType ) )
	{
		strFilePath += ".hlsl";
	}
#endif

	return CResourceFileSystem::GetResoucePath( strFilePath );
}
