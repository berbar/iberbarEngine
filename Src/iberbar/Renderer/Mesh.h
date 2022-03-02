#pragma once

#include <iberbar/Renderer/Headers.h>
#include <iberbar/Renderer/Types.h>
#include <iberbar/Utility/Math/Matrix.h>


namespace iberbar
{
	namespace RHI
	{
		class IVertexDeclaration;
	}

	namespace Renderer
	{

		


		class __iberbarRendererApi__ CMesh
			: public CRef
		{
		public:
			CMesh();
			CMesh( const CMesh& ) = delete;

			inline uint32 GetIndexCount() const { return m_nIndexCount; }
			inline uint16* GetIndexData() { return m_IndexData; }
			inline const uint16* GetIndexData() const { return m_IndexData; }
			
			inline uint32 GetVertexCount() const { return m_nVertexCount; }
			inline const void* GetVertexData( int nStreamIndex ) const { return (&m_Positions)[ nStreamIndex ]; }
			inline const UVector3f* GetPositions() const { return m_Positions; }
			inline const CColor4F* GetColors() const { return m_Colors; }
			inline const UVector3f* GetNormals() const { return m_Normals; }
			inline const UVector2f* GetTexcoords( int nSlot ) const { return m_TexcoordSlots[ nSlot  ]; }
			

		protected:
			uint32 m_nIndexCount;
			uint16* m_IndexData;
			uint32 m_nVertexCount;
			
			UVector3f* m_Positions;
			CColor4F* m_Colors;
			UVector3f* m_Normals;
			UVector2f* m_TexcoordSlots[8];
		};


		//class __iberbarRendererApi__ CMeshFilter
		//{
		//public:
		//	void SetMesh( IMesh* pMesh );
		//	IMesh* GetMesh();

		//protected:
		//	IMesh* m_pMesh;
		//};


		class __iberbarRendererApi__ CMeshDefault
			: public CMesh
		{
		public:
			CMeshDefault();
			virtual ~CMeshDefault();

			void NewVertices( uint32 nVertexCount, uint32 nUsage );
			void NewIndices( uint32 nIndexCount );

			inline uint16* GetBufferIndices() { return m_IndexData; }
			inline UVector3f* GetBufferPositions() { return m_Positions; }
			inline CColor4F* GetBufferColors() { return m_Colors; }
			inline UVector3f* GetBufferNormals() { return m_Normals; }
			inline UVector2f* GetBufferTexcoords( int nSlot ) { return m_TexcoordSlots[ nSlot ]; }

		protected:
			uint32 m_nVertexUsage;
		};



		template < int tTexCount >
		class TMeshForUI
			: public CMesh
		{
		public:
			TMeshForUI()
			{
				memset( m_BufferPositions, 0, sizeof( m_BufferPositions ) );
				memset( m_BufferColors, 0, sizeof( m_BufferColors ) );
				memset( m_BufferTexcoordSlots, 0, sizeof( m_BufferTexcoordSlots ) );
				memset( m_BufferIndices, 0, sizeof( m_BufferIndices ) );

				m_nIndexCount = 6;
				m_IndexData = m_BufferIndices;

				m_nVertexCount = 4;
				m_Positions = m_BufferPositions;
				m_Colors = m_BufferColors;
				for ( int i = 0; i < tTexCount; i++ )
				{
					m_TexcoordSlots[ i ] = m_BufferTexcoordSlots[ i ];
				}
			}

		public:
			inline UVector3f* GetBufferPositions() { return m_BufferPositions; }
			inline CColor4F* GetBufferColors() { return m_BufferColors; }
			inline UVector2f* GetBufferTexcoords( int nSlot ) { assert( nSlot < tTexCount ); return m_BufferTexcoordSlots[ nSlot ]; }

		protected:
			UVector3f m_BufferPositions[ 4 ];
			CColor4F m_BufferColors[ 4 ];
			UVector2f m_BufferTexcoordSlots[ 4 ][ tTexCount ];
			uint16 m_BufferIndices[ 6 ];
		};

		template class __iberbarRendererApi__ TMeshForUI<1>;
		typedef TMeshForUI<1> CMeshForUI_1;
	}
}