

#ifndef __iberbar_vector_h__
#define __iberbar_vector_h__

#include <vector>

namespace iberbar
{

	template < typename _Key, typename _Data >
	class TVector_SortDichotomy
	{
	public:
		
		struct _Node
		{
			friend class TVector_SortDichotomy;

		private:
			_Node( void ) {}
			~_Node() {}
		private:
			_Key key;
		public:
			_Data data;
			const _Key& getKey() const { return key; }
		};

		typedef typename std::vector< _Node* >     VectorCache;
		typedef typename VectorCache::iterator     node_iterator;


	public:
		bool find( const _Key& key, _Data* ptr ) const;
		void reserve( size_t count );
		bool add( const _Key& key, const _Data& data, _Data* pOld = NULL, bool bIsRecover = false );  // can't recover if there is data
		void remove( const _Key& key );
		void clear();
		size_t size() const;
		bool empty() const;
		bool empty_at_key( const _Key& key ) const;

		node_iterator begin() { return m_Datas.begin(); }
		node_iterator end() { return m_Datas.end(); }
		node_iterator erase( node_iterator node );

		node_iterator get_iterator( const _Key& key );


	private:
		bool find_at_index( _Key key, size_t* pindex ) const;

	private:
		VectorCache m_Datas;
	};



	template < typename _Key, typename _Data >
	bool TVector_SortDichotomy< _Key, _Data >::find( const _Key& key, _Data* ptr ) const
	{
		size_t lc_index;
		if ( find_at_index( key, &lc_index ) == false )
			return false;
		if ( ptr )
			*ptr = m_Datas[ lc_index ]->data;
		return true;
	}


	template < typename _Key, typename _Data >
	void TVector_SortDichotomy< _Key, _Data >::reserve( size_t count )
	{
		m_Datas.reserve( count );
	}


	template < typename _Key, typename _Data >
	bool TVector_SortDichotomy< _Key, _Data >::add( const _Key& key, const _Data& data, _Data* pOld, bool bIsRecover )
	{
		size_t lc_index;
		if ( find_at_index( key, &lc_index ) == false )
		{
			_Node* lc_node = new _Node();
			lc_node->data = data;
			lc_node->key = key;
			m_Datas.insert( m_Datas.begin() + lc_index, lc_node );
			return true;
		}

		if ( bIsRecover == true )
		{
			if ( pOld )
				*pOld = m_Datas[ lc_index ]->data;
			m_Datas[ lc_index ]->data = data;
			return true;
		}

		return false;
	}


	template < typename _Key, typename _Data >
	void TVector_SortDichotomy< _Key, _Data >::remove( const _Key& key )
	{
		size_t lc_index;
		if ( find_at_index( key, &lc_index ) == true )
		{
			node_iterator lc_node = m_Datas.begin() + lc_index;
			erase( lc_node );
		}
	}


	template < typename _Key, typename _Data >
	void TVector_SortDichotomy< _Key, _Data >::clear()
	{
		node_iterator lc_iter = m_Datas.begin();
		node_iterator lc_end  = m_Datas.end();
		for ( ; lc_iter != lc_end; lc_iter ++ )
		{
			if ( *lc_iter )
				delete (*lc_iter);
			(*lc_iter) = NULL;
		}
		m_Datas.clear();
		//m_Datas.swap( VectorCache() );
	}


	template < typename _Key, typename _Data >
	size_t TVector_SortDichotomy< _Key, _Data >::size() const
	{
		return m_Datas.size();
	}

	template < typename _Key, typename _Data >
	bool TVector_SortDichotomy< _Key, _Data >::empty() const
	{
		return m_Datas.empty();
	}


	template < typename _Key, typename _Data >
	bool TVector_SortDichotomy< _Key, _Data >::empty_at_key( const _Key& key ) const
	{
		if ( find_at_index( key, NULL ) == false )
			return true;
		return true;
	}


	template < typename _Key, typename _Data >
	typename TVector_SortDichotomy< _Key, _Data >::node_iterator TVector_SortDichotomy< _Key, _Data >::erase( typename TVector_SortDichotomy< _Key, _Data >::node_iterator node )
	{
		SAFE_DELETE( *node );
		return m_Datas.erase( node );
	}


	template < typename _Key, typename _Data >
	typename TVector_SortDichotomy< _Key, _Data >::node_iterator TVector_SortDichotomy< _Key, _Data >::get_iterator( const _Key& key )
	{
		size_t lc_index;
		if ( find_at_index( key, &lc_index ) == false )
			return m_Datas.end();
		return m_Datas.begin() + lc_index;
	}


	template < typename _Key, typename _Data >
	bool TVector_SortDichotomy< _Key, _Data >::find_at_index( _Key key, size_t* pIndex ) const
	{
		if ( m_Datas.empty() )
		{
			if ( pIndex )
				*pIndex = 0;
			return false;
		}

		size_t lc_nLeft  = 0;
		size_t lc_nRight = m_Datas.size() - 1;

		// 如果只有一个
		if ( lc_nRight == 0 )
		{
			if ( m_Datas[ lc_nRight ]->key == key )
			{
				if ( pIndex )
					*pIndex = lc_nRight;
				return true;
			}
			else if ( m_Datas[ lc_nRight ]->key < key )
			{
				if ( pIndex )
					*pIndex = lc_nRight+1;
				return false;
			}
			else
			{
				if ( pIndex )
					*pIndex = lc_nRight;
				return false;
			}
		}

		//----------------------
		// 二分法
		//----------------------
		// 1. check left
		//lc_cmp = arrayData[ lc_nLeft ]->cmp( compData );
		if ( m_Datas[ lc_nLeft ]->key == key )
		{
			if ( pIndex )
				*pIndex = lc_nLeft;
			return true;
		}
		else if ( m_Datas[ lc_nLeft ]->key > key )
		{
			if ( pIndex )
				*pIndex = lc_nLeft;
			return false;
		}

		// 2. check right
		//lc_cmp = m_Datas[ lc_nRight ]->cmp( compData );
		if ( m_Datas[ lc_nRight ]->key == key )
		{
			if ( pIndex )
				*pIndex = lc_nRight;
			return true;
		}
		else if ( m_Datas[ lc_nRight ]->key < key )
		{
			if ( pIndex )
				*pIndex = lc_nRight+1;
			return false;
		}

		// 3. divide
		size_t lc_nMiddle = 0;
		while ( ( lc_nRight-lc_nLeft ) > 1 )
		{
			lc_nMiddle = ( lc_nLeft + lc_nRight ) / 2;
			//lc_cmp = arrayData[ lc_nMiddle ]->cmp( compData );
			if ( m_Datas[ lc_nMiddle ]->key == key )
			{
				if ( pIndex )
					*pIndex = lc_nMiddle;
				return true;
			}
			else if ( m_Datas[ lc_nMiddle ]->key < key )
			{
				lc_nLeft = lc_nMiddle;
			}
			else
			{
				lc_nRight = lc_nMiddle;
			}
		}

		if ( pIndex )
			*pIndex = (int)lc_nRight;

		return false;
	}


}


#endif