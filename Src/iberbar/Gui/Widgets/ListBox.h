#pragma once


#include <iberbar/Gui/Widget.h>
#include <iberbar/Gui/RenderElement.h>
#include <iberbar/Utility/Variant.h>
#include <vector>


namespace iberbar
{
	namespace Gui
	{
		struct UListBoxItem
		{
			std::string strText;
			std::string strValue;
		};

		template < typename TData >
		struct UListBoxItemNode
		{
			UListBoxItemNode()
				: Selected( false )
				, Disabled( false )
				, Data()
			{
			}
			bool Selected;
			bool Disabled;
			TData Data;
		};


		enum class UListBoxDirection
		{
			Vertical = 1,
			Horizental = 2
		};

		enum class UListBoxSelectType
		{
			None,
			Single,
			Multiple,
		};


		class __iberbarGuiApi__ CListBoxNoData abstract
			: public CWidget
		{
		public:
			CListBoxNoData();
			virtual ~CListBoxNoData();
		protected:
			CListBoxNoData( const CListBoxNoData& listBox );

		public:
			virtual UHandleMessageResult HandleMouse( const UMouseEventData* pEvent ) override;
			virtual UHandleMessageResult HandleKeyboard( const UKeyboardEventData* pEvent ) override;
			virtual void OnMouseLeave() override;
			virtual void Update( float nDelta ) override;
			virtual void Render() override;


		public:
			virtual int GetItemCount() const = 0;
			int GetItemAtPoint( const CPoint2i& point ) const;


		public:
			void SetEnableDrag( bool bEnableDrag ) { m_bDragView = bEnableDrag; }
			void SetDragScrollSense( int nSense ) { m_nDragScrollSense = nSense; }
			void SetItemDirection( UListBoxDirection nDirection );
			void SetItemSize( int nWidth, int nHeight );
			void SetItemMargin( int nMargin );


			bool GetEnableDrag() const { return m_bDragView; }
			UListBoxDirection GetItemDirection() const { return m_nItemDirection; }
			bool IsVerticalList() const { return m_nItemDirection == UListBoxDirection::Vertical; }
			int GetItemWidth() const { return m_nItemWidth; }
			int GetItemHeight() const { return m_nItemHeight; }
			int GetItemSpacing() const { return m_nItemSpacing; }


		public:
			void SetSelectType( UListBoxSelectType nType );
			UListBoxSelectType  GetSelectType() const { return m_nSelType; }
			void SetSelect( int nIndex, bool bSelected ) { SetSelectInternal( nIndex, bSelected, false ); }

		public:
			virtual void ClearSelect() = 0;
			virtual bool IsSelect( int nIndex ) const = 0;
			virtual int GetSelect( int nOffset = 0 ) const = 0;
			//virtual bool GetItemSelect();

		protected:
			virtual void SetSelectInternal( int nIndex, bool bSelected, bool bInternal ) = 0;
			void ResizeItemElements( int nOffset = 0, int nCount = -1 );
			void SetItemMouseOver( int nIndex );
			void SetItemPressed( int nIndex );
			void SetItemFocus( int nIndex, bool bFocus );

		protected:
			UListBoxSelectType m_nSelType;
			int m_nItemMouseOver;
			int m_nItemPressed;
			CRenderElement* m_pRenderElementItemsRoot;

			bool m_bDragView;
			bool m_bPressed;
			bool m_bIsDragging;
			int  m_nDragX;
			int  m_nDragY;
			int  m_nDragScrollSense;

			UListBoxDirection m_nItemDirection;
			int  m_nItemWidth;
			int  m_nItemHeight;
			int  m_nItemSpacing;
			int  m_nItemVirtualSize;
			int  m_nShowX;
			int  m_nShowY;
		};



		


		template < typename TData >
		class TListBox
			: public CListBoxNoData
		{
		protected:
			typedef typename UListBoxItemNode<TData> _ItemNode;
		public:
			typedef void UCallback_CreateItemElements( CRenderElement*, const TData* pData, int nIndex );
			typedef void UCallback_UpdateItemElements( CRenderElement*, const TData* pData, int nIndex );

		public:
			TListBox();


		public:
			virtual void Update( float nDelta ) override;
			virtual void ClearSelect() override;
			virtual bool IsSelect( int nIndex ) const override;
			virtual int GetSelect( int nOffset = 0 ) const override;

		protected:
			virtual void SetSelectInternal( int nIndex, bool bSelected, bool bInternal ) override;

		public:
			void SetCallback_CreateItemElements( std::function<UCallback_CreateItemElements> callback ) { m_callbackCreateItemElements = callback; }
			void SetCallback_UpdateItemElements( std::function<UCallback_UpdateItemElements> callback ) { m_callbackUpdateItemElements = callback; }

			int AddItem( const TData& Data );
			int RemoveItemOne( int nIndex );
			int RemoveItems( const int* pIndices, int nCount );
			void RemoveItemsAll();
			int GetItemCount() const { return (int)m_Items.size(); }
			const TData* GetItem( int nIndex );
			int FindItem( std::function<bool( const TData* )>, int nOffset = 0 );

		protected:
			int m_nSelMaxCount;
			int m_nSel;
			std::vector<_ItemNode> m_Items;
			std::function<UCallback_CreateItemElements> m_callbackCreateItemElements;
			std::function<UCallback_UpdateItemElements> m_callbackUpdateItemElements;
		};


		//class __iberbarGuiApi__ CListBoxItemDataDictionary
		//{
		//public:
		//	const CVariant* Get( const char* strName ) const;
		//	void SetString( const char* strName, const char* value );
		//	void SetBoolean( const char* strName, bool value );
		//	void SetInt( const char* strName, int64 value );
		//	void SetUInt( const char* strName, uint64 value );
		//	void SetFloat( const char* strName, float value );

		//	const std::unordered_map<std::string, CVariant>& GetMap() const { return m_AttrsMap; }

		//protected:
		//	std::unordered_map<std::string, CVariant> m_AttrsMap;
		//};


		typedef TListBox<CVariantDictionary> CListBoxDictionary;
	}
}






template < typename TData >
iberbar::Gui::TListBox<TData>::TListBox()
	: m_nSelMaxCount( -1 )
	, m_nSel( -1 )
	, m_Items()
	, m_callbackCreateItemElements()
	, m_callbackUpdateItemElements()
{

}


template< typename TData >
void iberbar::Gui::TListBox<TData>::Update( float nDelta )
{
	CListBoxNoData::Update( nDelta );
}


template< typename TData >
void iberbar::Gui::TListBox<TData>::ClearSelect()
{
	if ( GetSelectType() == UListBoxSelectType::Single )
	{
		if ( m_nSel < (int)m_Items.size() )
			m_Items[ m_nSel ].Selected = false;
	}
	else
	{
		for ( _ItemNode& itemNode : m_Items )
		{
			itemNode.Selected = false;
		}
	}
	m_nSel = -1;
}


template < typename TData >
bool iberbar::Gui::TListBox<TData>::IsSelect( int nIndex ) const
{
	if ( nIndex < 0 || nIndex >= (int)m_Items.size() )
		return false;
	const _ItemNode& item = m_Items[ nIndex ];
	return item.Selected;
}


template < typename TData >
int iberbar::Gui::TListBox<TData>::GetSelect( int nOffset ) const
{
	if ( GetSelectType() == UListBoxSelectType::Single )
	{
		if ( m_nSel >= nOffset )
			return m_nSel;
	}
	else if ( GetSelectType() == UListBoxSelectType::Multiple )
	{
		for ( int i = nOffset, s = m_Items.size(); i < s; i++ )
		{
			if ( m_Items[ i ].Selected == true )
				return i;
		}
	}

	return -1;
}



template < typename TData >
void iberbar::Gui::TListBox<TData>::SetSelectInternal( int index, bool sel, bool bInternal )
{
	int nCount = GetItemCount();

	if ( nCount == 0 )
	{
		m_nSel = -1;
		return;
	}

	if ( index < 0 || index >= nCount )
		return;

	if ( GetSelectType() == UListBoxSelectType::Single )
	{
		if ( index != m_nSel )
		{
			if ( sel == true )
			{
				if ( m_nSel >= 0 && m_nSel < nCount )
				{
					m_Items[ m_nSel ].Selected = false;
					SetItemFocus( m_nSel, false );
				}
					
				m_nSel = index;

				if ( m_nSel >= 0 && m_nSel < nCount )
				{
					m_Items[ m_nSel ].Selected = true;
					SetItemFocus( m_nSel, true );
				}

				if ( bInternal )
					SendEvent( BaseEvent::nValueChanged, index, nullptr );
			}
			else
			{
				
			}
		}
	}
	else if ( GetSelectType() == UListBoxSelectType::Multiple )
	{
		_ItemNode& item = m_Items[ index ];
		if ( item.Selected != sel )
		{
			item.Selected = sel;
			SetItemFocus( index, sel );

			if ( bInternal )
				SendEvent( BaseEvent::nValueChanged, index, nullptr );
		}
	}
}


template < typename TData >
int iberbar::Gui::TListBox<TData>::AddItem( const TData& Data )
{
	_ItemNode item;
	item.Selected = false;
	item.Disabled = false;
	item.Data = Data;
	m_Items.push_back( item );

	int nIndex = (int)m_Items.size() - 1;

	if ( m_pRenderElementItemsRoot != nullptr )
	{
		PTR_CRenderElement pElement = PTR_CRenderElement::_sNew();
		m_pRenderElementItemsRoot->AddChildElement( pElement );
		ResizeItemElements( nIndex, 1 );

		if ( m_callbackCreateItemElements )
			m_callbackCreateItemElements( pElement, &Data, nIndex );

		pElement->SetState( (int)UWidgetState::Normal );
	}

	return nIndex;
}


template < typename TData >
int iberbar::Gui::TListBox<TData>::RemoveItemOne( int nIndex )
{
	if ( nIndex < 0 || nIndex >= (int)m_Items.size() )
		return 0;

	m_Items.erase( m_Items.begin() + nIndex );

	m_pRenderElementItemsRoot->RemoveChildElement( nIndex );

	ResizeItemElements( nIndex );

	return 1;
}


template < typename TData >
int iberbar::Gui::TListBox<TData>::RemoveItems( const int* pIndices, int nCount )
{
	//int nIndexMin = pIndices[ 0 ];

	//std::vector<_ItemNode> ItemListNew;
	//ItemListNew.reserve( m_Items.size() );

	//m_Items.swap( ItemListNew );

	//m_pRenderElementItemsRoot->RemoveChildElement( 0 );

	//ResizeItemElements( nIndexMin );

	return 0;
}


template < typename TData >
void iberbar::Gui::TListBox<TData>::RemoveItemsAll()
{
	m_Items.clear();
	m_pRenderElementItemsRoot->RemoveChildElementsAll();
}


template < typename TData >
const TData* iberbar::Gui::TListBox<TData>::GetItem( int nIndex )
{
	if ( nIndex < 0 || nIndex >= (int)m_Items.size() )
		return nullptr;

	return &m_Items[ nIndex ].Data;
}


template < typename TData >
int iberbar::Gui::TListBox<TData>::FindItem( std::function<bool( const TData* )> Compare, int nOffset )
{
	if ( !Compare )
		return -1;

	for ( int i = nOffset, s = (int)m_Items.size(); i < s; i ++ )
	{
		if ( Compare( &m_Items[ i ].Data ) == true )
			return i;
	}

	return -1;
}







//
//
//
//inline const iberbar::CVariant* iberbar::Gui::CListBoxItemDataDictionary::Get( const char* strName ) const
//{
//	auto iter = m_AttrsMap.find( strName );
//	if ( iter == m_AttrsMap.end() )
//		return nullptr;
//	return &iter->second;
//}
//
//
//inline void iberbar::Gui::CListBoxItemDataDictionary::SetString( const char* strName, const char* value )
//{
//	if ( Get( strName ) != nullptr )
//	{
//		m_AttrsMap[ strName ] = CVariant( value );
//	}
//	else
//	{
//		m_AttrsMap.insert( std::make_pair( strName, CVariant( value ) ) );
//	}
//}
//
//
//inline void iberbar::Gui::CListBoxItemDataDictionary::SetBoolean( const char* strName, bool value )
//{
//	if ( Get( strName ) != nullptr )
//	{
//		m_AttrsMap[ strName ] = CVariant( value );
//	}
//	else
//	{
//		m_AttrsMap.insert( std::make_pair( strName, CVariant( value ) ) );
//	}
//}
//
//
//inline void iberbar::Gui::CListBoxItemDataDictionary::SetInt( const char* strName, int64 value )
//{
//	if ( Get( strName ) != nullptr )
//	{
//		m_AttrsMap[ strName ] = CVariant( value );
//	}
//	else
//	{
//		m_AttrsMap.insert( std::make_pair( strName, CVariant( value ) ) );
//	}
//}
//
//
//inline void iberbar::Gui::CListBoxItemDataDictionary::SetUInt( const char* strName, uint64 value )
//{
//	if ( Get( strName ) != nullptr )
//	{
//		m_AttrsMap[ strName ] = CVariant( value );
//	}
//	else
//	{
//		m_AttrsMap.insert( std::make_pair( strName, CVariant( value ) ) );
//	}
//}
//
//
//inline void iberbar::Gui::CListBoxItemDataDictionary::SetFloat( const char* strName, float value )
//{
//	if ( Get( strName ) != nullptr )
//	{
//		m_AttrsMap[ strName ] = CVariant( value );
//	}
//	else
//	{
//		m_AttrsMap.insert( std::make_pair( strName, CVariant( value ) ) );
//	}
//}

