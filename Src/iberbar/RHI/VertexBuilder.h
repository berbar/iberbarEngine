#pragma once


#include <iberbar/RHI/Vertex.h>


namespace iberbar
{
	namespace RHI
	{
		//void RectBuildVertices(
		//	UVertex_V3F_C4B* pVertices,
		//	uint16* pIndices,
		//	float nZindex,
		//	const CRect2f& rectDst,
		//	const CColor4F& color );

		void RectBuildVertices(
			UVertex_V3F_C4B* pVertices,
			float nZindex,
			const CRect2f& rectDst,
			const CColor4B& color );

		void RectBuildVertices(
			UVertex_V3F_C4B_T2F* pVertices,
			float nZindex,
			const CRect2f& rectDst );

		void RectBuildVertices_TexCoord(
			UVertex_V3F_C4B_T2F* pVertices,
			const CRect2f& rectTexCoord );

		void RectBuildVertices(
			UVertex_V3F_C4B_T2F* pVertices,
			float nZindex,
			const CRect2f& rectDst,
			const CColor4B& color,
			const CRect2f& rectCoord );

		static uint16 uRectVertexIndices[] = { 0, 1, 2, 1, 3, 2 };

		void BuildRectVertexIndices( uint16* pIndices, int nRectOffset );
	}
}


//inline void iberbar::RHI::RectBuildVertices(
//	UVertex_V3F_C4B* pVertices,
//	uint16* pIndices,
//	float nZindex,
//	const CRect2f& rectDst,
//	const CColor4F& color )
//{
//	pVertices[ 0 ].position = UVector3f( rectDst.l, rectDst.t, nZindex );
//	pVertices[ 0 ].color = color;
//	pVertices[ 1 ].position = UVector3f( rectDst.r, rectDst.t, nZindex );
//	pVertices[ 1 ].color = color;
//	pVertices[ 2 ].position = UVector3f( rectDst.l, rectDst.b, nZindex );
//	pVertices[ 2 ].color = color;
//	pVertices[ 3 ].position = UVector3f( rectDst.r, rectDst.b, nZindex );
//	pVertices[ 3 ].color = color;
//	pIndices[ 0 ] = 0;
//	pIndices[ 1 ] = 1;
//	pIndices[ 2 ] = 2;
//	pIndices[ 3 ] = 1;
//	pIndices[ 4 ] = 2;
//	pIndices[ 5 ] = 3;
//}

inline void iberbar::RHI::RectBuildVertices(
	UVertex_V3F_C4B* pVertices,
	float nZindex,
	const CRect2f& rectDst,
	const CColor4B& color )
{
	pVertices[ 0 ].position = UVector3f( rectDst.l, rectDst.t, nZindex );
	pVertices[ 0 ].color = color;

	pVertices[ 1 ].position = UVector3f( rectDst.r, rectDst.t, nZindex );
	pVertices[ 1 ].color = color;

	pVertices[ 2 ].position = UVector3f( rectDst.l, rectDst.b, nZindex );
	pVertices[ 2 ].color = color;

	pVertices[ 3 ].position = UVector3f( rectDst.r, rectDst.b, nZindex );
	pVertices[ 3 ].color = color;
}

inline void iberbar::RHI::RectBuildVertices(
	UVertex_V3F_C4B_T2F* pVertices,
	float nZindex,
	const CRect2f& rectDst )
{
	pVertices[ 0 ].position = UVector3f( rectDst.l, rectDst.t, nZindex );
	pVertices[ 1 ].position = UVector3f( rectDst.r, rectDst.t, nZindex );
	pVertices[ 2 ].position = UVector3f( rectDst.l, rectDst.b, nZindex );
	pVertices[ 3 ].position = UVector3f( rectDst.r, rectDst.b, nZindex );
}

inline void iberbar::RHI::RectBuildVertices_TexCoord(
	UVertex_V3F_C4B_T2F* pVertices,
	const CRect2f& rectTexCoord )
{
	pVertices[ 0 ].texcoord = UVector2f( rectTexCoord.l, rectTexCoord.t );
	pVertices[ 1 ].texcoord = UVector2f( rectTexCoord.r, rectTexCoord.t );
	pVertices[ 2 ].texcoord = UVector2f( rectTexCoord.l, rectTexCoord.b );
	pVertices[ 3 ].texcoord = UVector2f( rectTexCoord.r, rectTexCoord.b );
}

inline void iberbar::RHI::RectBuildVertices(
	UVertex_V3F_C4B_T2F* pVertices,
	float nZindex,
	const CRect2f& rectDst,
	const CColor4B& color,
	const CRect2f& rectCoord )
{
	pVertices[ 0 ].position = UVector3f( rectDst.l, rectDst.t, nZindex );
	pVertices[ 0 ].color = color;
	pVertices[ 0 ].texcoord = UVector2f( rectCoord.l, rectCoord.t );

	pVertices[ 1 ].position = UVector3f( rectDst.r, rectDst.t, nZindex );
	pVertices[ 1 ].color = color;
	pVertices[ 1 ].texcoord = UVector2f( rectCoord.r, rectCoord.t );

	pVertices[ 2 ].position = UVector3f( rectDst.l, rectDst.b, nZindex );
	pVertices[ 2 ].color = color;
	pVertices[ 2 ].texcoord = UVector2f( rectCoord.l, rectCoord.b );

	pVertices[ 3 ].position = UVector3f( rectDst.r, rectDst.b, nZindex );
	pVertices[ 3 ].color = color;
	pVertices[ 3 ].texcoord = UVector2f( rectCoord.r, rectCoord.b );
}

inline void iberbar::RHI::BuildRectVertexIndices( uint16* pIndices, int nRectOffset )
{
	//{ 0, 1, 2, 1, 2, 3 };
	uint16 nStart = (uint16)nRectOffset << 2;
	pIndices[ 0 ] = nStart;
	pIndices[ 1 ] = nStart + 1;
	pIndices[ 2 ] = nStart + 2;
	pIndices[ 3 ] = nStart + 1;
	pIndices[ 4 ] = nStart + 3;
	pIndices[ 5 ] = nStart + 2;
}
