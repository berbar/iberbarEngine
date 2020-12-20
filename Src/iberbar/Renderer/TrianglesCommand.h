#pragma once


#include <iberbar/Renderer/Headers.h>
#include <iberbar/Renderer/RenderCommand.h>
#include <iberbar/RHI/ShaderVariables.h>

namespace iberbar
{
	namespace RHI
	{
		class ITexture;
		class IShaderVariableTable;
	}

	namespace Renderer
	{
		class __iberbarRendererApi__ CTrianglesCommand
			: public CRenderCommand
		{
		public:
			struct __iberbarRendererApi__ UTriangles
			{
			public:
				typedef const void* PVerticesPointer;
				UTriangles();
				UTriangles(
					const void* vertices,
					const uint16* indices,
					uint32 nVertexSize,
					uint32 vertexCount,
					uint32 indexCount,
					uint32 nTrianglesCount );
			public:
				const void* vertices;
				const uint16* indices;
				uint32 vertexSize;
				uint32 vertexCount;
				uint32 vertexSizeTotal;
				uint32 indexCount;
				uint32 indexSizeTotal;
				uint32 nTrianglesCount;
			};

		public:
			CTrianglesCommand();

		public:
			inline void SetTriangles( const UTriangles& triangles ) { m_Triangles = triangles; }

			inline const UTriangles* GetTriangles() { return &m_Triangles; }

		protected:
			UTriangles m_Triangles;
		};
	}
}



FORCEINLINE iberbar::Renderer::CTrianglesCommand::UTriangles::UTriangles()
	: vertices( nullptr )
	, indices( nullptr )
	, vertexSize( 0 )
	, vertexCount( 0 )
	, vertexSizeTotal( 0 )
	, indexCount( 0 )
	, indexSizeTotal( 0 )
	, nTrianglesCount( 0 )
{
}

FORCEINLINE iberbar::Renderer::CTrianglesCommand::UTriangles::UTriangles(
	const void* vertices,
	const uint16* indices,
	uint32 nVertexSize,
	uint32 vertexCount,
	uint32 indexCount,
	uint32 nTrianglesCount )
	: vertices( vertices )
	, indices( indices )
	, vertexSize( nVertexSize )
	, vertexCount( vertexCount )
	, vertexSizeTotal( nVertexSize * vertexCount )
	, indexCount( indexCount )
	, indexSizeTotal( indexCount * 2 )
	, nTrianglesCount( nTrianglesCount )
{
}
