#pragma once

#include <iberbar/RHI/Types.h>
#include <iberbar/RHI/D3D9/Headers.h>

namespace iberbar
{
	namespace RHI
	{
		namespace D3D9
		{
			D3DDECLTYPE ConvertVertexFormat( UVertexFormat nVertexFormat );
			D3DDECLUSAGE ConvertVertexDeclareUsage( UVertexDeclareUsage usage );
			D3DPRIMITIVETYPE ConvertPrimitiveType( UPrimitiveType nPrimitiveType );
			D3DTEXTUREFILTERTYPE ConvertTextureFilterType( UTextureFilterType nFilterType );
			D3DTEXTUREADDRESS ConvertTextureAddress( UTextureAddress nAddress );
		}
	}
}



inline D3DDECLTYPE iberbar::RHI::D3D9::ConvertVertexFormat( UVertexFormat nVertexFormat )
{
	switch ( nVertexFormat )
	{
		case UVertexFormat::FLOAT:
			return D3DDECLTYPE_FLOAT1;

		case UVertexFormat::FLOAT2:
			return D3DDECLTYPE_FLOAT2;

		case UVertexFormat::FLOAT3:
			return D3DDECLTYPE_FLOAT3;

		case UVertexFormat::FLOAT4:
			return D3DDECLTYPE_FLOAT4;

			//case UVertexFormat::USHORT2:
			//	return D3DDECLTYPE_SHORT2;

			//case UVertexFormat::USHORT4:
			//	return D3DDECLTYPE_SHORT4;

		case UVertexFormat::COLOR:
			return D3DDECLTYPE_D3DCOLOR;

		default:
			break;
	}
	return D3DDECLTYPE_UNUSED;
}

inline D3DDECLUSAGE iberbar::RHI::D3D9::ConvertVertexDeclareUsage( UVertexDeclareUsage usage )
{
	switch ( usage )
	{
		case UVertexDeclareUsage::Position:
			return D3DDECLUSAGE_POSITION;

		case UVertexDeclareUsage::Normal:
			return D3DDECLUSAGE_NORMAL;

		case UVertexDeclareUsage::Color:
			return D3DDECLUSAGE_COLOR;

		case UVertexDeclareUsage::TexCoord:
			return D3DDECLUSAGE_TEXCOORD;

		case UVertexDeclareUsage::Sample:
			return D3DDECLUSAGE_SAMPLE;

		default:
			break;
	}

	return D3DDECLUSAGE_POSITION;
}

inline D3DPRIMITIVETYPE iberbar::RHI::D3D9::ConvertPrimitiveType( UPrimitiveType nPrimitiveType )
{
	switch ( nPrimitiveType )
	{
		case UPrimitiveType::Point:
			return D3DPRIMITIVETYPE::D3DPT_POINTLIST;

		case UPrimitiveType::Line:
			return D3DPRIMITIVETYPE::D3DPT_LINELIST;

		case UPrimitiveType::LineStrip:
			return D3DPRIMITIVETYPE::D3DPT_LINESTRIP;

		case UPrimitiveType::Triangle:
			return D3DPRIMITIVETYPE::D3DPT_TRIANGLELIST;

		case UPrimitiveType::TriangleStrip:
			return D3DPRIMITIVETYPE::D3DPT_TRIANGLESTRIP;

		default:
			break;
	}

	return D3DPRIMITIVETYPE::D3DPT_TRIANGLESTRIP;
}

inline D3DTEXTUREFILTERTYPE iberbar::RHI::D3D9::ConvertTextureFilterType( UTextureFilterType nFilterType )
{
	switch ( nFilterType )
	{
		case UTextureFilterType::Linear:
			return D3DTEXF_LINEAR;

		case UTextureFilterType::Point:
			return D3DTEXF_POINT;

		case UTextureFilterType::Anisotropic:
			return D3DTEXF_ANISOTROPIC;

		//case UTextureFilterType::PyramidalQuad:
		//	return D3DTEXF_PYRAMIDALQUAD;

		default:break;
	}
	return D3DTEXF_NONE;
}

inline D3DTEXTUREADDRESS iberbar::RHI::D3D9::ConvertTextureAddress( UTextureAddress nAddress )
{
	switch ( nAddress )
	{
		case UTextureAddress::Wrap:
			return D3DTADDRESS_WRAP;

		case UTextureAddress::Mirror:
			return D3DTADDRESS_MIRROR;

		case UTextureAddress::Clamp:
			return D3DTADDRESS_CLAMP;

		case UTextureAddress::Border:
			return D3DTADDRESS_BORDER;

		default:break;
	}
	return D3DTADDRESS_WRAP;
}