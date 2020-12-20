

#include <iberbar/RHI/D3D9/CommandContext.h>
#include <iberbar/RHI/D3D9/Types.h>
#include <iberbar/RHI/D3D9/Device.h>
#include <iberbar/RHI/D3D9/Buffer.h>
#include <iberbar/RHI/D3D9/Shader.h>
#include <iberbar/RHI/D3D9/ShaderState.h>
#include <iberbar/RHI/D3D9/ShaderVariables.h>
#include <iberbar/RHI/D3D9/VertexDeclaration.h>
#include <iberbar/RHI/D3D9/Texture.h>
#include <iberbar/RHI/ShaderVariables.h>



iberbar::RHI::D3D9::CCommandContext::CCommandContext( CDevice* pDevice )
	: ICommandContext()
	, m_pDevice( pDevice )
	, m_pVertexBuffer( nullptr )
	, m_pIndexBuffer( nullptr )
	, m_pShaderState( nullptr )
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


void iberbar::RHI::D3D9::CCommandContext::SetShaderVariableTable( IShaderVariableTable* pShaderVariableTable )
{
	if ( m_pShaderVariableTable != pShaderVariableTable )
	{
		UNKNOWN_SAFE_RELEASE_NULL( m_pShaderVariableTable );
		m_pShaderVariableTable = (CShaderVariableTable*)pShaderVariableTable;
		UNKNOWN_SAFE_ADDREF( m_pShaderVariableTable );
	}
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

	// 顶点声明的启用
	m_pDevice->GetD3DDevice()->SetVertexDeclaration( m_pShaderState->GetVertexDeclaration()->GetD3DVertexDeclaration() );

	CShader* pShader = (CShader*)m_pShaderState->GetShader();
	m_pDevice->SetVertexShader( pShader->GetD3DVertexShader() );
	m_pDevice->SetPixelShader( pShader->GetD3DPixelShader() );
	m_pDevice->BindVertexBuffer( m_pVertexBuffer->GetD3DVertexBuffer(), m_pShaderState->GetVertexDeclaration()->GetStride() );
	m_pDevice->BindIndexBuffer( m_pIndexBuffer->GetD3DIndexBuffer() );

	// 准备shader变量
	PrepareShaderVariables();

	//
	//ID3DXConstantTable* pD3DConstTablePixel = pShader->GetD3DPixelShaderConstTable();
	////
	//// 设置 shader 的 Sampler
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
	//// 设置 Shader 的 bool布尔变量
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

	//pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE ); //启用深度测试
	pd3dDevice->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, FALSE );

	pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
	
	//pd3dDevice->SetRenderState( D3DRS_SEPARATEALPHABLENDENABLE, TRUE );
	pd3dDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
	pd3dDevice->SetRenderState( D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_RED );
	pd3dDevice->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_FLAT ); //设置平面着色模式
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


void iberbar::RHI::D3D9::CCommandContext::PrepareShaderVariables()
{
	// 填充Sampler
	auto& Samplers = m_pShaderVariableTable->GetSamplers();
	CTexture* pTextureTemp = nullptr;
	for ( size_t i = 0, s = Samplers.size(); i < s; i++ )
	{
		pTextureTemp = (CTexture*)(RHI::ITexture*)Samplers[ i ].pTexture;
		m_pDevice->SetTexture( (uint32)i, pTextureTemp == nullptr ? nullptr : pTextureTemp->GetD3DTexture() );
		if ( pTextureTemp != nullptr )
		{
			m_pDevice->SetSamplerState( (uint32)i, Samplers[ i ].State );
		}
	}

	// 填充普通变量
	IDirect3DDevice9* pD3DDevice = m_pDevice->GetD3DDevice();
	const uint8* pVarsBuffer = m_pShaderVariableTable->GetBuffer();
	const auto& VarsList = m_pShaderVariableTable->GetVars();
	const UShaderVariableDeclarationNode* pVarDeclNode = nullptr;
	ID3DXConstantTable* pD3DConstTableArray[ 2 ];
	pD3DConstTableArray[ (int)UShaderType::Vertex ] = m_pShaderState->GetShader()->GetD3DVertexShaderConstTable();
	pD3DConstTableArray[ (int)UShaderType::Pixel ] = m_pShaderState->GetShader()->GetD3DPixelShaderConstTable();
	for ( size_t i = 0, s = VarsList.size(); i < s; i++ )
	{
		pVarDeclNode = VarsList[ i ];
		pD3DConstTableArray[ (int)pVarDeclNode->nShaderType ]->SetValue(
			pD3DDevice,
			pVarDeclNode->D3DHandle,
			pVarsBuffer + pVarDeclNode->nOffset,
			pVarDeclNode->nTotalSize );
	}

	auto pVarDeclNode_Viewport = m_pShaderState->GetShaderVariableDeclaration()->GetNode( UShaderType::Vertex, "g_viewport" );
	if ( pVarDeclNode_Viewport != nullptr )
	{
		float viewport[ 2 ] = { (float)m_pDevice->GetContextSize().w, (float)m_pDevice->GetContextSize().h };
		pD3DConstTableArray[ (int)UShaderType::Vertex ]->SetValue( pD3DDevice, pVarDeclNode_Viewport->D3DHandle, viewport, sizeof( float ) * 2 );
	}
}


void iberbar::RHI::D3D9::CCommandContext::CleanResources()
{
	UNKNOWN_SAFE_RELEASE_NULL( m_pShaderState );
	UNKNOWN_SAFE_RELEASE_NULL( m_pShaderVariableTable );
	UNKNOWN_SAFE_RELEASE_NULL( m_pVertexBuffer );
	UNKNOWN_SAFE_RELEASE_NULL( m_pIndexBuffer );
}