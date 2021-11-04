
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
	, m_pD3DDeviceContext( pDevice->GetD3DDeviceContext() )
	, m_nPrimitiveType( UPrimitiveType::Undefined )
	, m_StateCache( pDevice )
{
	assert( m_pD3DDeviceContext );
	m_pD3DDeviceContext->AddRef();
}


iberbar::RHI::D3D11::CCommandContext::~CCommandContext()
{
	m_pD3DDeviceContext->Release();
	m_pD3DDeviceContext = nullptr;
}



void iberbar::RHI::D3D11::CCommandContext::SetVertexBuffer( uint32 nStreamIndex, IVertexBuffer* pVertexBuffer, uint32 nOffset )
{
	CVertexBuffer* pVertexBufferInternal = (CVertexBuffer*)pVertexBuffer;
	ID3D11Buffer* pD3DVertexBuffer = pVertexBufferInternal == nullptr ? nullptr : pVertexBufferInternal->GetD3DBuffer();
	m_StateCache.SetStreamSource( pD3DVertexBuffer, nStreamIndex, nOffset );
}


void iberbar::RHI::D3D11::CCommandContext::SetIndexBuffer( IIndexBuffer* pIndexBuffer, uint32 nOffset )
{
	CIndexBuffer* pIndexBufferInternal = (CIndexBuffer*)pIndexBuffer;
	DXGI_FORMAT Format = (pIndexBufferInternal->GetStride() == sizeof( uint16 ) ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT);
	m_StateCache.SetIndexBuffer( pIndexBufferInternal->GetD3DBuffer(), Format, nOffset );
}


void iberbar::RHI::D3D11::CCommandContext::SetShaderState( IShaderState* pShaderState )
{
	CShaderState* pShaderStateInternal = (CShaderState*)pShaderState;
	m_StateCache.SetVertexShader( pShaderStateInternal->GetD3DVertexShader() );
	m_StateCache.SetPixelShader( pShaderStateInternal->GetD3DPixelShader() );
	m_StateCache.SetGeometryShader( pShaderStateInternal->GetD3DGeometryShader() );
	m_StateCache.SetDomainShader( pShaderStateInternal->GetD3DDomainShader() );
	m_StateCache.SetHullShader( pShaderStateInternal->GetD3DHullShader() );
	m_StateCache.SetComputeShader( pShaderStateInternal->GetD3DComputeShader() );
	m_StateCache.SetInputLayout( pShaderStateInternal->GetD3DInputLayout() );
	m_StateCache.SetStreamStrides( pShaderStateInternal->GetVertexDeclaration()->GetStrides() );

	//m_StateCache.ClearConstantBuffers( EShaderType::VertexShader );

	//ID3D11DeviceContext* pD3DDeviceContext = m_pDevice->GetD3DDeviceContext();
	//CShader* pShader = nullptr;
	//const CShaderReflection* pReflection = nullptr;

	//pShader = m_pShaderState->GetVertexShader();
	//if ( pShader != nullptr )
	//{
	//	pReflection = pShader->GetReflectionInternal();
	//	for ( int i = 0, s = pReflection->GetBufferCountInternal(); i < s; i++ )
	//	{
	//		pD3DDeviceContext->VSSetConstantBuffers( pReflection->GetBufferByIndexInternal( i )->GetBindPoint(), 1, (pShader->GetConstantBuffers()[i]->GetD3DBufferAddress()) );
	//	}
	//}

	//pShader = m_pShaderState->GetPixelShader();
	//if ( pShader != nullptr )
	//{
	//	pReflection = pShader->GetReflectionInternal();
	//	for ( int i = 0, s = pReflection->GetBufferCountInternal(); i < s; i++ )
	//	{
	//		pD3DDeviceContext->PSSetConstantBuffers( pReflection->GetBufferByIndexInternal( i )->GetBindPoint(), 1, (pShader->GetConstantBuffers()[i]->GetD3DBufferAddress()) );
	//	}
	//}

	//pShader = m_pShaderState->GetGeometryShader();
	//if ( pShader != nullptr )
	//{
	//	pReflection = pShader->GetReflectionInternal();
	//	for ( int i = 0, s = pReflection->GetBufferCountInternal(); i < s; i++ )
	//	{
	//		pD3DDeviceContext->GSSetConstantBuffers( pReflection->GetBufferByIndexInternal( i )->GetBindPoint(), 1, (pShader->GetConstantBuffers()[i]->GetD3DBufferAddress()) );
	//	}
	//}

	//pShader = m_pShaderState->GetDomainShader();
	//if ( pShader != nullptr )
	//{
	//	pReflection = pShader->GetReflectionInternal();
	//	for ( int i = 0, s = pReflection->GetBufferCountInternal(); i < s; i++ )
	//	{
	//		pD3DDeviceContext->DSSetConstantBuffers( pReflection->GetBufferByIndexInternal( i )->GetBindPoint(), 1, (pShader->GetConstantBuffers()[i]->GetD3DBufferAddress()) );
	//	}
	//}

	//pShader = m_pShaderState->GetHullShader();
	//if ( pShader != nullptr )
	//{
	//	pReflection = pShader->GetReflectionInternal();
	//	for ( int i = 0, s = pReflection->GetBufferCountInternal(); i < s; i++ )
	//	{
	//		pD3DDeviceContext->HSSetConstantBuffers( pReflection->GetBufferByIndexInternal( i )->GetBindPoint(), 1, (pShader->GetConstantBuffers()[i]->GetD3DBufferAddress()) );
	//	}
	//}
}


void iberbar::RHI::D3D11::CCommandContext::SetTexture( EShaderType nShaderType, uint32 nTextureIndex, ITexture* pTexture )
{
	CTexture* pTextureInternal = (CTexture*)pTexture;
	ID3D11ShaderResourceView* pD3DShaderResourceView = pTextureInternal == nullptr ? nullptr : pTextureInternal->GetD3DShaderResourceView();
	m_StateCache.SetShaderResourceView( pD3DShaderResourceView, nShaderType, nTextureIndex );
}


void iberbar::RHI::D3D11::CCommandContext::SetSamplerState( EShaderType nShaderType, uint32 nSamplerIndex, ISamplerState* pSamplerState )
{
	CSamplerState* pSamplerStateInternal = (CSamplerState*)pSamplerState;
	ID3D11SamplerState* pD3DSamplerState = pSamplerStateInternal == nullptr ? nullptr : pSamplerStateInternal->GetD3DSamplerState();
	m_StateCache.SetSamplerState( pD3DSamplerState, nShaderType, nSamplerIndex );
}


void iberbar::RHI::D3D11::CCommandContext::SetUniformBuffer( EShaderType nShaderType, uint32 nBufferIndex, IUniformBuffer* pUniformBuffer )
{
	CUniformBuffer* pUniformBufferInternal = (CUniformBuffer*)pUniformBuffer;
	ID3D11Buffer* pD3DBuffer = pUniformBufferInternal == nullptr ? nullptr : pUniformBufferInternal->GetD3DBuffer();
	m_StateCache.SetConstantBuffer( pD3DBuffer, nShaderType, nBufferIndex );
}


//void iberbar::RHI::D3D11::CDevice::SetShaderVariableTable( EShaderType eShaderType, IShaderVariableTable* pShaderVariableTable )
//{
//
//}


//void iberbar::RHI::D3D11::CDevice::SetTexture( uint32 nStage, ID3D11ShaderResourceView* pD3DShaderResourceView )
//{
//	assert( nStage < 8 );
//
//	if ( m_pD3DTextures[ nStage ].Get() == pD3DShaderResourceView )
//		return;
//
//	m_pD3DTextures[ nStage ] = pD3DShaderResourceView;
//	m_pD3DDeviceContext->PSSetShaderResources( nStage, 1, m_pD3DTextures[nStage].GetAddressOf() );
//}
//
//
//void iberbar::RHI::D3D11::CDevice::SetSamplerState( uint32 nStage, ID3D11SamplerState* pD3DSamplerState )
//{
//	assert( nStage < 8 );
//
//	if ( m_pD3DSamplerStates[ nStage ].Get() == pD3DSamplerState )
//		return;
//
//	m_pD3DSamplerStates[ nStage ] = pD3DSamplerState;
//	m_pD3DDeviceContext->PSSetSamplers( nStage, 1, m_pD3DSamplerStates[nStage].GetAddressOf() );
//}


void iberbar::RHI::D3D11::CCommandContext::SetBlendFactor( const CColor4F& Factor )
{
	m_StateCache.SetBlendFactor( Factor, 0xffffffff );
}


void iberbar::RHI::D3D11::CCommandContext::SetBlendState( IBlendState* pBlendState, const CColor4F& Factor )
{
	CBlendState* pBlendStateInternal = (CBlendState*)pBlendState;
	m_StateCache.SetBlendState( pBlendStateInternal->GetD3DBlendState(), Factor, 0xffffffff );
}


void iberbar::RHI::D3D11::CCommandContext::SetDepthStencilState( IDepthStencilState* pDepthStencilState, uint32 nStencilRef )
{
	CDepthStencilState* pDepthStencilStateInternal = (CDepthStencilState*)pDepthStencilState;
	m_StateCache.SetDepthStencilState( pDepthStencilStateInternal->GetD3DDepthStencilState(), nStencilRef );
}


void iberbar::RHI::D3D11::CCommandContext::SetPrimitiveTopology( UPrimitiveType nPrimitiveType )
{
	m_nPrimitiveType = nPrimitiveType;
	m_pD3DDeviceContext->IASetPrimitiveTopology( ConvertPrimitiveType( nPrimitiveType ) );
}


void iberbar::RHI::D3D11::CCommandContext::DrawPrimitive( uint32 nBaseVertexIndex, uint32 nNumPrimitives )
{
	uint32 nVertexCount = GetVertexCountForPrimitiveCount( nNumPrimitives, m_nPrimitiveType );
	if ( nVertexCount == 0 )
		return;
	m_pD3DDeviceContext->Draw( nVertexCount, nBaseVertexIndex );
}


void iberbar::RHI::D3D11::CCommandContext::DrawIndexedPrimitive( uint32 nStartIndex, uint32 nBaseVertexIndex, uint32 nNumPrimitives )
{
	uint32 nIndexCount = GetVertexCountForPrimitiveCount( nNumPrimitives, m_nPrimitiveType );
	if ( nIndexCount == 0 )
		return;
	m_pD3DDeviceContext->DrawIndexed( nIndexCount, nStartIndex, nBaseVertexIndex );
}

//
//void iberbar::RHI::D3D11::CCommandContext::PrepareDraw()
//{
//	ID3D11Device* pD3DDevice = m_pDevice->GetD3DDevice();
//	ID3D11DeviceContext* pD3DDeviceContext = m_pDevice->GetD3DDeviceContext();
//
//	// 顶点声明的启用
//	pD3DDeviceContext->IASetInputLayout( m_pShaderState->GetD3DInputLayout() );
//
//	m_pDevice->SetVertexShader( m_pShaderState->GetVertexShader()->GetD3DShader() );
//	m_pDevice->SetPixelShader( m_pShaderState->GetPixelShader()->GetD3DShader() );
//	m_pDevice->BindVertexBuffer( m_pVertexBuffer->GetD3DBuffer(), m_pShaderState->GetVertexDeclaration()->GetStride() );
//	m_pDevice->BindIndexBuffer( m_pIndexBuffer->GetD3DBuffer() );
//
//	PrepareShaderVariables();
//}
//
//
//void iberbar::RHI::D3D11::CCommandContext::PrepareShaderVariableTable( EShaderType eShaderType, CShaderVariableTable* pShaderVariableTable )
//{
//	if ( pShaderVariableTable == nullptr )
//		return;
//
//	ID3D11DeviceContext* pD3DDeviceContext = m_pDevice->GetD3DDeviceContext();
//
//	if ( eShaderType == EShaderType::PixelShader )
//	{
//		// 填充Sampler
//		auto& Samplers = pShaderVariableTable->GetSamplerStates();
//		auto& Textures = pShaderVariableTable->GetTextures();
//		CTexture* pTextureTemp = nullptr;
//		CSamplerState* pSamplerStateTemp = nullptr;
//		for ( size_t i = 0, s = Textures.size(); i < s; i++ )
//		{
//			pTextureTemp = Textures[ i ];
//			m_pDevice->SetTexture( (uint32)i, pTextureTemp == nullptr ? nullptr : pTextureTemp->GetD3DShaderResourceView() );
//		}
//		for ( size_t i = 0, s = Samplers.size(); i < s; i++ )
//		{
//			pSamplerStateTemp = Samplers[ i ];
//			m_pDevice->SetSamplerState( (uint32)i, pSamplerStateTemp == nullptr ? nullptr : pSamplerStateTemp->GetD3DSamplerState() );
//		}
//	}
//
//	CShader* pShader = pShaderVariableTable->GetShaderInternal();
//	const CShaderReflection* pReflection = pShaderVariableTable->GetShaderReflection();
//	const CShaderReflectionBuffer* pReflectionBuffer = nullptr;
//	int nBufferCount = pReflection->GetBufferCountInternal();
//	auto& Buffers = pShader->GetConstantBuffers();
//	const uint8* pBuffersMemoryTotalOld = pShader->GetConstantBuffersMemory();
//	if (nBufferCount == 0 || Buffers.empty() == true || pBuffersMemoryTotalOld == nullptr)
//		return;
//	const uint8* pBufferMemoryNow = nullptr;
//	CConstantBuffer* pCBufferTemp = nullptr;
//	D3D11_MAPPED_SUBRESOURCE MappedData;
//	HRESULT hResult;
//	for ( int i = 0; i < nBufferCount; i++ )
//	{
//		pReflectionBuffer = pReflection->GetBufferByIndexInternal( i );
//		pCBufferTemp = Buffers[ i ];
//		pBufferMemoryNow = pShaderVariableTable->GetBuffer( i );
//		if ( memcmp( pBuffersMemoryTotalOld + pReflectionBuffer->GetOffset(), pBufferMemoryNow, pReflectionBuffer->GetSize() ) == 0 )
//			continue;
//		if ( pCBufferTemp->IsDynamic() == true )
//		{
//			hResult = pD3DDeviceContext->Map( pCBufferTemp->GetD3DBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedData );
//			if ( SUCCEEDED( hResult ) )
//			{
//				memcpy_s( MappedData.pData, pReflectionBuffer->GetSize(), pBufferMemoryNow, pReflectionBuffer->GetSize() );
//				pD3DDeviceContext->Unmap( pCBufferTemp->GetD3DBuffer(), 0 );
//			}
//
//		}
//		else
//		{
//			pD3DDeviceContext->UpdateSubresource( pCBufferTemp->GetD3DBuffer(), 0, nullptr, pBufferMemoryNow, 0, 0 );
//		}
//
//		switch (eShaderType)
//		{
//		case EShaderType::VertexShader:
//			pD3DDeviceContext->VSSetConstantBuffers(pReflection->GetBufferByIndexInternal(i)->GetBindPoint(), 1, (pShader->GetConstantBuffers()[i]->GetD3DBufferAddress()));
//			break;
//
//		case EShaderType::PixelShader:
//			pD3DDeviceContext->PSSetConstantBuffers(pReflection->GetBufferByIndexInternal(i)->GetBindPoint(), 1, (pShader->GetConstantBuffers()[i]->GetD3DBufferAddress()));
//			break;
//
//		default:
//			break;
//		}
//		
//	}
//
//	//ID3D11DeviceContext* pD3DDeviceContext = m_pDevice->GetD3DDeviceContext();
//	//CShader* pShader = nullptr;
//	//const CShaderReflection* pReflection = nullptr;
//
//	//pShader = m_pShaderState->GetVertexShader();
//	//if (pShader != nullptr)
//	//{
//	//	pReflection = pShader->GetReflectionInternal();
//	//	for (int i = 0, s = pReflection->GetBufferCountInternal(); i < s; i++)
//	//	{
//	//		pD3DDeviceContext->VSSetConstantBuffers(pReflection->GetBufferByIndexInternal(i)->GetBindPoint(), 1, (pShader->GetConstantBuffers()[i]->GetD3DBufferAddress()));
//	//	}
//	//}
//
//	//pShader = m_pShaderState->GetPixelShader();
//	//if (pShader != nullptr)
//	//{
//	//	pReflection = pShader->GetReflectionInternal();
//	//	for (int i = 0, s = pReflection->GetBufferCountInternal(); i < s; i++)
//	//	{
//	//		pD3DDeviceContext->PSSetConstantBuffers(pReflection->GetBufferByIndexInternal(i)->GetBindPoint(), 1, (pShader->GetConstantBuffers()[i]->GetD3DBufferAddress()));
//	//	}
//	//}
//
//}
//
//
//void iberbar::RHI::D3D11::CCommandContext::PrepareShaderVariables()
//{
//	// 填充普通变量
//	PrepareShaderVariableTable( EShaderType::VertexShader, m_pShaderVariableTables[ (int)EShaderType::VertexShader ] );
//	PrepareShaderVariableTable( EShaderType::PixelShader, m_pShaderVariableTables[ (int)EShaderType::PixelShader ] );
//	PrepareShaderVariableTable( EShaderType::GeometryShader, m_pShaderVariableTables[ (int)EShaderType::GeometryShader ] );
//	PrepareShaderVariableTable( EShaderType::HullShader, m_pShaderVariableTables[ (int)EShaderType::HullShader ] );
//	PrepareShaderVariableTable( EShaderType::DomainShader, m_pShaderVariableTables[ (int)EShaderType::DomainShader ] );
//}
//
//
//void iberbar::RHI::D3D11::CCommandContext::PrepareBlendState()
//{
//	assert( m_pBlendState );
//
//
//}
//
//
//void iberbar::RHI::D3D11::CCommandContext::CleanResources()
//{
//	UNKNOWN_SAFE_RELEASE_NULL( m_pShaderState );
//	for ( int i = 0, s = (int)EShaderType::__Count; i < s; i++ )
//	{
//		UNKNOWN_SAFE_RELEASE_NULL( m_pShaderVariableTables[ i ] );
//	}
//	UNKNOWN_SAFE_RELEASE_NULL( m_pVertexBuffer );
//	UNKNOWN_SAFE_RELEASE_NULL( m_pIndexBuffer );
//}

