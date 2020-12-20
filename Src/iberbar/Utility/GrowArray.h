

namespace iberbar
{
	template < typename T >
	class TGrowArray
	{
	public:
		TGrowArray( void )
			: m_count( 0 )
			, m_data( NULL )
		{
		}

		TGrowArray( const TGrowArray& dest )
			: m_count( 0 )
			, m_data( NULL )
		{
			if ( dest.m_data )
			{
				m_data = new T[ dest.m_count ];
				memcpy_s( m_data, sizeof( T ) * dest.m_count, dest.m_data, sizeof( T ) * dest.m_count );
				m_count = dest.m_count;
			}
		}

		~TGrowArray()
		{
			clear();
		}

	public:
		/*
		 * 当resize的尺寸小于已有尺寸，多余的对象会丢失，注意内存是否会泄露
		 */
		void resize( int count )
		{
			if ( count == m_count )
				return ;
			T* lc_new = new T[ count ];
			memcpy_s( lc_new, sizeof( T )*count, m_data, sizeof( T )*tMin<uint32>( m_count, count ) );
			delete[] m_data;
			m_data = lc_new;
			m_count = count;
		}

		void push_back( const T& val )
		{
			T* lc_new = new T[ m_count + 1 ];
			memcpy_s( lc_new, sizeof( T )*( m_count+1 ), m_data, sizeof( T )*m_count );
			lc_new[ m_count ] = val;
			delete[] m_data;
			m_data = lc_new;
			m_count ++;
		}

		void push_front( const T& val )
		{
			T* lc_new = new T[ m_count + 1 ];
			memcpy_s( lc_new+1, sizeof( T )*( m_count+1 ), m_data, sizeof( T )*m_count );
			lc_new[ 0 ] = val;
			delete[] m_data;
			m_data = lc_new;
			m_count ++;
		}

		void insert( uint32 index, const T& val )
		{
			assert( index < m_count );
			T* lc_new = new T[ m_count + 1 ];
			memcpy_s( lc_new, sizeof( T )*( m_count+1 ), m_data, sizeof( T )*index );
			lc_new[ index ] = val;
			memcpy_s( lc_new + index + 1, sizeof( T )*( m_count+1 ), m_data+index, sizeof( T )*( m_count-index ) );
			delete[] m_data;
			m_data = lc_new;
			m_count ++;
		}

		void remove( uint32 index )
		{
			assert( index < m_count );
			T* lc_new = new T[ m_count-1 ];
			memcpy_s( lc_new, sizeof( T )*( m_count-1 ), m_data, sizeof( T )*index );
			memcpy_s( lc_new + index, sizeof( T )*( m_count+1 ), m_data+index+1, sizeof( T )*( m_count-index-1 ) );
			delete[] m_data;
			m_data = lc_new;
			m_count --;
		}

		T pop_back()
		{
			assert( m_count > 0 );
			T lc_pop = m_data[ m_count-1 ];
			T* lc_new = new T[ m_count-1 ];
			memcpy_s( lc_new, sizeof( T )*( m_count-1 ), m_data, sizeof( T )*( m_count-1 ) );
			delete[] m_data;
			m_data = lc_new;
			m_count --;
			return lc_pop;
		}

		T pop_front()
		{
			assert( m_count > 0 );
			T lc_pop = m_data[ 0 ];
			T* lc_new = new T[ m_count - 1 ];
			memcpy_s( lc_new, sizeof( T )*( m_count-1 ), m_data+1, sizeof( T )*( m_count-1 ) );
			delete[] m_data;
			m_data = lc_new;
			m_count --;
			return lc_pop;
		}

		void clear()
		{
			delete[] m_data;
			m_data = NULL;
			m_count = 0;
		}


	public:
		uint32 size() const
		{
			return m_count;
		}

		uint32 get_data( T* pBuf, uint32 nBufSize ) const
		{
			uint32 lc_copyCount = tMin< uint32 >( nBufSize, m_count );
			if ( lc_copyCount > 0 )
				memcpy_s( pBuf, nBufSize*sizeof(T), m_data, lc_copyCount*sizeof(T) );
			return lc_copyCount;
		}

		bool empty() const
		{
			if ( m_count == 0 )
				return true;
			return false;
		}

	public:
		T& operator[]( uint32 index )
		{
			assert( index < m_count );
			return m_data[ index ];
		}

		const T& operator[]( uint32 index ) const
		{
			assert( index < m_count );
			return m_data[ index ];
		}

	private:
		uint32 m_count;
		T* m_data;
	};
}