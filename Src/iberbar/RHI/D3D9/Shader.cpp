
#include <iberbar/RHI/D3D9/Shader.h>
#include <iberbar/RHI/D3D9/Device.h>
#include <iberbar/RHI/D3D9/ShaderReflection.h>



iberbar::RHI::D3D9::CShader::CShader( CDevice* pDevice, EShaderType eShaderType )
    : IShader( eShaderType )
    , m_pDevice( pDevice )
    , m_pD3DConstTable( nullptr )
{
    m_pDevice->AddRef();
    m_bIsManaged = true;
}


iberbar::RHI::D3D9::CShader::~CShader()
{
    UNKNOWN_SAFE_RELEASE_NULL( m_pDevice );
    D3D_SAFE_RELEASE( m_pD3DConstTable );
}


iberbar::CResult iberbar::RHI::D3D9::CShader::LoadFromSource( const char* pstrSource )
{
    ComPtr<ID3DXBuffer> pD3DBuffer = nullptr;
    ComPtr<ID3DXBuffer> pD3DError = nullptr;

    DWORD nFlag = 0;
#ifdef _DEBUG
    nFlag |= D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION;
#endif

    HRESULT hResult;

    const char* pstrProfile = nullptr;
    if ( m_eShaderType == EShaderType::VertexShader )
    {
        pstrProfile = "vs_2_0";
    }
    else if ( m_eShaderType == EShaderType::PixelShader )
    {
        pstrProfile = "ps_2_0";
    }
    else
    {
        return MakeResult( ResultCode::Bad, "not support" );
    }

    hResult = D3DXCompileShader(
        pstrSource,
        strlen( pstrSource ),
        nullptr,
        nullptr,
        "Main",
        pstrProfile,
        nFlag,
        &pD3DBuffer,
        &pD3DError,
        &m_pD3DConstTable );
    if ( FAILED( hResult ) )
    {
        if ( pD3DError == nullptr )
            return MakeResult( ResultCode::Bad, "Failed to load VerterShader file" );

        return MakeResult( ResultCode::Bad, (const char*)pD3DError->GetBufferPointer() );
    }

    CResult Ret;

    Ret = Load( pD3DBuffer->GetBufferPointer(), pD3DBuffer->GetBufferSize() );
    if ( Ret.IsOK() == false )
        return Ret;

    Ret = m_Reflection.Initial( m_pD3DConstTable );
    if ( Ret.IsOK() == false )
        return Ret;

    return CResult();
}


iberbar::CResult iberbar::RHI::D3D9::CShader::LoadFromFile( const char* pstrFile )
{
    ComPtr<ID3DXBuffer> pD3DBuffer = nullptr;
    ComPtr<ID3DXBuffer> pD3DError = nullptr;

    DWORD nFlag = 0;
#ifdef _DEBUG
    nFlag |= D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION;
#endif

    HRESULT hResult;

    const char* pstrProfile = nullptr;
    if ( m_eShaderType == EShaderType::VertexShader )
    {
        pstrProfile = "vs_2_0";
    }
    else if ( m_eShaderType == EShaderType::PixelShader )
    {
        pstrProfile = "ps_2_0";
    }
    else
    {
        return MakeResult( ResultCode::Bad, "not support" );
    }

    hResult = D3DXCompileShaderFromFileA(
        pstrFile,
        0,
        0,
        "Main",
        pstrProfile,
        nFlag,
        &pD3DBuffer,
        &pD3DError,
        &m_pD3DConstTable );
    if ( FAILED( hResult ) )
    {
        if ( pD3DError == nullptr )
            return MakeResult( ResultCode::Bad, "Failed to load VerterShader file" );

        return MakeResult( ResultCode::Bad, (const char*)pD3DError->GetBufferPointer() );
    }

    CResult Ret;

    Ret = Load( pD3DBuffer->GetBufferPointer(), pD3DBuffer->GetBufferSize() );
    if ( Ret.IsOK() == false )
        return Ret;

    Ret = m_Reflection.Initial( m_pD3DConstTable );
    if ( Ret.IsOK() == false )
        return Ret;

    return CResult();
}






iberbar::RHI::D3D9::CVertexShader::CVertexShader( CDevice* pDevice )
    : CShader( pDevice, EShaderType::VertexShader )
    , m_pD3DShader( nullptr )
{

}


iberbar::CResult iberbar::RHI::D3D9::CVertexShader::Load( const void* pCodes, uint32 nCodeLen )
{
    HRESULT hResult = m_pDevice->GetD3DDevice()->CreateVertexShader( (const DWORD*)pCodes, &m_pD3DShader );
    if ( FAILED( hResult ) )
    {
        return MakeResult( ResultCode::Bad, DXGetErrorDescriptionA( hResult ) );
    }
    return CResult();
}







iberbar::RHI::D3D9::CPixelShader::CPixelShader( CDevice* pDevice )
    : CShader( pDevice, EShaderType::PixelShader )
    , m_pD3DShader( nullptr )
{

}


iberbar::CResult iberbar::RHI::D3D9::CPixelShader::Load( const void* pCodes, uint32 nCodeLen )
{
    HRESULT hResult = m_pDevice->GetD3DDevice()->CreatePixelShader( (const DWORD*)pCodes, &m_pD3DShader );
    if ( FAILED( hResult ) )
    {
        return MakeResult( ResultCode::Bad, DXGetErrorDescriptionA( hResult ) );
    }
    return CResult();
}

