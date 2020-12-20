
#include <iberbar/RHI/D3D9/Shader.h>
#include <iberbar/RHI/D3D9/Device.h>



iberbar::RHI::D3D9::CShader::CShader( CDevice* pDevice )
    : IShader()
    , m_pDevice( pDevice )
    , m_pD3DVertexShader( nullptr )
    , m_pD3DVertexShaderConstTable( nullptr )
    , m_pD3DPixelShader( nullptr )
    , m_pD3DPixelShaderConstTable( nullptr )
{
    m_pDevice->AddRef();
}


iberbar::RHI::D3D9::CShader::~CShader()
{
    UNKNOWN_SAFE_RELEASE_NULL( m_pDevice );
    D3D_SAFE_RELEASE( m_pD3DVertexShader );
    D3D_SAFE_RELEASE( m_pD3DPixelShader );
    D3D_SAFE_RELEASE( m_pD3DVertexShaderConstTable );
    D3D_SAFE_RELEASE( m_pD3DPixelShaderConstTable );
}


iberbar::CResult iberbar::RHI::D3D9::CShader::LoadFromSource( const char* strVertexSource, const char* strPixelSource )
{
    return MakeResult( ResultCode::Bad, "" );
}


iberbar::CResult iberbar::RHI::D3D9::CShader::LoadFromFile( const char* strVertexFile, const char* strPixelFile )
{
    CResult ret;

    ret = LoadD3DVS( strVertexFile );
    if ( ret.IsOK() == false )
        return ret;

    ret = LoadD3DPS( strPixelFile );
    if ( ret.IsOK() == false )
        return ret;

    return CResult();
}


iberbar::CResult iberbar::RHI::D3D9::CShader::LoadD3DVS( const char* strVSFile )
{
    ID3DXBuffer* shader = nullptr;
    ID3DXBuffer* errorBuffer = nullptr;

    DWORD flag = D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION;
    //DWORD flag = 0;

    HRESULT hResult;
    CResult ret;

    hResult = D3DXCompileShaderFromFileA(
        strVSFile,
        0,
        0,
        "Main",
        "vs_2_0",
        flag,
        &shader,
        &errorBuffer,
        &m_pD3DVertexShaderConstTable );
    if ( FAILED( hResult ) )
    {
        if ( errorBuffer == nullptr )
            ret = MakeResult( ResultCode::Bad, "Failed to load VerterShader file" );
        else
            ret = MakeResult( ResultCode::Bad, (const char*)errorBuffer->GetBufferPointer() );
        goto _completed;
    }

    hResult = m_pDevice->GetD3DDevice()->CreateVertexShader( (const DWORD*)shader->GetBufferPointer(), &m_pD3DVertexShader );
    if ( FAILED( hResult ) )
    {
        ret = MakeResult( ResultCode::Bad, DXGetErrorDescriptionA( hResult ) );
        goto _completed;
    }

_completed:
    D3D_SAFE_RELEASE( shader );
    D3D_SAFE_RELEASE( errorBuffer );

    return ret;
}


iberbar::CResult iberbar::RHI::D3D9::CShader::LoadD3DPS( const char* strPSFile )
{
    ID3DXBuffer* shader = nullptr;
    ID3DXBuffer* errorBuffer = nullptr;

    DWORD flag = D3DXSHADER_DEBUG;

    HRESULT hResult;
    CResult ret;

    hResult = D3DXCompileShaderFromFileA(
        strPSFile,
        0,
        0,
        "Main",
        "ps_2_0",
        flag,
        &shader,
        &errorBuffer,
        &m_pD3DPixelShaderConstTable );
    if ( FAILED( hResult ) )
    {
        if ( errorBuffer == nullptr )
            ret = MakeResult( ResultCode::Bad, "Failed to load PixelShader file" );
        else
            ret = MakeResult( ResultCode::Bad, (const char*)errorBuffer->GetBufferPointer() );
        goto _completed;
    }

    hResult = m_pDevice->GetD3DDevice()->CreatePixelShader( (const DWORD*)shader->GetBufferPointer(), &m_pD3DPixelShader );
    if ( FAILED( hResult ) )
    {
        ret = MakeResult( ResultCode::Bad, DXGetErrorDescriptionA( hResult ) );
        goto _completed;
    }

_completed:
    D3D_SAFE_RELEASE( shader );
    D3D_SAFE_RELEASE( errorBuffer );

    //D3DSAMP_MINFILTER;
    //D3DTEXTUREFILTERTYPE::D3DTEXF_LINEAR;
    ////auto pD3DPixelShader = m_pShaderState->GetShader()->GetD3DPixelShader();
    //ID3DXConstantTable* pD3DConstTable = m_pD3DPixelShaderConstTable;
    ////pD3DConstTable->SetDefaults( m_pDevice->GetD3DDevice() );
    ////auto hTex = pD3DConstTable->GetConstantByName( 0, "g_MeshTexture" );
    //auto hTex = pD3DConstTable->GetConstantByName( 0, "MeshTextureSampler" );
    //UINT count;
    //D3DXCONSTANT_DESC Tex0Desc;
    //pD3DConstTable->GetConstantDesc( hTex, &Tex0Desc, &count );
    ////pD3DConstTable->SetBool();

    return ret;
}

