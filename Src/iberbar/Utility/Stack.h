
#pragma once

#include <iberbar/Utility/Platform.h>

namespace iberbar
{
	template < typename T >
	class __iberbarUtilityApi__ TListStack
	{
	private:
		struct Node
		{
			Node* prev;
			T data;
		};


	public:
		TListStack( void )
			: m_pTop( nullptr )
			, m_size( 0 )
		{
		}

		TListStack( const TListStack& stack )
			: m_pTop( NULL )
			, m_size( 0 )
		{
			if ( stack.IsEmpty() == false )
			{
				Node* lc_ptr = stack.m_pTop;
				Node** lc_nodes = new Node*[ stack.Size() ];

				lc_nodes[ 0 ] = new Node;
				lc_nodes[ 0 ]->data = lc_ptr->data;
				lc_nodes[ 0 ]->prev = NULL;
				lc_ptr = lc_ptr->prev;

				for ( uint32 lc_i = 1; lc_i < stack.Size(); lc_i ++ )
				{
					lc_nodes[ lc_i ] = new Node;
					lc_nodes[ lc_i ]->data = lc_ptr->data;
					lc_nodes[ lc_i ]->prev = NULL;
					lc_nodes[ lc_i-1 ]->prev = lc_nodes[ lc_i ];
					lc_ptr = lc_ptr->prev;
				}

				m_pTop = lc_nodes[ 0 ];
				m_size = stack.Size();
			}
		}

		~TListStack()
		{
			Node* lc_node = m_pTop;
			while ( lc_node )
			{
				m_pTop = lc_node->prev;
				delete lc_node;
				lc_node = m_pTop;
			}
		}


	public:
		void Push( const T& val )
		{
			Node* lc_new = new Node;
			if ( lc_new == NULL )
				return ;
			lc_new->data = val;
			lc_new->prev = m_pTop;
			m_pTop = lc_new;
			m_size ++;
		}

		void Pop()
		{
			assert( m_pTop );
			Node* lc_delete = m_pTop;
			m_pTop = m_pTop->prev;
			delete lc_delete;
			m_size --;
		}

		void Clear()
		{
			Node* lc_node = m_pTop;
			while ( lc_node )
			{
				m_pTop = lc_node->prev;
				delete lc_node;
				lc_node = m_pTop;
			}
			m_size = 0;
		}

		T& Top()
		{
			assert( m_pTop );
			return m_pTop->data;
		}

		const T& Top() const
		{
			assert( m_pTop );
			return m_pTop->data;
		}

		uint32 Size() const
		{
			return m_size;
		}

		bool IsEmpty() const
		{
			if ( m_pTop == NULL )
				return true;
			return false;
		}


	private:
		Node*   m_pTop;
		uint32 m_size;
	};
}

