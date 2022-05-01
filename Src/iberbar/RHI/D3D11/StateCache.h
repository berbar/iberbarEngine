#pragma once

#include <iberbar/RHI/D3D11/Headers.h>
#include <iberbar/RHI/Types.h>

namespace iberbar
{
	namespace RHI
	{
		namespace D3D11
		{
			class CDevice;

			class CStateCache
			{
			public:
				CStateCache( CDevice* pDevice );
				~CStateCache();

				void SetBlendFactor( const float Factor[4], uint32 SamplerMask );
				void SetBlendState( ID3D11BlendState* pD3DBlendState, const float Factor[4], uint32 SamplerMask );

				void SetDepthStencilState( ID3D11DepthStencilState* pD3DDepthStencilState, uint32 Ref );

				void SetVertexShader( ID3D11VertexShader* pShader );
				void SetPixelShader( ID3D11PixelShader* pShader );
				void SetGeometryShader( ID3D11GeometryShader* pShader );
				void SetDomainShader( ID3D11DomainShader* pShader );
				void SetHullShader( ID3D11HullShader* pShader );
				void SetComputeShader( ID3D11ComputeShader* pShader );

				void SetConstantBuffer( ID3D11Buffer* pConstantBuffer, EShaderType nShaderType, uint32 nSlotIndex );
				void ClearConstantBuffers( EShaderType nShaderType );

				void SetInputLayout( ID3D11InputLayout* pInputLayout );
				void SetStreamStrides( const uint32 nStrides[ MaxVertexElementCount ] );
				void SetStreamSource( ID3D11Buffer* pVertexBuffer, uint32 nStreamIndex, uint32 nOffset );
				void SetStreamSource( ID3D11Buffer* pVertexBuffer, uint32 nStreamIndex, uint32 nStride, uint32 nOffset );
				void SetIndexBuffer( ID3D11Buffer* pIndexBuffer, DXGI_FORMAT Format, uint32 Offset );

				void SetShaderResourceView( ID3D11ShaderResourceView* pD3DSRV, EShaderType nShaderType, uint32 nResourceIndex );
				void SetSamplerState( ID3D11SamplerState* pD3DSamplerState, EShaderType nShaderType, uint32 nSamplerIndex );

			protected:
				void InternalSetSetConstantBuffer( EShaderType nShaderType, uint32 SlotIndex, ID3D11Buffer*& ConstantBuffer );

			protected:
				ComPtr<ID3D11DeviceContext> m_pD3DDeviceContext;

				float m_BlendFactor[4];
				uint32 m_BlendSampleMask;
				ComPtr<ID3D11BlendState> m_pD3DBlendState;

				ID3D11DepthStencilState* m_pD3DDepthStencilState;
				UINT m_nDepthStencilRef;

				ID3D11ShaderResourceView* m_D3DShaderResourceViews[ (int)EShaderType::__Count ][ D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT ];
				ID3D11SamplerState* m_D3DSamplerStates[ (int)EShaderType::__Count ][ D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT ];

				ID3D11InputLayout* m_pD3DInputLayout;

				ID3D11VertexShader* m_pD3DVertexShader;
				ID3D11PixelShader* m_pD3DPixelShader;
				ID3D11GeometryShader* m_pD3DGeometryShader;
				ID3D11DomainShader* m_pD3DDomainShader;
				ID3D11HullShader* m_pD3DHullShader;
				ID3D11ComputeShader* m_pD3DComputeShader;

				UINT m_StreamStrides[ MaxVertexElementCount ];

				// Vertex Buffer State
				struct UVertexBufferState
				{
					ID3D11Buffer* pD3DVertexBuffer;
					UINT nStride;
					UINT nOffset;
				} m_VertexBufferStates[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT];

				// Index Buffer State
				ID3D11Buffer* m_pD3DIndexBuffer;
				DXGI_FORMAT m_nIndexFormat;
				UINT m_nIndexOffset;

				// Constant Buffer State
				struct UConstantBufferState
				{
					ID3D11Buffer* Buffer;
					uint32 FirstConstant;
					uint32 NumConstants;
				} m_ConstantBufferStates[ (int)EShaderType::__Count ][ D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT ];
			};
		}
	}
}


FORCEINLINE void iberbar::RHI::D3D11::CStateCache::SetBlendFactor( const float Factor[4], uint32 SamplerMask )
{
	if ( memcmp( m_BlendFactor, Factor, sizeof( m_BlendFactor ) ) == 0 && m_BlendSampleMask == SamplerMask )
		return;
	memcpy_s( m_BlendFactor, sizeof( m_BlendFactor ), Factor, sizeof( m_BlendFactor ) );
	m_BlendSampleMask = SamplerMask;
	m_pD3DDeviceContext->OMSetBlendState( m_pD3DBlendState.Get(), m_BlendFactor, m_BlendSampleMask );
}


FORCEINLINE void iberbar::RHI::D3D11::CStateCache::SetBlendState( ID3D11BlendState* pD3DBlendState, const float Factor[4], uint32 SamplerMask )
{
	if ( m_pD3DBlendState.Get() == pD3DBlendState && memcmp( m_BlendFactor, Factor, sizeof( m_BlendFactor ) ) == 0 && m_BlendSampleMask == SamplerMask )
		return;
	m_pD3DBlendState = pD3DBlendState;
	memcpy_s( m_BlendFactor, sizeof( m_BlendFactor ), Factor, sizeof( m_BlendFactor ) );
	m_BlendSampleMask = SamplerMask;
	m_pD3DDeviceContext->OMSetBlendState( m_pD3DBlendState.Get(), m_BlendFactor, m_BlendSampleMask );
}


FORCEINLINE void iberbar::RHI::D3D11::CStateCache::SetDepthStencilState( ID3D11DepthStencilState* pD3DDepthStencilState, uint32 nStencilRef )
{
	if ( m_pD3DDepthStencilState != pD3DDepthStencilState || m_nDepthStencilRef != nStencilRef )
	{
		m_pD3DDepthStencilState = pD3DDepthStencilState;
		m_nDepthStencilRef = nStencilRef;
		m_pD3DDeviceContext->OMSetDepthStencilState( m_pD3DDepthStencilState, m_nDepthStencilRef );
	}
}


FORCEINLINE void iberbar::RHI::D3D11::CStateCache::SetVertexShader( ID3D11VertexShader* pShader )
{
	if ( m_pD3DVertexShader != pShader )
	{
		m_pD3DVertexShader = pShader;
		m_pD3DDeviceContext->VSSetShader( m_pD3DVertexShader, nullptr, 0 );
	}
}


FORCEINLINE void iberbar::RHI::D3D11::CStateCache::SetPixelShader( ID3D11PixelShader* pShader )
{
	if ( m_pD3DPixelShader != pShader )
	{
		m_pD3DPixelShader = pShader;
		m_pD3DDeviceContext->PSSetShader( m_pD3DPixelShader, nullptr, 0 );
	}
}


FORCEINLINE void iberbar::RHI::D3D11::CStateCache::SetGeometryShader( ID3D11GeometryShader* pShader )
{
	if ( m_pD3DGeometryShader != pShader )
	{
		m_pD3DGeometryShader = pShader;
		m_pD3DDeviceContext->GSSetShader( m_pD3DGeometryShader, nullptr, 0 );
	}
}


FORCEINLINE void iberbar::RHI::D3D11::CStateCache::SetDomainShader( ID3D11DomainShader* pShader )
{
	if ( m_pD3DDomainShader != pShader )
	{
		m_pD3DDomainShader = pShader;
		m_pD3DDeviceContext->DSSetShader( m_pD3DDomainShader, nullptr, 0 );
	}
}


FORCEINLINE void iberbar::RHI::D3D11::CStateCache::SetHullShader( ID3D11HullShader* pShader )
{
	if ( m_pD3DHullShader != pShader )
	{
		m_pD3DHullShader = pShader;
		m_pD3DDeviceContext->HSSetShader( m_pD3DHullShader, nullptr, 0 );
	}
}


FORCEINLINE void iberbar::RHI::D3D11::CStateCache::SetComputeShader( ID3D11ComputeShader* pShader )
{
	if ( m_pD3DComputeShader != pShader )
	{
		m_pD3DComputeShader = pShader;
		m_pD3DDeviceContext->CSSetShader( m_pD3DComputeShader, nullptr, 0 );
	}
}


FORCEINLINE void iberbar::RHI::D3D11::CStateCache::SetConstantBuffer( ID3D11Buffer* pConstantBuffer, EShaderType nShaderType, uint32 nSlotIndex )
{
	UConstantBufferState& Slot = m_ConstantBufferStates[ (int)nShaderType ][ nSlotIndex ];
	if ( Slot.Buffer != pConstantBuffer )
	{
		Slot.Buffer = pConstantBuffer;
		Slot.FirstConstant = 0;
		Slot.NumConstants = D3D11_REQ_CONSTANT_BUFFER_ELEMENT_COUNT;
		InternalSetSetConstantBuffer( nShaderType, nSlotIndex, Slot.Buffer );
	}
}


FORCEINLINE void iberbar::RHI::D3D11::CStateCache::ClearConstantBuffers( EShaderType nShaderType )
{
	memset( m_ConstantBufferStates[(int)nShaderType], 0, sizeof( UConstantBufferState ) * D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT );

	ID3D11Buffer* Empty[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT] = { 0 };
	switch ( nShaderType )
	{
	case EShaderType::VertexShader:
		m_pD3DDeviceContext->VSSetConstantBuffers( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, Empty );
		break;

	case EShaderType::HullShader:
		m_pD3DDeviceContext->HSSetConstantBuffers( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, Empty );
		break;

	case EShaderType::DomainShader:
		m_pD3DDeviceContext->DSSetConstantBuffers( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, Empty );
		break;

	case EShaderType::GeometryShader:
		m_pD3DDeviceContext->GSSetConstantBuffers( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, Empty );
		break;

	case EShaderType::PixelShader:
		m_pD3DDeviceContext->PSSetConstantBuffers( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, Empty );
		break;

	case EShaderType::ComputeShader:
		m_pD3DDeviceContext->CSSetConstantBuffers( 0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, Empty );
		break;
	}
}


FORCEINLINE void iberbar::RHI::D3D11::CStateCache::SetInputLayout( ID3D11InputLayout* pInputLayout )
{
	if ( m_pD3DInputLayout != pInputLayout )
	{
		m_pD3DInputLayout = pInputLayout;
		m_pD3DDeviceContext->IASetInputLayout( pInputLayout );
	}
}


FORCEINLINE void iberbar::RHI::D3D11::CStateCache::SetStreamStrides( const uint32 Strides[MaxVertexElementCount] )
{
	memcpy_s( m_StreamStrides, sizeof( m_StreamStrides ), Strides, sizeof( m_StreamStrides ) );
}


FORCEINLINE void iberbar::RHI::D3D11::CStateCache::SetStreamSource( ID3D11Buffer* pVertexBuffer, uint32 nStreamIndex, uint32 nOffset )
{
	SetStreamSource( pVertexBuffer, nStreamIndex, m_StreamStrides[nStreamIndex], nOffset );
}


FORCEINLINE void iberbar::RHI::D3D11::CStateCache::SetStreamSource( ID3D11Buffer* pVertexBuffer, uint32 nStreamIndex, uint32 nStride, uint32 nOffset )
{
	assert( nStride == m_StreamStrides[nStreamIndex] );
	assert( nStreamIndex < D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT );
	UVertexBufferState& Slot = m_VertexBufferStates[nStreamIndex];
	if ( Slot.pD3DVertexBuffer != pVertexBuffer || Slot.nStride != nStride || Slot.nOffset != nOffset )
	{
		Slot.pD3DVertexBuffer = pVertexBuffer;
		Slot.nStride = nStride;
		Slot.nOffset = nOffset;
		m_pD3DDeviceContext->IASetVertexBuffers( nStreamIndex, 1, &Slot.pD3DVertexBuffer, &Slot.nStride, &Slot.nOffset );
	}
}


FORCEINLINE void iberbar::RHI::D3D11::CStateCache::SetIndexBuffer( ID3D11Buffer* pIndexBuffer, DXGI_FORMAT Format, uint32 Offset )
{
	if ( m_pD3DIndexBuffer != pIndexBuffer || m_nIndexFormat != Format || m_nIndexOffset != Offset )
	{
		m_pD3DIndexBuffer = pIndexBuffer;
		m_nIndexFormat = Format;
		m_nIndexOffset = Offset;
		m_pD3DDeviceContext->IASetIndexBuffer( m_pD3DIndexBuffer, m_nIndexFormat, m_nIndexOffset );
	}
}


FORCEINLINE void iberbar::RHI::D3D11::CStateCache::SetShaderResourceView( ID3D11ShaderResourceView* pD3DSRV, EShaderType nShaderType, uint32 nResourceIndex )
{
	assert( nResourceIndex < D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT );
	ID3D11ShaderResourceView* pD3DSRV_Current = m_D3DShaderResourceViews[ (int)nShaderType ][ nResourceIndex ];
	if ( pD3DSRV_Current != pD3DSRV )
	{
		if ( pD3DSRV_Current )
		{
			pD3DSRV_Current->Release();
		}
		m_D3DShaderResourceViews[ (int)nShaderType ][ nResourceIndex ] = pD3DSRV;
		if ( pD3DSRV )
		{
			pD3DSRV->AddRef();
		}

		switch ( nShaderType )
		{
		case EShaderType::VertexShader:
			m_pD3DDeviceContext->VSSetShaderResources( nResourceIndex, 1, &pD3DSRV );
			break;

		case EShaderType::PixelShader:
			m_pD3DDeviceContext->PSSetShaderResources( nResourceIndex, 1, &pD3DSRV );
			break;

		case EShaderType::GeometryShader:
			m_pD3DDeviceContext->GSSetShaderResources( nResourceIndex, 1, &pD3DSRV );
			break;

		case EShaderType::DomainShader:
			m_pD3DDeviceContext->DSSetShaderResources( nResourceIndex, 1, &pD3DSRV );
			break;

		case EShaderType::HullShader:
			m_pD3DDeviceContext->HSSetShaderResources( nResourceIndex, 1, &pD3DSRV );
			break;

		case EShaderType::ComputeShader:
			m_pD3DDeviceContext->CSSetShaderResources( nResourceIndex, 1, &pD3DSRV );
			break;
		}
	}
}


FORCEINLINE void iberbar::RHI::D3D11::CStateCache::SetSamplerState( ID3D11SamplerState* pD3DSamplerState, EShaderType nShaderType, uint32 nSamplerIndex )
{
	assert( nSamplerIndex < D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT );
	if ( m_D3DSamplerStates[ (int)nShaderType ][ nSamplerIndex ] != pD3DSamplerState )
	{
		m_D3DSamplerStates[ (int)nShaderType ][ nSamplerIndex ] = pD3DSamplerState;

		switch ( nShaderType )
		{
		case EShaderType::VertexShader:
			m_pD3DDeviceContext->VSSetSamplers( nSamplerIndex, 1, &pD3DSamplerState );
			break;

		case EShaderType::PixelShader:
			m_pD3DDeviceContext->PSSetSamplers( nSamplerIndex, 1, &pD3DSamplerState );
			break;

		case EShaderType::GeometryShader:
			m_pD3DDeviceContext->GSSetSamplers( nSamplerIndex, 1, &pD3DSamplerState );
			break;

		case EShaderType::DomainShader:
			m_pD3DDeviceContext->DSSetSamplers( nSamplerIndex, 1, &pD3DSamplerState );
			break;

		case EShaderType::HullShader:
			m_pD3DDeviceContext->HSSetSamplers( nSamplerIndex, 1, &pD3DSamplerState );
			break;

		case EShaderType::ComputeShader:
			m_pD3DDeviceContext->CSSetSamplers( nSamplerIndex, 1, &pD3DSamplerState );
			break;
		}
	}
}


FORCEINLINE void iberbar::RHI::D3D11::CStateCache::InternalSetSetConstantBuffer( EShaderType nShaderType, uint32 SlotIndex, ID3D11Buffer*& ConstantBuffer )
{
	switch ( nShaderType )
	{
	case EShaderType::VertexShader:
		m_pD3DDeviceContext->VSSetConstantBuffers( SlotIndex, 1, &ConstantBuffer );
		break;

	case EShaderType::HullShader:
		m_pD3DDeviceContext->HSSetConstantBuffers( SlotIndex, 1, &ConstantBuffer );
		break;

	case EShaderType::DomainShader:
		m_pD3DDeviceContext->DSSetConstantBuffers( SlotIndex, 1, &ConstantBuffer );
		break;

	case EShaderType::GeometryShader:
		m_pD3DDeviceContext->GSSetConstantBuffers( SlotIndex, 1, &ConstantBuffer );
		break;

	case EShaderType::PixelShader:
		m_pD3DDeviceContext->PSSetConstantBuffers( SlotIndex, 1, &ConstantBuffer );
		break;

	case EShaderType::ComputeShader:
		m_pD3DDeviceContext->CSSetConstantBuffers( SlotIndex, 1, &ConstantBuffer );
		break;
	}
}

