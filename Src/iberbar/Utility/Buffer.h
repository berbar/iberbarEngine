
#pragma once


#include <iberbar/Utility/Platform.h>
#include <iberbar/Utility/Math.h>

namespace iberbar
{
	template < typename T >
	class TBuffer
	{
	public:
		TBuffer( void );
		TBuffer( const TBuffer& SrcObject ) throw();
		~TBuffer();

	public:
		void Resize( uint32 size, bool bCopy = true );
		void Clear();
		T* GetPointer();
		const T* GetPointer() const;
		uint32 GetDataSize() const;

	private:
		T* m_pData;
		uint32 m_nDataSize;
	};


	// »á²úÉúËéÆ¬
	template < typename T >
	class TBufferList
	{
	public:
		TBufferList( uint32 nElementsPerBuffer );
		~TBufferList();

	public:
		void ResetUsed();
		T* AllocElements( uint32 nElementCount );
		void Init( uint32 nBufferCount = 1 );
		void Clear();

	private:
		uint32 m_nElementsPerBuffer;
		uint32 m_nUsedBuffer;
		uint32 m_nUsedElementCount;
		std::vector< T* > m_pDataList;
	};
}


template < typename T >
iberbar::TBuffer<T>::TBuffer( void )
	: m_pData( nullptr )
	, m_nDataSize( 0 )
{
}

template < typename T >
iberbar::TBuffer<T>::TBuffer( const TBuffer& SrcObject ) throw()
	: m_pData( nullptr )
	, m_nDataSize( 0 )
{
	if ( SrcObject.m_pData && SrcObject.m_nDataSize > 0 )
	{
		m_pData = new T[SrcObject.m_nDataSize];
		if ( m_pData )
			m_nDataSize = SrcObject.m_nDataSize;
	}
}

template < typename T >
iberbar::TBuffer<T>::~TBuffer()
{
	SAFE_DELETE_ARRAY( m_pData );
}

template < typename T >
void iberbar::TBuffer<T>::Resize( uint32 nSize, bool bCopy )
{
	if ( nSize <= 0 )
		return;

	if ( nSize <= m_nDataSize )
		return;

	T* pDataTemp = new T[ nSize ];

	if ( m_nDataSize > 0 )
	{
		if ( bCopy == true )
		{
#ifdef _WIN32
			memcpy_s( pDataTemp, nSize, m_pData, m_nDataSize );
#else
			memcpy( pDataTemp, m_pData, m_nDataSize );
#endif
		}

		delete[] m_pData;
	}

	m_pData = pDataTemp;
	m_nDataSize = nSize;
}

template < typename T >
void iberbar::TBuffer<T>::Clear()
{
	SAFE_DELETE_ARRAY( m_pData );
	m_nDataSize = 0;
}

template < typename T >
T* iberbar::TBuffer<T>::GetPointer()
{
	return m_pData;
}

template < typename T >
const T* iberbar::TBuffer<T>::GetPointer() const
{
	return m_pData;
}

template < typename T >
uint32 iberbar::TBuffer<T>::GetDataSize() const
{
	return m_nDataSize;
}





template < typename T >
iberbar::TBufferList< T >::TBufferList( uint32 nElementsPerBuffer )
	: m_nElementsPerBuffer( nElementsPerBuffer )
	, m_nUsedBuffer( 0 )
	, m_nUsedElementCount( 0 )
	, m_pDataList()
{
	m_pDataList.push_back( new T[ m_nElementsPerBuffer ] );
}


template < typename T >
iberbar::TBufferList<T>::~TBufferList()
{
	for ( size_t i = 0, s = m_pDataList.size(); i < s; i++ )
	{
		delete[] m_pDataList[ i ];
		m_pDataList[ i ] = nullptr;
	}
	m_pDataList.clear();
}


template < typename T >
inline void iberbar::TBufferList<T>::ResetUsed()
{
	m_nUsedBuffer = 0;
	m_nUsedElementCount = 0;
}


template < typename T >
inline T* iberbar::TBufferList<T>::AllocElements( uint32 nElementCount )
{
	assert( nElementCount <= m_nElementsPerBuffer );

	uint32 nUsedElementCountNew = m_nUsedElementCount + nElementCount;
	T* pBuffer = nullptr;
	if ( nUsedElementCountNew > m_nElementsPerBuffer )
	{
		m_nUsedBuffer++;
		m_nUsedElementCount = 0;
		if ( m_nUsedBuffer == m_pDataList.size() )
		{
			m_pDataList.push_back( pBuffer = new T[ m_nElementsPerBuffer ] );
		}
		else
		{
			pBuffer = m_pDataList[ m_nUsedBuffer ];
		}
	}
	else
	{
		pBuffer = m_pDataList[ m_nUsedBuffer ] + m_nUsedElementCount;
		m_nUsedElementCount = nUsedElementCountNew;
	}
	return pBuffer;
}


template < typename T >
void iberbar::TBufferList<T>::Init( uint32 nBufferCount )
{
	//for ( uint32 i = 0; i < nBufferCount )
	//{

	//}
}


template < typename T >
void iberbar::TBufferList<T>::Clear()
{
	for ( size_t i = 0, s = m_pDataList.size(); i < s; i++ )
	{
		delete[] m_pDataList[ i ];
		m_pDataList[ i ] = nullptr;
	}
	m_pDataList.clear();
	m_nUsedBuffer = 0;
	m_nUsedElementCount = 0;
}



