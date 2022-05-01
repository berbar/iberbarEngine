#pragma once


#include <iberbar/RHI/Resource.h>
#include <iberbar/RHI/Types.h>


namespace iberbar
{
	namespace RHI
	{
		class __iberbarRHIApi__ IVertexBuffer abstract
			: public IResource
		{
		public:
			IVertexBuffer( uint32 nInSize, uint32 nUsage )
				: IResource( UResourceType::VertexBuffer )
				, m_nInSize( nInSize )
				, m_nUsage( nUsage )
			{
			}

			inline uint32 GetInSize() const { return m_nInSize; }
			inline uint32 GetUsage() const { return m_nUsage; }
			inline bool IsDynamic() const { return ( m_nUsage & UBufferUsageFlags::AnyDynamic ) != 0; }

		public:
			virtual CResult Lock( uint32 nOffset, uint32 nSize, void** ppOut, uint32 nFlags = 0 ) = 0;
			virtual CResult Unlock() = 0;
			//virtual void Write( void* pData, uint32 nOffset, uint32 nSize ) = 0;

		private:
			uint32 m_nInSize;
			uint32 m_nUsage;
		};


		class __iberbarRHIApi__ IIndexBuffer abstract
			: public IResource
		{
		public:
			IIndexBuffer( uint32 nStride, uint32 nInSize, uint32 nUsage )
				: IResource( UResourceType::IndexBuffer )
				, m_nStride( nStride )
				, m_nInSize( nInSize )
				, m_nUsage( nUsage )
			{
			}

			inline uint32 GetStride() const { return m_nStride; }
			inline uint32 GetInSize() const { return m_nInSize; }
			inline uint32 GetUsage() const { return m_nUsage; }
			inline bool IsDynamic() const { return (m_nUsage & UBufferUsageFlags::AnyDynamic) != 0; }

		public:
			virtual CResult Lock( uint32 nOffset, uint32 nSize, void** ppOut, uint32 nFlags = 0 ) = 0;
			virtual CResult Unlock() = 0;
			//virtual void Write( void* pData, uint32 nOffset, uint32 nSize ) = 0;

		private:
			uint32 m_nStride;
			uint32 m_nInSize;
			uint32 m_nUsage;
		};



		class __iberbarRHIApi__ IUniformBuffer abstract
			: public IResource
		{
		public:
			IUniformBuffer()
				: IResource( UResourceType::UniformBuffer )
			{
			}

		public:
			virtual void UpdateContents( const void* pContents, uint32 nContentSize ) = 0;
		};
	}
}
