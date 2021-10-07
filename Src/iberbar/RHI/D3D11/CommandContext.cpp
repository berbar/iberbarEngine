
#include <iberbar/RHI/D3D11/CommandContext.h>
#include <iberbar/RHI/D3D11/Device.h>
#include <iberbar/RHI/D3D11/Buffer.h>
#include <iberbar/RHI/D3D11/Shader.h>
#include <iberbar/RHI/D3D11/ShaderState.h>
#include <iberbar/RHI/D3D11/ShaderVariables.h>
#include <iberbar/RHI/D3D11/VertexDeclaration.h>
#include <iberbar/RHI/D3D11/Texture.h>
#include <iberbar/RHI/D3D11/RenderState.h>
#include <iberbar/RHI/D3D11/ShaderReflection.h>
#include <iberbar/RHI/D3D11/Types.h>


iberbar::RHI::D3D11::CCommandContext::CCommandContext( CDevice* pDevice )
	: ICommandContext()
	, m_pDevice( pDevice )
	, m_pVertexBuffer( nullptr )
	, m_pIndexBuffer( nullptr )
	, m_pShaderState( nullptr )
	, m_pBlendState( nullptr )
	, m_pBlendStateDefault( nullptr )
	, m_pTextures()
{
	m_pDevice->AddRef();
	memset( m_pTextures, 0, sizeof( m_pTextures ) );
}


iberbar::RHI::D3D11::CCommandContext::~CCommandContext()
{
	for ( int i = 0; i < 8; i++ )
	{
		UNKNOWN_SAFE_RELEASE_NULL( m_pTextures[ i ] );
	}
	UNKNOWN_SAFE_RELEASE_NULL( m_pVertexBuffer );
	UNKNOWN_SAFE_RELEASE_NULL( m_pIndexBuffer );
	UNKNOWN_SAFE_RELEASE_NULL( m_pBlendState );
	UNKNOWN_SAFE_RELEASE_NULL( m_pBlendStateDefault );
	UNKNOWN_SAFE_RELEASE_NULL( m_pDevice );
}



void iberbar::RHI::D3D11::CCommandContext::SetVertexBuffer( IVertexBuffer* pVertexBuffer )
{
	if ( m_pVertexBuffer != pVertexBuffer )
	{
		UNKNOWN_SAFE_RELEASE_NULL( m_pVertexBuffer );
		m_pVertexBuffer = (CVertexBuffer*)pVertexBuffer;
		UNKNOWN_SAFE_ADDREF( m_pVertexBuffer );
	}
}


void iberbar::RHI::D3D11::CCommandContext::SetIndexBuffer( IIndexBuffer* pIndexBuffer )
{
	if ( m_pIndexBuffer != pIndexBuffer )
	{
		UNKNOWN_SAFE_RELEASE_NULL( m_pIndexBuffer );
		m_pIndexBuffer = (CIndexBuffer*)pIndexBuffer;
		UNKNOWN_SAFE_ADDREF( m_pIndexBuffer );
	}
}


void iberbar::RHI::D3D11::CCommandContext::SetShaderState( IShaderState* pShaderState )
{
	if ( m_pShaderState != pShaderState )
	{
		UNKNOWN_SAFE_RELEASE_NULL( m_pShaderState );
		m_pShaderState = (CShaderState*)pShaderState;
		UNKNOWN_SAFE_ADDREF( m_pShaderState );

		ID3D11DeviceContext* pD3DDeviceContext = m_pDevice->GetD3DDeviceContext();
		CShader* pShader = nullptr;
		const CShaderReflection* pReflection = nullptr;

		pShader = m_pShaderState->GetVertexShader();
		if ( pShader != nullptr )
		{
			pReflection = pShader->GetReflectionInternal();
			for ( int i = 0, s = pReflection->GetBufferCountInternal(); i < s; i++ )
			{
				pD3DDeviceContext->VSSetConstantBuffers( pReflection->GetBufferByIndexInternal(i)->GetBindPoint(), 1, ( pShader->GetConstantBuffers()[ i ]->GetD3DBufferAddress() ) );
			}
		}

		pShader = m_pShaderState->GetPixelShader();
		if ( pShader != nullptr )
		{
			pReflection = pShader->GetReflectionInternal();
			for ( int i = 0, s = pReflection->GetBufferCountInternal(); i < s; i++ )
			{
				pD3DDeviceContext->PSSetConstantBuffers( pReflection->GetBufferByIndexInternal( i )->GetBindPoint(), 1, ( pShader->GetConstantBuffers()[ i ]->GetD3DBufferAddress() ) );
			}
		}

		pShader = m_pShaderState->GetGeometryShader();
		if ( pShader != nullptr )
		{
			pReflection = pShader->GetReflectionInternal();
			for ( int i = 0, s = pReflection->GetBufferCountInternal(); i < s; i++ )
			{
				pD3DDeviceContext->GSSetConstantBuffers( pReflection->GetBufferByIndexInternal( i )->GetBindPoint(), 1, ( pShader->GetConstantBuffers()[ i ]->GetD3DBufferAddress() ) );
			}
		}

		pShader = m_pShaderState->GetDomainShader();
		if ( pShader != nullptr )
		{
			pReflection = pShader->GetReflectionInternal();
			for ( int i = 0, s = pReflection->GetBufferCountInternal(); i < s; i++ )
			{
				pD3DDeviceContext->DSSetConstantBuffers( pReflection->GetBufferByIndexInternal( i )->GetBindPoint(), 1, ( pShader->GetConstantBuffers()[ i ]->GetD3DBufferAddress() ) );
			}
		}

		pShader = m_pShaderState->GetHullShader();
		if ( pShader != nullptr )
		{
			pReflection = pShader->GetReflectionInternal();
			for ( int i = 0, s = pReflection->GetBufferCountInternal(); i < s; i++ )
			{
				pD3DDeviceContext->HSSetConstantBuffers( pReflection->GetBufferByIndexInternal( i )->GetBindPoint(), 1, ( pShader->GetConstantBuffers()[ i ]->GetD3DBufferAddress() ) );
			}
		}
	}
}


void iberbar::RHI::D3D11::CCommandContext::SetShaderVariableTable( EShaderType eShaderType, IShaderVariableTable* pShaderVariableTable )
{
	if ( m_pShaderVariableTables[ (int)eShaderType ] != pShaderVariableTable )
	{
		UNKNOWN_SAFE_RELEASE_NULL( m_pShaderVariableTables[ (int)eShaderType ] );
		m_pShaderVariableTables[ (int)eShaderType ] = (CShaderVariableTable*)pShaderVariableTable;
		UNKNOWN_SAFE_ADDREF( m_pShaderVariableTables[ (int)eShaderType ] );

		//PrepareShaderVariables();
	}
}


void iberbar::RHI::D3D11::CCommandContext::SetBlendState( IBlendState* pBlendState )
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


void iberbar::RHI::D3D11::CCommandContext::SetBlendStateDefault( IBlendState* pBlendState )
{
	assert( pBlendState );

	UNKNOWN_SAFE_RELEASE_NULL( m_pBlendStateDefault );
	m_pBlendStateDefault = (CBlendState*)pBlendState;
	m_pBlendStateDefault->AddRef();
	SetBlendState( m_pBlendStateDefault );
}


void iberbar::RHI::D3D11::CCommandContext::DrawElements( UPrimitiveType nPrimitiveType, UIndexFormat nIndexFormat, uint32 nCount, uint32 nOffset )
{
	ID3D11DeviceContext* pD3DDeviceContext = m_pDevice->GetD3DDeviceContext();
	PrepareDraw();
	uint32 nVertexCount = nCount / 2 * 4;
	//uint32 nVertexCount = nCount * 3;
	pD3DDeviceContext->IASetPrimitiveTopology( ConvertPrimitiveType( nPrimitiveType ) );
	pD3DDeviceContext->DrawIndexed( nCount * 3, 0, 0);

	//pD3DDviceContext->DrawIndexedInstanced();
	//pD3DDevice->DrawIndexedPrimitive( ConvertPrimitiveType( nPrimitiveType ), 0, 0, nVertexCount, 0, nCount );
	CleanResources();
}


void iberbar::RHI::D3D11::CCommandContext::PrepareDraw()
{
	ID3D11Device* pD3DDevice = m_pDevice->GetD3DDevice();
	ID3D11DeviceContext* pD3DDeviceContext = m_pDevice->GetD3DDeviceContext();

	// 顶点声明的启用
	pD3DDeviceContext->IASetInputLayout( m_pShaderState->GetD3DInputLayout() );

	m_pDevice->SetVertexShader( m_pShaderState->GetVertexShader()->GetD3DShader() );
	m_pDevice->SetPixelShader( m_pShaderState->GetPixelShader()->GetD3DShader() );
	m_pDevice->BindVertexBuffer( m_pVertexBuffer->GetD3DBuffer(), m_pShaderState->GetVertexDeclaration()->GetStride() );
	m_pDevice->BindIndexBuffer( m_pIndexBuffer->GetD3DBuffer() );

	PrepareShaderVariables();
}


void iberbar::RHI::D3D11::CCommandContext::PrepareShaderVariableTable( EShaderType eShaderType, CShaderVariableTable* pShaderVariableTable )
{
	if ( pShaderVariableTable == nullptr )
		return;

	ID3D11DeviceContext* pD3DDeviceContext = m_pDevice->GetD3DDeviceContext();

	if ( eShaderType == EShaderType::PixelShader )
	{
		// 填充Sampler
		auto& Samplers = pShaderVariableTable->GetSamplerStates();
		auto& Textures = pShaderVariableTable->GetTextures();
		CTexture* pTextureTemp = nullptr;
		CSamplerState* pSamplerStateTemp = nullptr;
		for ( size_t i = 0, s = Textures.size(); i < s; i++ )
		{
			pTextureTemp = Textures[ i ];
			m_pDevice->SetTexture( (uint32)i, pTextureTemp == nullptr ? nullptr : pTextureTemp->GetD3DShaderResourceView() );
		}
		for ( size_t i = 0, s = Samplers.size(); i < s; i++ )
		{
			pSamplerStateTemp = Samplers[ i ];
			m_pDevice->SetSamplerState( (uint32)i, pSamplerStateTemp == nullptr ? nullptr : pSamplerStateTemp->GetD3DSamplerState() );
		}
	}

	CShader* pShader = pShaderVariableTable->GetShaderInternal();
	const CShaderReflection* pReflection = pShaderVariableTable->GetShaderReflection();
	const CShaderReflectionBuffer* pReflectionBuffer = nullptr;
	int nBufferCount = pReflection->GetBufferCountInternal();
	auto& Buffers = pShader->GetConstantBuffers();
	const uint8* pBuffersMemoryTotalOld = pShader->GetConstantBuffersMemory();
	const uint8* pBufferMemoryNow = nullptr;
	CConstantBuffer* pCBufferTemp = nullptr;
	D3D11_MAPPED_SUBRESOURCE MappedData;
	HRESULT hResult;
	for ( int i = 0; i < nBufferCount; i++ )
	{
		pReflectionBuffer = pReflection->GetBufferByIndexInternal( i );
		pCBufferTemp = Buffers[ i ];
		pBufferMemoryNow = pShaderVariableTable->GetBuffer( i );
		if ( memcmp( pBuffersMemoryTotalOld + pReflectionBuffer->GetOffset(), pBufferMemoryNow, pReflectionBuffer->GetSize() ) == 0 )
			continue;
		if ( pCBufferTemp->IsDynamic() == true )
		{
			hResult = pD3DDeviceContext->Map( pCBufferTemp->GetD3DBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedData );
			if ( SUCCEEDED( hResult ) )
			{
				memcpy_s( MappedData.pData, pReflectionBuffer->GetSize(), pBufferMemoryNow, pReflectionBuffer->GetSize() );
				pD3DDeviceContext->Unmap( pCBufferTemp->GetD3DBuffer(), 0 );
			}

		}
		else
		{
			pD3DDeviceContext->UpdateSubresource( pCBufferTemp->GetD3DBuffer(), 0, nullptr, pBufferMemoryNow, 0, 0 );
		}
	}
}


void iberbar::RHI::D3D11::CCommandContext::PrepareShaderVariables()
{
	// 填充普通变量
	PrepareShaderVariableTable( EShaderType::VertexShader, m_pShaderVariableTables[ (int)EShaderType::VertexShader ] );
	PrepareShaderVariableTable( EShaderType::PixelShader, m_pShaderVariableTables[ (int)EShaderType::PixelShader ] );
	PrepareShaderVariableTable( EShaderType::GeometryShader, m_pShaderVariableTables[ (int)EShaderType::GeometryShader ] );
	PrepareShaderVariableTable( EShaderType::HullShader, m_pShaderVariableTables[ (int)EShaderType::HullShader ] );
	PrepareShaderVariableTable( EShaderType::DomainShader, m_pShaderVariableTables[ (int)EShaderType::DomainShader ] );
}


void iberbar::RHI::D3D11::CCommandContext::PrepareBlendState()
{
	assert( m_pBlendState );

	float BlendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_pDevice->GetD3DDeviceContext()->OMSetBlendState( m_pBlendState->GetD3DBlendState(), BlendFactor, 0xffffffff );
}


void iberbar::RHI::D3D11::CCommandContext::CleanResources()
{
	UNKNOWN_SAFE_RELEASE_NULL( m_pShaderState );
	for ( int i = 0, s = (int)EShaderType::__Count; i < s; i++ )
	{
		UNKNOWN_SAFE_RELEASE_NULL( m_pShaderVariableTables[ i ] );
	}
	UNKNOWN_SAFE_RELEASE_NULL( m_pVertexBuffer );
	UNKNOWN_SAFE_RELEASE_NULL( m_pIndexBuffer );
}

