

#include <iberbar/RHI/D3D9/CommandContext.h>
#include <iberbar/RHI/D3D9/Types.h>
#include <iberbar/RHI/D3D9/Device.h>
#include <iberbar/RHI/D3D9/Buffer.h>
#include <iberbar/RHI/D3D9/Shader.h>
#include <iberbar/RHI/D3D9/ShaderState.h>
#include <iberbar/RHI/D3D9/ShaderVariables.h>
#include <iberbar/RHI/D3D9/VertexDeclaration.h>
#include <iberbar/RHI/D3D9/Texture.h>
#include <iberbar/RHI/D3D9/RenderState.h>
#include <iberbar/RHI/ShaderVariables.h>



iberbar::RHI::D3D9::CCommandContext::CCommandContext( CDevice* pDevice )
	: ICommandContext()
	, m_pDevice( pDevice )
	, m_pVertexBuffer( nullptr )
	, m_pIndexBuffer( nullptr )
	, m_pShaderState( nullptr )
	, m_pBlendState( nullptr )
	, m_pBlendStateDefault( nullptr )
	, m_pTexture()
{
	m_pDevice->AddRef();
	memset( m_pTexture, 0, sizeof( m_pTexture ) );
}


iberbar::RHI::D3D9::CCommandContext::~CCommandContext()
{
	for ( int i = 0; i < 8; i++ )
	{
		UNKNOWN_SAFE_RELEASE_NULL( m_pTexture[ i ] );
	}
	UNKNOWN_SAFE_RELEASE_NULL( m_pVertexBuffer );
	UNKNOWN_SAFE_RELEASE_NULL( m_pIndexBuffer );
	UNKNOWN_SAFE_RELEASE_NULL( m_pBlendState );
	UNKNOWN_SAFE_RELEASE_NULL( m_pBlendStateDefault );
	UNKNOWN_SAFE_RELEASE_NULL( m_pDevice );
}


void iberbar::RHI::D3D9::CCommandContext::SetVertexBuffer( IVertexBuffer* pVertexBuffer )
{
	if ( m_pVertexBuffer != pVertexBuffer )
	{
		UNKNOWN_SAFE_RELEASE_NULL( m_pVertexBuffer );
		m_pVertexBuffer = (CVertexBuffer*)pVertexBuffer;
		UNKNOWN_SAFE_ADDREF( m_pVertexBuffer );
	}
}


void iberbar::RHI::D3D9::CCommandContext::SetIndexBuffer( IIndexBuffer* pIndexBuffer )
{
	if ( m_pIndexBuffer != pIndexBuffer )
	{
		UNKNOWN_SAFE_RELEASE_NULL( m_pIndexBuffer );
		m_pIndexBuffer = (CIndexBuffer*)pIndexBuffer;
		UNKNOWN_SAFE_ADDREF( m_pIndexBuffer );
	}
}


void iberbar::RHI::D3D9::CCommandContext::SetShaderState( IShaderState* pShaderState )
{
	if ( m_pShaderState != pShaderState )
	{
		UNKNOWN_SAFE_RELEASE_NULL( m_pShaderState );
		m_pShaderState = (CShaderState*)pShaderState;
		UNKNOWN_SAFE_ADDREF( m_pShaderState );
	}
}


void iberbar::RHI::D3D9::CCommandContext::SetShaderVariableTable( EShaderType eShaderType, IShaderVariableTable* pShaderVariableTable )
{
	if ( m_pShaderVariableTables[ (int)eShaderType ] != pShaderVariableTable )
	{
		UNKNOWN_SAFE_RELEASE_NULL( m_pShaderVariableTables[ (int)eShaderType ] );
		m_pShaderVariableTables[ (int)eShaderType ] = (CShaderVariableTable*)pShaderVariableTable;
		UNKNOWN_SAFE_ADDREF( m_pShaderVariableTables[ (int)eShaderType ] );
	}
}


void iberbar::RHI::D3D9::CCommandContext::SetBlendState( IBlendState* pBlendState )
{
	if ( m_pBlendState == pBlendState )
		return;

	if ( m_pBlendState != nullptr && pBlendState != nullptr && m_pBlendState->Equal( pBlendState ) )
		return;

	UNKNOWN_SAFE_RELEASE_NULL( m_pBlendState );
	m_pBlendState = (CBlendState*)pBlendState;
	if ( m_pBlendState == nullptr )
	{
		m_pBlendState = m_pBlendStateDefault;
		m_pBlendState->AddRef();
	}

	PrepareBlendState();
}


void iberbar::RHI::D3D9::CCommandContext::SetBlendStateDefault( IBlendState* pBlendState )
{
	assert( pBlendState );

	UNKNOWN_SAFE_RELEASE_NULL( m_pBlendStateDefault );
	m_pBlendStateDefault = (CBlendState*)pBlendState;
	m_pBlendStateDefault->AddRef();
	SetBlendState( m_pBlendStateDefault );
}


void iberbar::RHI::D3D9::CCommandContext::DrawElements( UPrimitiveType nPrimitiveType, UIndexFormat nIndexFormat, uint32 nCount, uint32 nOffset )
{
	IDirect3DDevice9* pD3DDevice = m_pDevice->GetD3DDevice();
	PrepareDraw();
	uint32 nVertexCount = nCount / 2 * 4;
	//uint32 nVertexCount = nCount * 3;
	pD3DDevice->DrawIndexedPrimitive( ConvertPrimitiveType( nPrimitiveType ), 0, 0, nVertexCount, 0, nCount );
	CleanResources();
}


void iberbar::RHI::D3D9::CCommandContext::PrepareDraw()
{
	IDirect3DDevice9* pd3dDevice = m_pDevice->GetD3DDevice();

	// ��������������
	m_pDevice->GetD3DDevice()->SetVertexDeclaration( m_pShaderState->GetVertexDeclaration()->GetD3DVertexDeclaration() );
	m_pDevice->SetVertexShader( m_pShaderState->GetVertexShader()->GetD3DShader() );
	m_pDevice->SetPixelShader( m_pShaderState->GetPixelShader()->GetD3DShader() );
	m_pDevice->BindVertexBuffer( m_pVertexBuffer->GetD3DVertexBuffer(), m_pShaderState->GetVertexDeclaration()->GetStride() );
	m_pDevice->BindIndexBuffer( m_pIndexBuffer->GetD3DIndexBuffer() );

	// ׼��shader����
	PrepareShaderVariables();

	//
	//ID3DXConstantTable* pD3DConstTablePixel = pShader->GetD3DPixelShaderConstTable();
	////
	//// ���� shader �� Sampler
	//auto& ShaderVars_Samplers = m_pShaderVariableTable->GetSamplers();
	//if ( ShaderVars_Samplers.empty() == false )
	//{
	//	D3DXHANDLE hTex = nullptr;
	//	UINT nCount;
	//	D3DXCONSTANT_DESC TexDesc;
	//	const UShaderSampler* pSampler = nullptr;
	//	CTexture* pTextureTemp = nullptr;
	//	for ( size_t i = 0, s = ShaderVars_Samplers.size(); i < s; i++ )
	//	{
	//		hTex = pD3DConstTablePixel->GetConstantByName( 0, ShaderVars_Samplers[ i ].strName.c_str() );
	//		if ( hTex == nullptr )
	//			continue;

	//		pD3DConstTablePixel->GetConstantDesc( hTex, &TexDesc, &nCount );
	//		if ( TexDesc.Type == D3DXPT_SAMPLER2D )
	//		{
	//			pSampler = &ShaderVars_Samplers[ i ].Value;
	//			pTextureTemp = (CTexture*)(ITexture*)(pSampler->pTexture);

	//			m_pDevice->SetTexture( TexDesc.RegisterIndex, pTextureTemp == nullptr ? nullptr : pTextureTemp->GetD3DTexture() );
	//			m_pDevice->SetSamplerState( TexDesc.RegisterIndex, pSampler->State );
	//		}
	//	}
	//}
	////
	//// ���� Shader �� bool��������
	//auto& ShaderVars_Bools = m_pShaderVars->GetBools();
	//if ( ShaderVars_Bools.empty() == false )
	//{
	//	D3DXHANDLE pHandle = nullptr;
	//	UINT nCount;
	//	D3DXCONSTANT_DESC TexDesc;
	//	for ( size_t i = 0, s = ShaderVars_Bools.size(); i < s; i++ )
	//	{
	//		pHandle = pD3DConstTablePixel->GetConstantByName( 0, ShaderVars_Bools[ i ].strName.c_str() );
	//		if ( pHandle == nullptr )
	//			continue;

	//		pD3DConstTablePixel->GetConstantDesc( pHandle, &TexDesc, &nCount );
	//		if ( TexDesc.Type == D3DXPT_BOOL )
	//		{
	//			pD3DConstTablePixel->SetBool( m_pDevice->GetD3DDevice(), pHandle, ShaderVars_Bools[ i ].Value == false ? FALSE : TRUE );
	//		}
	//	}
	//}

/*	m_pDevice->GetD3DDevice()->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_GOURAUD );
	m_pDevice->GetD3DDevice()->SetRenderState( D3DRS_LIGHTING, FALSE );
	m_pDevice->GetD3DDevice()->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE )*/;

	//pd3dDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );;

	//pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE ); //������Ȳ���
	pd3dDevice->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, FALSE );

	//pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	//pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	//pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	//pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
	
	//pd3dDevice->SetRenderState( D3DRS_SEPARATEALPHABLENDENABLE, TRUE );
	//pd3dDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
	//pd3dDevice->SetRenderState( D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_RED );
	pd3dDevice->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_FLAT ); //����ƽ����ɫģʽ
	//pd3dDevice->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_GOURAUD );
	pd3dDevice->SetRenderState( D3DRS_FOGENABLE, FALSE );
	pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
	pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
	pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );

	//pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG2 );
	//pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	//pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );
	//pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE );
	//pd3dDevice->SetTextureStageState( 0, D3DTSS_RESULTARG, D3DTA_CURRENT );
	//pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE );
	//pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE );

	//pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	//pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	//pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

	//pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
	//pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	//pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
}


void iberbar::RHI::D3D9::CCommandContext::PrepareShaderVariableTable( EShaderType eShaderType, CShaderVariableTable* pShaderVariableTable )
{
	if ( pShaderVariableTable == nullptr )
	{
		return;
	}

	IDirect3DDevice9* pD3DDevice = m_pDevice->GetD3DDevice();

	if ( eShaderType == EShaderType::PixelShader )
	{
		// ���Sampler
		auto& SamplerStates = pShaderVariableTable->GetSamplerStates();
		CSamplerState* pSamplerStateTemp = nullptr;
		for ( size_t i = 0, s = SamplerStates.size(); i < s; i++ )
		{
			//pTextureTemp = (CTexture*)(RHI::ITexture*)Samplers[ i ].pTexture;
			//m_pDevice->SetTexture( (uint32)i, pTextureTemp == nullptr ? nullptr : pTextureTemp->GetD3DTexture() );
			//if ( pTextureTemp != nullptr )
			//{
			//	m_pDevice->SetSamplerState( (uint32)i, Samplers[ i ].pSamplerState->GetSamplerDesc() );
			//}
			pSamplerStateTemp = SamplerStates[ i ];
			if ( pSamplerStateTemp == nullptr )
			{
				m_pDevice->SetSamplerState( (uint32)i, UTextureSamplerState::s_Default );
			}
			else
			{
				m_pDevice->SetSamplerState( (uint32)i, pSamplerStateTemp->GetSamplerDesc() );
			}
		}

		auto& Textures = pShaderVariableTable->GetTextures();
		CTexture* pTextureTemp = nullptr;
		for ( size_t i = 0, s = SamplerStates.size(); i < s; i++ )
		{
			pTextureTemp = Textures[ i ];
			m_pDevice->SetTexture( (uint32)i, pTextureTemp == nullptr ? nullptr : pTextureTemp->GetD3DTexture() );
		}
	}

	const uint8* pVarsBuffer = pShaderVariableTable->GetBuffer();
	const CShaderReflection* pReflection = pShaderVariableTable->GetShaderReflection();
	const CShaderReflectionBuffer* pReflectionBuffer = pReflection->GetBuffer();
	const CShaderReflectionVariable* pReflectionVar = nullptr;
	int nVarCount = pReflectionBuffer->GetVariableCountInternal();
	ID3DXConstantTable* pD3DConstTable = pShaderVariableTable->GetShaderInternal()->GetD3DConstTable();
	for ( int i = 0, s = nVarCount; i < s; i++ )
	{
		pReflectionVar = pReflectionBuffer->GetVariableByIndexInternal( i );
		pD3DConstTable->SetValue(
			pD3DDevice,
			pReflectionVar->GetD3DHandle(),
			pVarsBuffer + pReflectionVar->GetOffset(),
			pReflectionVar->GetTotalSize() );
	}

	float viewport[ 2 ] = { (float)m_pDevice->GetContextSize().w, (float)m_pDevice->GetContextSize().h };
	pReflectionVar = pReflectionBuffer->GetVariableByNameInternal( "g_viewport" );
	if ( pReflectionVar )
	{
		pD3DConstTable->SetValue(
			pD3DDevice,
			pReflectionVar->GetD3DHandle(),
			viewport,
			sizeof( float ) * 2 );
	}

}


void iberbar::RHI::D3D9::CCommandContext::PrepareShaderVariables()
{
	// �����ͨ����
	IDirect3DDevice9* pD3DDevice = m_pDevice->GetD3DDevice();
	PrepareShaderVariableTable( EShaderType::VertexShader, m_pShaderVariableTables[ (int)EShaderType::VertexShader ] );
	PrepareShaderVariableTable( EShaderType::PixelShader, m_pShaderVariableTables[ (int)EShaderType::PixelShader ] );
}


void iberbar::RHI::D3D9::CCommandContext::PrepareBlendState()
{
	//if ( m_pBlendState == nullptr )
	//{
	//	SetBlendState( m_pBlendStateDefault );
	//}
	assert( m_pBlendState );

	IDirect3DDevice9* pD3DDevice = m_pDevice->GetD3DDevice();
	const UBlendDesc& BlendDesc = m_pBlendState->GetDesc();

	BOOL AlphaTest = BlendDesc.RenderTargets[ 0 ].BlendEnable == true ? TRUE : FALSE;
	pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, AlphaTest );

	pD3DDevice->SetRenderState( D3DRS_SRCBLEND, ConvertBlend( BlendDesc.RenderTargets[ 0 ].SrcBlend ) );
	pD3DDevice->SetRenderState( D3DRS_DESTBLEND, ConvertBlend( BlendDesc.RenderTargets[ 0 ].DestBlend ) );
	pD3DDevice->SetRenderState( D3DRS_ALPHATESTENABLE, AlphaTest );
	pD3DDevice->SetRenderState( D3DRS_BLENDOP, ConvertBlendOP( BlendDesc.RenderTargets[ 0 ].BlendOp ) );

	pD3DDevice->SetRenderState( D3DRS_SRCBLENDALPHA, ConvertBlend( BlendDesc.RenderTargets[ 0 ].SrcBlendAlpha ) );
	pD3DDevice->SetRenderState( D3DRS_DESTBLENDALPHA, ConvertBlend( BlendDesc.RenderTargets[ 0 ].DestBlendAlpha ) );
	pD3DDevice->SetRenderState( D3DRS_BLENDOPALPHA, ConvertBlendOP( BlendDesc.RenderTargets[ 0 ].BlendOpAlpha ) );

	pD3DDevice->SetRenderState( D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_RED );
}


void iberbar::RHI::D3D9::CCommandContext::CleanResources()
{
	UNKNOWN_SAFE_RELEASE_NULL( m_pShaderState );
	for ( int i = 0, s = (int)EShaderType::__Count; i < s; i ++ )
	{
		UNKNOWN_SAFE_RELEASE_NULL( m_pShaderVariableTables[i] );
	}
	UNKNOWN_SAFE_RELEASE_NULL( m_pVertexBuffer );
	UNKNOWN_SAFE_RELEASE_NULL( m_pIndexBuffer );
}