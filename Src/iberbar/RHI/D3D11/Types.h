#pragma once

#include <iberbar/RHI/D3D11/Headers.h>
#include <iberbar/RHI/Types.h>

namespace iberbar
{
	namespace RHI
	{
		namespace D3D11
		{
			D3D11_PRIMITIVE_TOPOLOGY ConvertPrimitiveType( UPrimitiveType nPrimitiveType );
			DXGI_FORMAT ConvertVertexFormat( UVertexFormat nVertexFormat );
			LPCSTR ConvertVertexDeclareUsage( UVertexDeclareUsage usage );
			D3D11_BLEND ConvertBlend( EBlend nBlend );
			D3D11_BLEND_OP ConvertBlendOP( EBlendOP nBlendOP );
			D3D11_TEXTURE_ADDRESS_MODE ConvertTextureAddressMode( UTextureAddress nAddress );
			D3D11_FILTER_TYPE ConvertTextureFilter( UTextureFilterType nFilter );
			D3D11_FILTER EncodeTextureFilter( UTextureFilterType nMin, UTextureFilterType nMag, UTextureFilterType nMip );
			static const int MAX_VERTEX_BUFFERS_COUNT = 16;
		}
	}
}


inline D3D11_PRIMITIVE_TOPOLOGY iberbar::RHI::D3D11::ConvertPrimitiveType( UPrimitiveType nPrimitiveType )
{
	switch ( nPrimitiveType )
	{
		case UPrimitiveType::Point:
			return D3D_PRIMITIVE_TOPOLOGY_POINTLIST;

		case UPrimitiveType::Line:
			return D3D_PRIMITIVE_TOPOLOGY_LINELIST;

		case UPrimitiveType::LineStrip:
			return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;

		case UPrimitiveType::Triangle:
			return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		case UPrimitiveType::TriangleStrip:
			return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

		default:
			break;
	}

	return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
}


inline DXGI_FORMAT iberbar::RHI::D3D11::ConvertVertexFormat( UVertexFormat nVertexFormat )
{
	switch ( nVertexFormat )
	{
		case UVertexFormat::FLOAT4:
			return DXGI_FORMAT_R32G32B32A32_FLOAT;

		case UVertexFormat::FLOAT3:
			return DXGI_FORMAT_R32G32B32_FLOAT;

		case UVertexFormat::FLOAT2:
			return DXGI_FORMAT_R32G32_FLOAT;

		case UVertexFormat::FLOAT:
			return DXGI_FORMAT_R32_FLOAT;

		case UVertexFormat::COLOR:
			return DXGI_FORMAT_R32_UINT;

		default:
			break;
	}
	return DXGI_FORMAT_UNKNOWN;
}


inline LPCSTR iberbar::RHI::D3D11::ConvertVertexDeclareUsage( UVertexDeclareUsage usage )
{
	// https://docs.microsoft.com/en-us/windows/win32/direct3dhlsl/dx-graphics-hlsl-semantics
	switch ( usage )
	{
		case UVertexDeclareUsage::Position:
			return "POSITION";

		case UVertexDeclareUsage::Normal:
			return "NORMAL";

		case UVertexDeclareUsage::Color:
			return "COLOR";

		case UVertexDeclareUsage::TexCoord:
			return "TEXCOORD";

		default:
			break;
	}

	return nullptr;
}



inline D3D11_BLEND iberbar::RHI::D3D11::ConvertBlend( EBlend nBlend )
{
	switch ( nBlend )
	{
		case EBlend::Zero:
			return D3D11_BLEND_ZERO;

		case EBlend::One:
			return D3D11_BLEND_ONE;

		case EBlend::SrcColor:
			return D3D11_BLEND_SRC_COLOR;

		case EBlend::InvSrcColor:
			return D3D11_BLEND_INV_SRC_COLOR;

		case EBlend::SrcAlpha:
			return D3D11_BLEND_SRC_ALPHA;

		case EBlend::InvSrcAlpha:
			return D3D11_BLEND_INV_SRC_ALPHA;

		case EBlend::DestAlpha:
			return D3D11_BLEND_DEST_ALPHA;

		case EBlend::InvDestAlpha:
			return D3D11_BLEND_INV_DEST_ALPHA;

		case EBlend::DestColor:
			return D3D11_BLEND_DEST_COLOR;

		case EBlend::InvDestColor:
			return D3D11_BLEND_INV_DEST_COLOR;

		case EBlend::SrcAlphaSat:
			return D3D11_BLEND_SRC_ALPHA_SAT;

		case EBlend::BlendFactor:
			return D3D11_BLEND_BLEND_FACTOR;

		case EBlend::InvBlendFactor:
			return D3D11_BLEND_INV_BLEND_FACTOR;

		case EBlend::Src1Color:
			return D3D11_BLEND_SRC1_COLOR;

		case EBlend::InvSrc1Color:
			return D3D11_BLEND_INV_SRC1_COLOR;

		default:
			break;
	}

	return D3D11_BLEND_ZERO;
}


inline D3D11_BLEND_OP iberbar::RHI::D3D11::ConvertBlendOP( EBlendOP nBlendOP )
{
	switch ( nBlendOP )
	{
		case EBlendOP::Add:
			return D3D11_BLEND_OP_ADD;

		case EBlendOP::Subtract:
			return D3D11_BLEND_OP_SUBTRACT;

		case EBlendOP::RevSubtract:
			return D3D11_BLEND_OP_REV_SUBTRACT;

		case EBlendOP::Min:
			return D3D11_BLEND_OP_MIN;

		case EBlendOP::Max:
			return D3D11_BLEND_OP_MAX;

		default:
			break;
	}

	return D3D11_BLEND_OP_ADD;
}


inline D3D11_TEXTURE_ADDRESS_MODE iberbar::RHI::D3D11::ConvertTextureAddressMode( UTextureAddress nAddress )
{
	switch ( nAddress )
	{
		case UTextureAddress::Wrap:
			return D3D11_TEXTURE_ADDRESS_WRAP;

		case UTextureAddress::Mirror:
			return D3D11_TEXTURE_ADDRESS_MIRROR;

		case UTextureAddress::Clamp:
			return D3D11_TEXTURE_ADDRESS_CLAMP;

		case UTextureAddress::Border:
			return D3D11_TEXTURE_ADDRESS_BORDER;

		case UTextureAddress::MirrorOnce:
			return D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;

		default:
			break;
	}

	return D3D11_TEXTURE_ADDRESS_WRAP;
}


inline D3D11_FILTER_TYPE iberbar::RHI::D3D11::ConvertTextureFilter( UTextureFilterType nFilter )
{
	switch ( nFilter )
	{
		case UTextureFilterType::Linear:
			return D3D11_FILTER_TYPE_LINEAR;

		case UTextureFilterType::Point:
			return D3D11_FILTER_TYPE_POINT;

		default:
			break;
	}
	return D3D11_FILTER_TYPE_LINEAR;
}


inline D3D11_FILTER iberbar::RHI::D3D11::EncodeTextureFilter( UTextureFilterType nMin, UTextureFilterType nMag, UTextureFilterType nMip )
{
	return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
}