
#include <iberbar/RHI/D3D11/Shader.h>
#include <iberbar/RHI/D3D11/ShaderReflection.h>
#include <iberbar/RHI/D3D11/Buffer.h>
#include <iberbar/RHI/D3D11/Device.h>
#include <iberbar/Utility/String.h>



iberbar::RHI::D3D11::CShader::CShader( CDevice* pDevice, EShaderType eShaderType )
	: IShader( eShaderType )
	, m_pDevice( pDevice )
	, m_pReflection( nullptr )
	//, m_ConstantBuffers()
	//, m_ConstBuffersData()
{
	assert( m_pDevice );
	m_pDevice->AddRef();
}


iberbar::RHI::D3D11::CShader::~CShader()
{
	SAFE_DELETE(m_pReflection);
	UNKNOWN_SAFE_RELEASE_NULL( m_pDevice );
	//auto iter = m_ConstantBuffers.begin();
	//auto end = m_ConstantBuffers.end();
	//for ( ; iter != end; iter++ )
	//{
	//	SAFE_DELETE( *iter );
	//}
}


iberbar::CResult iberbar::RHI::D3D11::CShader::LoadFromFile( const char* pstrFile )
{
	ComPtr<ID3DBlob> pD3DBlob = nullptr;
	ComPtr<ID3DBlob> pD3DBlobError = nullptr;
	HRESULT hResult;
	wchar_t strFileUnicode[MAX_PATH] = {0};
	Utf8ToUnicode( pstrFile, strFileUnicode, MAX_PATH );
	hResult = D3DReadFileToBlob( strFileUnicode, &pD3DBlob );
	if ( FAILED( hResult ) )
	{
		return MakeResult( ResultCode::Bad, "failed to D3DReadFileToBlob, file=%s", pstrFile );
	}

	const char* pstrTarget = nullptr;
	if ( m_eShaderType == EShaderType::VertexShader )
	{
		pstrTarget = "vs_5_0";
	}
	else if ( m_eShaderType == EShaderType::PixelShader )
	{
		pstrTarget = "ps_5_0";
	}
	else if ( m_eShaderType == EShaderType::HullShader )
	{
		pstrTarget = "hs_5_0";
	}
	else if ( m_eShaderType == EShaderType::GeometryShader )
	{
		pstrTarget = "gs_5_0";
	}
	else if ( m_eShaderType == EShaderType::DomainShader )
	{
		pstrTarget = "ds_5_0";
	}
	else if ( m_eShaderType == EShaderType::ComputeShader )
	{
		pstrTarget = "cs_5_0";
	}
	else
	{
		return MakeResult( ResultCode::Bad, "not support" );
	}

	DWORD nShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	// 设置 D3DCOMPILE_DEBUG 标志用于获取着色器调试信息。该标志可以提升调试体验，
	// 但仍然允许着色器进行优化操作
	nShaderFlags |= D3DCOMPILE_DEBUG;

	// 在Debug环境下禁用优化以避免出现一些不合理的情况
	nShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	hResult = D3DCompileFromFile( strFileUnicode, NULL, NULL, "Main", pstrTarget, nShaderFlags, 0, &pD3DBlob, &pD3DBlobError );
	if ( FAILED( hResult ) )
	{
		std::string strError;
		if ( hResult == HRESULT_FROM_WIN32( ERROR_PATH_NOT_FOUND ) )
		{
			strError = "not found";
		}
		else if ( pD3DBlobError != nullptr )
		{
			strError = (const char*)pD3DBlobError->GetBufferPointer();
		}
		return CResult( ResultCode::Bad, strError.c_str() );
	}

	return Load( pD3DBlob->GetBufferPointer(), pD3DBlob->GetBufferSize() );
}


iberbar::CResult iberbar::RHI::D3D11::CShader::LoadFromSource( const char* pstrSource )
{
	ComPtr<ID3DBlob> pD3DBlob = nullptr;
	ComPtr<ID3DBlob> pD3DBlobError = nullptr;
	HRESULT hResult;

	hResult = D3DCompile( pstrSource, strlen( pstrSource ), "", NULL, NULL, "Main", NULL, 0, 0, &pD3DBlob, &pD3DBlobError );
	if ( FAILED( hResult ) )
	{
		std::string strError = UnicodeToUtf8( (const wchar_t*)pD3DBlobError->GetBufferPointer() );
		return CResult( ResultCode::Bad, strError.c_str() );
	}

	return Load( pD3DBlob->GetBufferPointer(), pD3DBlob->GetBufferSize() );
}


iberbar::RHI::IShaderReflection* iberbar::RHI::D3D11::CShader::GetReflection()
{
	return m_pReflection;
}


iberbar::CResult iberbar::RHI::D3D11::CShader::CreateReflection( const void* pCodes, uint32 nCodeLen )
{
	m_pReflection = new CShaderReflection();
	CResult cResult = m_pReflection->Initial( pCodes, nCodeLen );
	if ( cResult.IsOK() == false )
		return cResult;

	//int nBufferCount = m_pReflection->GetBufferCountInternal();
	//if ( nBufferCount > 0 )
	//{
	//	m_ConstantBuffers.resize( nBufferCount, nullptr );
	//	for ( int i = 0; i < nBufferCount; i++ )
	//	{
	//		const CShaderReflectionBuffer* pReflectionBuffer = m_pReflection->GetBufferByIndexInternal( i );
	//		m_ConstantBuffers[ i ] = new CConstantBuffer( m_pDevice, pReflectionBuffer->GetSize(), UBufferUsageFlags::Dynamic );
	//		cResult = m_ConstantBuffers[ i ]->Initial();
	//		if ( cResult.IsOK() == false )
	//			return cResult;
	//	}
	//	m_ConstBuffersData.resize( m_pReflection->GetBufferSizeTotal() );
	//	memset( &(m_ConstBuffersData.front()), 0, sizeof( m_pReflection->GetBufferSizeTotal() ) );
	//}

	return CResult();
}






iberbar::RHI::D3D11::CVertexShader::CVertexShader( CDevice* pDevice )
	: CShader( pDevice, EShaderType::VertexShader )
	, m_pD3DShader( nullptr )
	, m_Code()
{
}


iberbar::CResult iberbar::RHI::D3D11::CVertexShader::Load( const void* pCodes, uint32 nCodeLen )
{
	ID3D11Device* pD3DDevice = m_pDevice->GetD3DDevice();
	HRESULT hResult;
	hResult = pD3DDevice->CreateVertexShader( pCodes, nCodeLen, nullptr, &m_pD3DShader );
	if ( FAILED( hResult ) )
	{
		return MakeResult( ResultCode::Bad, "" );
	}

	m_Code.resize( nCodeLen );
	memcpy_s( &m_Code.front(), nCodeLen, pCodes, nCodeLen );

	return CreateReflection( pCodes, nCodeLen );
}





iberbar::RHI::D3D11::CPixelShader::CPixelShader( CDevice* pDevice )
	: CShader( pDevice, EShaderType::PixelShader )
	, m_pD3DShader( nullptr )
{
}


iberbar::CResult iberbar::RHI::D3D11::CPixelShader::Load( const void* pCodes, uint32 nCodeLen )
{
	ID3D11Device* pD3DDevice = m_pDevice->GetD3DDevice();
	HRESULT hResult;
	hResult = pD3DDevice->CreatePixelShader( pCodes, nCodeLen, nullptr, &m_pD3DShader );
	if ( FAILED( hResult ) )
	{
		return MakeResult( ResultCode::Bad, "" );
	}

	return CreateReflection( pCodes, nCodeLen );
}





iberbar::RHI::D3D11::CGeometryShader::CGeometryShader( CDevice* pDevice )
	: CShader( pDevice, EShaderType::GeometryShader )
	, m_pD3DShader( nullptr )
{
}


iberbar::CResult iberbar::RHI::D3D11::CGeometryShader::Load( const void* pCodes, uint32 nCodeLen )
{
	ID3D11Device* pD3DDevice = m_pDevice->GetD3DDevice();
	HRESULT hResult;
	hResult = pD3DDevice->CreateGeometryShader( pCodes, nCodeLen, nullptr, &m_pD3DShader );
	if ( FAILED( hResult ) )
	{
		return MakeResult( ResultCode::Bad, "" );
	}

	return CreateReflection( pCodes, nCodeLen );
}






iberbar::RHI::D3D11::CHullShader::CHullShader( CDevice* pDevice )
	: CShader( pDevice, EShaderType::HullShader )
	, m_pD3DShader( nullptr )
{
}


iberbar::CResult iberbar::RHI::D3D11::CHullShader::Load( const void* pCodes, uint32 nCodeLen )
{
	ID3D11Device* pD3DDevice = m_pDevice->GetD3DDevice();
	HRESULT hResult;
	hResult = pD3DDevice->CreateHullShader( pCodes, nCodeLen, nullptr, &m_pD3DShader );
	if ( FAILED( hResult ) )
	{
		return MakeResult( ResultCode::Bad, "" );
	}

	return CreateReflection( pCodes, nCodeLen );
}






iberbar::RHI::D3D11::CDomainShader::CDomainShader( CDevice* pDevice )
	: CShader( pDevice, EShaderType::DomainShader )
	, m_pD3DShader( nullptr )
{
}


iberbar::CResult iberbar::RHI::D3D11::CDomainShader::Load( const void* pCodes, uint32 nCodeLen )
{
	ID3D11Device* pD3DDevice = m_pDevice->GetD3DDevice();
	HRESULT hResult;
	hResult = pD3DDevice->CreateDomainShader( pCodes, nCodeLen, nullptr, &m_pD3DShader );
	if ( FAILED( hResult ) )
	{
		return MakeResult( ResultCode::Bad, "" );
	}

	return CreateReflection( pCodes, nCodeLen );
}






iberbar::RHI::D3D11::CComputeShader::CComputeShader( CDevice* pDevice )
	: CShader( pDevice, EShaderType::ComputeShader )
	, m_pD3DShader( nullptr )
{
}


iberbar::CResult iberbar::RHI::D3D11::CComputeShader::Load( const void* pCodes, uint32 nCodeLen )
{
	ID3D11Device* pD3DDevice = m_pDevice->GetD3DDevice();
	HRESULT hResult;
	hResult = pD3DDevice->CreateComputeShader( pCodes, nCodeLen, nullptr, &m_pD3DShader );
	if ( FAILED( hResult ) )
	{
		return MakeResult( ResultCode::Bad, "" );
	}

	return CreateReflection( pCodes, nCodeLen );
}