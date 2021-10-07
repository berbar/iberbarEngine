
#include <stdio.h>
#include <wrl/client.h>

//#include <d3dx9.h>
//#include <DxErr.h>

#include <d3d11.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>

#include <string>


using namespace Microsoft::WRL;

void TestD3D9();
void TestD3D11();
void TestD3D12();


std::string DXGetErr( HRESULT hr )
{
    char err[ 256 ];
    memset( err, 0, sizeof( err ) );
    FormatMessageA( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ARGUMENT_ARRAY,
        nullptr, hr, 0,
        err, 256, nullptr );

    return err;
}



void TestD3D9()
{
//    HRESULT hResult;
//    UINT nFlag = D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION;
//    ComPtr<ID3DXBuffer> pD3DErrorBuffer;
//    ComPtr<ID3DXConstantTable> pD3DConstantTable;
//    hResult = D3DXCompileShaderFromFileW(
//        L"D3D9/VertexShader.hlsl",
//        0,
//        0,
//        "main",
//        "vs_2_0",
//        nFlag,
//        NULL,
//        &pD3DErrorBuffer,
//        &pD3DConstantTable );
//    if ( FAILED( hResult ) )
//    {
//        if ( pD3DErrorBuffer == nullptr )
//            printf_s( "Failed to load VerterShader file" );
//        else
//            printf_s( (const char*)pD3DErrorBuffer->GetBufferPointer() );
//        return;
//    }
//
//    D3DXCONSTANTTABLE_DESC TableDesc;
//    hResult = pD3DConstantTable->GetDesc( &TableDesc );
//    if ( FAILED( hResult ) )
//    {
//        printf_s( DXGetErrorDescriptionA( hResult ) );
//        return;
//    }
//
//    D3DXHANDLE pHandle = nullptr;
//    D3DXHANDLE pHandleMember = nullptr;
//    D3DXCONSTANT_DESC ConstDesc;
//    D3DXCONSTANT_DESC MemberDesc;
//    UINT nCount;
//    for ( UINT i = 0; i < TableDesc.Constants; i++ )
//    {
//        pHandle = pD3DConstantTable->GetConstant( nullptr, i );
//        if ( pHandle == nullptr )
//            continue;
//
//        hResult = pD3DConstantTable->GetConstantDesc( pHandle, &ConstDesc, &nCount );
//        if ( FAILED( hResult ) )
//            continue;
//
//        printf_s( "Variable: Name=%s, Type=%d, Bytes=%d\n", ConstDesc.Name, ConstDesc.Type, ConstDesc.Bytes );
//        if ( ConstDesc.Type == D3DXPT_VOID && ConstDesc.Class == D3DXPC_STRUCT )
//        {
//            for ( UINT j = 0; j < ConstDesc.StructMembers; j++ )
//            {
//                pHandleMember = pD3DConstantTable->GetConstant( pHandle, j );
//                hResult = pD3DConstantTable->GetConstantDesc( pHandleMember, &MemberDesc, &nCount );
//                if ( FAILED( hResult ) )
//                    continue;
//                printf_s( "\tMember: Name=%s, Type=%d, Bytes=%d\n", MemberDesc.Name, MemberDesc.Type, MemberDesc.Bytes );
//            }
//        }
//    }
}



void TestD3D11()
{
    ComPtr<ID3DBlob> pD3DBlobFile = nullptr;
    ComPtr<ID3DBlob> pD3DBlob = nullptr;
    ComPtr<ID3DBlob> pD3DBlobError = nullptr;
    ComPtr<ID3D11ShaderReflection> pD3DShaderReflection;

    HRESULT hResult;

    hResult = D3DReadFileToBlob( L"D3D\\PixelShader.hlsl", &pD3DBlobFile );
    if ( FAILED( hResult ) )
    {
        printf_s( DXGetErr( hResult ).c_str() );
        return;
    }

    hResult = D3DCompile( pD3DBlobFile->GetBufferPointer(), pD3DBlobFile->GetBufferSize(), "", NULL, NULL, "Main", "ps_5_0", 0, 0, &pD3DBlob, &pD3DBlobError);
    if ( FAILED( hResult ) )
    {
        return;
    }

    hResult = D3DReflect( pD3DBlob->GetBufferPointer(), pD3DBlob->GetBufferSize(), __uuidof( ID3D11ShaderReflection ), (void**)&pD3DShaderReflection );
    if ( FAILED( hResult ) )
    {
        printf_s( DXGetErr( hResult ).c_str() );
        return;
    }

    D3D11_SHADER_DESC D3DDesc;
    D3D11_SHADER_BUFFER_DESC D3DBufferDesc;
    D3D11_SHADER_VARIABLE_DESC D3DVariableDesc;
    D3D11_SHADER_TYPE_DESC D3DTypeDesc;
    ID3D11ShaderReflectionConstantBuffer* pD3DShaderReflectionCBuffer = nullptr;
    ID3D11ShaderReflectionVariable* pD3DShaderReflectionVariable = nullptr;
    ID3D11ShaderReflectionType* pD3DShaderReflectionType = nullptr;

    pD3DShaderReflection->GetDesc( &D3DDesc );
    if ( D3DDesc.ConstantBuffers > 0 )
    {
        for ( UINT nCBuffIndex = 0, nCBufferCount = D3DDesc.ConstantBuffers; nCBuffIndex < nCBufferCount; nCBuffIndex++ )
        {
            pD3DShaderReflectionCBuffer = pD3DShaderReflection->GetConstantBufferByIndex( nCBuffIndex );
            pD3DShaderReflectionCBuffer->GetDesc( &D3DBufferDesc );

            for ( UINT nVarIndex = 0, nVarCount = D3DBufferDesc.Variables; nVarIndex < nVarCount; nVarIndex++ )
            {
                pD3DShaderReflectionVariable = pD3DShaderReflectionCBuffer->GetVariableByIndex( nVarIndex );
                pD3DShaderReflectionVariable->GetDesc( &D3DVariableDesc );

                pD3DShaderReflectionType = pD3DShaderReflectionVariable->GetType();
                pD3DShaderReflectionType->GetDesc( &D3DTypeDesc );

                if ( D3DTypeDesc.Class == D3D_SVC_STRUCT )
                {
                    for ( UINT nMemberIndex = 0, nMemberCount = D3DTypeDesc.Members; nMemberIndex < nMemberCount; nMemberIndex++ )
                    {
                        auto pMember = pD3DShaderReflectionType->GetMemberTypeByIndex( nMemberIndex );
                        auto pMemberTypeName = pD3DShaderReflectionType->GetMemberTypeName( nMemberIndex );
                        D3D11_SHADER_TYPE_DESC MemberTypeDesc;
                        pMember->GetDesc( &MemberTypeDesc );
                        int jj = 0;
                    }

                }
            }
        }
    }



    for ( int i = 0, s = D3DDesc.BoundResources; i < s; i++ )
    {
        D3D11_SHADER_INPUT_BIND_DESC InputBinDesc;
        pD3DShaderReflection->GetResourceBindingDesc( i, &InputBinDesc );

        if ( InputBinDesc.Type == D3D_SIT_TEXTURE )
        {
            auto pVarTexture = pD3DShaderReflection->GetVariableByName( InputBinDesc.Name );
            D3D11_SHADER_VARIABLE_DESC VarTextureDesc;
            pVarTexture->GetDesc( &VarTextureDesc );
            printf_s( InputBinDesc.Name );
        }
    }
   
}



int main()
{
    TestD3D9();
    TestD3D11();
    return 0;
}
