

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


iberbar::CResult iberbar::Game::CShaderManager::GetOrCreateShader( RHI::EShaderType eShaderType, const char* strName, RHI::IShader** ppOutShader )
{
	

	TSmartRefPtr<RHI::IShader> pShader;

	if ( FindShader( eShaderType, strName, &pShader ) == false )
	{
		CResult ret;

		std::string strVS = CombineShaderFilePath( eShaderType, strName );
		switch ( eShaderType )
		{
			case RHI::EShaderType::VertexShader:
				ret = m_pRHIDevice->CreateVertexShader( &pShader );
				break;
			case RHI::EShaderType::PixelShader:
				ret = m_pRHIDevice->CreatePixelShader( &pShader );
				break;
			case RHI::EShaderType::HullShader:
				ret = m_pRHIDevice->CreateHullShader( &pShader );
				break;
			case RHI::EShaderType::GeometryShader:
				ret = m_pRHIDevice->CreateGeometryShader( &pShader );
				break;
			case RHI::EShaderType::DomainShader:
				ret = m_pRHIDevice->CreateDomainShader( &pShader );
				break;
			case RHI::EShaderType::ComputeShader:
				ret = m_pRHIDevice->CreateComputeShader( &pShader );
				break;
			default:
				return MakeResult( ResultCode::Bad, "unknown shader type" );
				break;
		}
		
		if ( ret.IsOK() == false )
			return ret;

		ret = pShader->LoadFromFile( strVS.c_str() );
		if ( ret.IsOK() == false )
			return ret;

		_ShaderNode node;
		node.eShaderType = eShaderType;
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


bool iberbar::Game::CShaderManager::FindShader( RHI::EShaderType eShaderType, const char* strName, RHI::IShader** ppOutShader )
{
	for ( int i = 0, s = (int)m_ShaderNodeList.size(); i < s; i++ )
	{
		if ( m_ShaderNodeList[ i ].eShaderType == eShaderType && strcmp( strName, m_ShaderNodeList[ i ].strName.c_str() ) == 0 )
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


std::string iberbar::Game::CShaderManager::CombineShaderFilePath( RHI::EShaderType eShaderType, const char* strName )
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
