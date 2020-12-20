
#pragma once

#include <iberbar/RHI/Buffer.h>
#include <iberbar/RHI/OpenGL/Device.h>
#include <iberbar/Utility/Buffer.h>

namespace iberbar
{
	namespace RHI
	{
		namespace OpenGL
		{
			class CDevice;

			//typedef void (*PFunctionBufferBind)(GLuint Buffer);

			//template < typename BaseType, GLenum Type, PFunctionBufferBind BufBind >
			//class __iberbarExports__ TBuffer
			//	: public BaseType
			//{
			//public:
			//	TBuffer( uint32 nStride, uint32 nInSize, uint32 nUsage )
			//		: BaseType( nStride, nInSize, nUsage )
			//		, m_bIsLocked(0)
			//		, m_bIsLockReadOnly( 0 )
			//		, m_bStreamDraw( false )
			//		, m_bLockBufferWasAllocated( 0 )
			//		, m_nLockSize( 0 )
			//		, m_nLockOffset( 0 )
			//		, m_pLockBuffer( nullptr )
			//		, m_nRealSize( 0 )
			//	{

			//	}
			//	virtual ~TBuffer();

			//public:
			//	void Bind()
			//	{
			//		BufBind( m_Resource );
			//	}
			//	//uint8* Lock( uint32 InOffset, uint32 InSize, bool bReadOnly, bool bDiscard );
			//	//void Unlock();
			//	void UpdateData( void* pData, uint32 nOffset, uint32 nSize );

			//private:
			//	CDevice* m_pDevice;
			//	GLuint m_Resource;

			//	uint32 m_bIsLocked : 1;
			//	uint32 m_bIsLockReadOnly : 1;
			//	uint32 m_bStreamDraw : 1;
			//	uint32 m_bLockBufferWasAllocated : 1;

			//	uint32 m_nLockSize;
			//	uint32 m_nLockOffset;
			//	void* m_pLockBuffer;

			//	uint32 m_nRealSize;
			//};

			class __iberbarRHIOpenGLApi__ CVertexBuffer
				: public IVertexBuffer
			{
			public:
				CVertexBuffer( CDevice* pDevice, uint32 nSize, uint32 nUsage );
				virtual ~CVertexBuffer();

			public:
				void Initial();
				void Destory();
				inline void Bind() { m_pDevice->BindArrayBuffer( m_nBuffer ); }
				inline GLenum GetAccess()
				{
					return m_bStreamDraw == true ? GL_STREAM_DRAW : (IsDynamic() ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
				}

			public:
				virtual CResult Lock( uint32 nOffset, uint32 nSize, void** ppOut, uint32 nFlags = 0 ) override;
				virtual CResult Unlock() override;
				//virtual void Write( void* pData, uint32 nOffset, uint32 nSize ) override;
				
			protected:
				CDevice* m_pDevice;
				GLuint m_nBuffer;
				bool m_bStreamDraw;
				bool m_bLocked;
				uint32 m_nLockSize;
				TBuffer<uint8> m_LockBuffer;
			};

			class __iberbarRHIOpenGLApi__ CIndexBuffer
				: public IIndexBuffer
			{
			public:
				CIndexBuffer( CDevice* pDevice, uint32 nStride, uint32 nSize, uint32 nUsage );
				virtual ~CIndexBuffer();

			public:
				void Initial();
				void Destory();
				inline void Bind() { m_pDevice->BindElementArrayBuffer( m_nBuffer ); }
				inline GLenum GetAccess()
				{
					return m_bStreamDraw == true ? GL_STREAM_DRAW : (IsDynamic() ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
				}

			public:
				virtual CResult Lock( uint32 nOffset, uint32 nSize, void** ppOut, uint32 nFlags = 0 ) override;
				virtual CResult Unlock() override;
				//virtual void Write( void* pData, uint32 nOffset, uint32 nSize ) override;

			protected:
				CDevice* m_pDevice;
				GLuint m_nBuffer;
				bool m_bStreamDraw;
				bool m_bLocked;
				uint32 m_nLockSize;
				TBuffer<uint8> m_LockBuffer;
			};
		}
	}
}
