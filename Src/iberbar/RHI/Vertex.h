#pragma once

#include <iberbar/RHI/Headers.h>
#include <iberbar/Utility/Math/Matrix.h>

namespace iberbar
{
	namespace RHI
	{
		struct UVertex_V3F_C4B
		{
		public:
			UVector3f position;
			CColor4B color;
		};

		struct UVertex_V3F_C4B_T2F
		{
		public:
			UVector3f position;
			//CColor4B color;
			CColor4F color;
			UVector2f texcoord;
		};

		template < typename TVertex, int nVertexCount, int nIndexCount >
		struct TTriangles
		{
		public:
			TTriangles()
			{
				memset( Vertices, 0, sizeof( Vertices ) );
				memset( Indices, 0, sizeof( Indices ) );
			}
			int GetVertexCount() const { return nVertexCount; }
			int GetIndexCount() const { return nIndexCount; }
			TVertex Vertices[ nVertexCount ];
			uint16 Indices[ nIndexCount ];
		};


		template struct __iberbarRHIApi__ TTriangles<UVertex_V3F_C4B, 4, 6>;
		template struct __iberbarRHIApi__ TTriangles<UVertex_V3F_C4B_T2F, 4, 6>;
	}
}
