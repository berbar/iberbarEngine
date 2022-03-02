#pragma once

#include <iberbar/Utility/Color.h>
#include <iberbar/Utility/Math/Matrix.h>

namespace iberbar
{
	namespace Renderer
	{
		struct UVertex_V3F_C4B_T2F
		{
		public:
			UVector3f position;
			CColor4F color;
			UVector2f texcoord;
		};


		void VerticesRectUpdateUV( UVertex_V3F_C4B_T2F pVertices[4], const CRect2f& UV );

		void VerticesRectUpdateUV( UVector2f pVertices[ 4 ], const CRect2f& UV );

		template < typename T >
		void VerticesRectUpdatePosition( UVertex_V3F_C4B_T2F pVertices[ 4 ], const TRect<T>& Bounding, float nZPosition );

		template < typename T >
		void VerticesRectUpdatePosition( UVector3f pVertices[ 4 ], const TRect<T>& Bounding, float nZPosition );
	}
}


inline void iberbar::Renderer::VerticesRectUpdateUV( UVertex_V3F_C4B_T2F pVertices[ 4 ], const CRect2f& UV )
{
	pVertices[ 0 ].texcoord = UVector2f( UV.l, UV.t );
	pVertices[ 1 ].texcoord = UVector2f( UV.r, UV.t );
	pVertices[ 2 ].texcoord = UVector2f( UV.l, UV.b );
	pVertices[ 3 ].texcoord = UVector2f( UV.r, UV.b );
}


inline void iberbar::Renderer::VerticesRectUpdateUV( UVector2f pVertices[ 4 ], const CRect2f& UV )
{
	pVertices[ 0 ] = UVector2f( UV.l, UV.t );
	pVertices[ 1 ] = UVector2f( UV.r, UV.t );
	pVertices[ 2 ] = UVector2f( UV.l, UV.b );
	pVertices[ 3 ] = UVector2f( UV.r, UV.b );
}



template < typename T >
inline void iberbar::Renderer::VerticesRectUpdatePosition( UVertex_V3F_C4B_T2F pVertices[ 4 ], const TRect<T>& Bounding, float nZPosition )
{
	pVertices[ 0 ].position = UVector3f( (float)Bounding.l, (float)Bounding.t, nZPosition );
	pVertices[ 1 ].position = UVector3f( (float)Bounding.r, (float)Bounding.t, nZPosition );
	pVertices[ 2 ].position = UVector3f( (float)Bounding.l, (float)Bounding.b, nZPosition );
	pVertices[ 3 ].position = UVector3f( (float)Bounding.r, (float)Bounding.b, nZPosition );
}


template < typename T >
inline void iberbar::Renderer::VerticesRectUpdatePosition( UVector3f pVertices[ 4 ], const TRect<T>& Bounding, float nZPosition )
{
	pVertices[ 0 ] = UVector3f( (float)Bounding.l, (float)Bounding.t, nZPosition );
	pVertices[ 1 ] = UVector3f( (float)Bounding.r, (float)Bounding.t, nZPosition );
	pVertices[ 2 ] = UVector3f( (float)Bounding.l, (float)Bounding.b, nZPosition );
	pVertices[ 3 ] = UVector3f( (float)Bounding.r, (float)Bounding.b, nZPosition );
}