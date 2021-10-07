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
				, bEnable( true )
				, Data()
			{
			}
			bool Selected;
			bool bEnable;
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

		enum class EListBoxDragStyle
		{
			None,
			DragView,
			DragItem,
		};


		class __iberbarGuiApi__ CListBoxNoData abstract
			: public CWidget
		{
		public:
			typedef CRenderElement* (UCallback_ItemElementAllocate)( );
			typedef void (UCallback_ItemElementRelease)( CRenderElement* );

		public:
			CListBoxNoData();
			virtual ~CListBoxNoData();
		protected:
			CListBoxNoData( const CListBoxNoData& listBox );

		public:
			virtual UHandleMessageResult HandleMouse( const UMouseEventData* pEvent ) override;
			virtual UHandleMessageResult HandleKeyboard( const UKeyboardEventData* pEvent ) override;
			virtual void OnMouseLeave() override;
			virtual void OnFocusOut() override;
			virtual void Refresh() override;
			virtual void Update( float nDelta ) override;
			virtual void Render() override;


		public:
			virtual int GetItemCount() const = 0;
			int GetItemAtPoint( const CPoint2i& point ) const;


		public:
			void SetCallback_ItemElementAllocate( std::function<CRenderElement* ( )> Callback ) { m_Callback_ItemElementAllocate = Callback; }
			void SetCallback_ItemElementRelease( std::function<void ( CRenderElement* )> Callback ) { m_Callback_ItemElementRelease = Callback; }
			void SetDragStyle( EListBoxDragStyle nStyle ) { m_nDragStyle = nStyle; }
			void SetDragScrollSense( int nSense ) { m_nDragScrollSense = nSense; }
			void SetItemDirection( UListBoxDirection nDirection );
			void SetItemSize( int nWidth, int nHeight );
			void SetItemMargin( int nMargin );


			EListBoxDragStyle GetDragStyle() const { return m_nDragStyle; }
			UListBoxDirection GetItemDirection() const { return m_nItemDirection; }
			bool IsVerticalList() const { return m_nItemDirection == UListBoxDirection::Vertical; }
			int GetItemWidth() const { return m_nItemWidth; }
			int GetItemHeight() const { return m_nItemHeight; }
			int GetItemSpacing() const { return m_nItemSpacing; }




		public:
			void SetSelectType( UListBoxSelectType nType );
			UListBoxSelectType  GetSelectType() const { return m_nSelType; }
			void SetSelect( int nIndex, bool bSelected ) { SetSelectInternal( nIndex, bSelected, false ); }
			int GetItemContextMenu() const { return m_nItemContextMenu; }
			CPoint2i GetItemContextMenuAtPoint() const { return CPoint2i( m_nItemContextMenu_AtPointX, m_nItemContextMenu_AtPointY ); }
			CRect2i GetItemBounding( int nIndex ) const;

		public:
			virtual void ClearSelect() = 0;
			virtual bool IsSelect( int nIndex ) const = 0;
			virtual int GetSelect( int nOffset = 0 ) const = 0;
			virtual bool IsItemEnable( int nIndex ) const = 0;

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
			int m_nItemContextMenu;
			int m_nItemContextMenu_AtPointX;
			int m_nItemContextMenu_AtPointY;
			int m_nItemDragging;
			CRenderElement* m_pRenderElementItemsRoot;
			CRenderElement* m_pRenderElementDrag;

			EListBoxDragStyle m_nDragStyle;
			bool m_bPressed;
			bool m_bIsDragging;
			int m_nDragX;
			int m_nDragY;
			int m_nDragScrollSense;

			UListBoxDirection m_nItemDirection;
			int  m_nItemWidth;
			int  m_nItemHeight;
			int  m_nItemSpacing;
			int  m_nItemVirtualSize;
			int  m_nShowX;
			int  m_nShowY;

			std::function<UCallback_ItemElementAllocate> m_Callback_ItemElementAllocate;
			std::function<UCallback_ItemElementRelease> m_Callback_ItemElementRelease;
		};



		


		template < typename TData >
		class TListBox
			: public CListBoxNoData
		{
		protected:
			typedef typename UListBoxItemNode<TData> _ItemNode;
		public:
			typedef void UCallback_ItemElementCreate( CRenderElement*, const TData* pData, int nIndex );
			typedef void UCallback_ItemElementUpdate( CRenderElement*, const TData* pData, int nIndex, int nUpdateEvent );

		public:
			TListBox();


		public:
			virtual void Update( float nDelta ) override;
			virtual void ClearSelect() override;
			virtual bool IsSelect( int nIndex ) const override;
			virtual int GetSelect( int nOffset = 0 ) const override;
			virtual bool IsItemEnable( int nIndex ) const override;

		protected:
			virtual void SetSelectInternal( int nIndex, bool bSelected, bool bInternal ) override;
			virtual void OnSetItemEnable( int nIndex, bool nEnable );
			virtual void OnSetItemVisible( int nIndex, bool bVisible );

		public:
			void SetCallback_ItemElementCreate( std::function<UCallback_ItemElementCreate> Callback ) { m_Callback_ItemElementCreate = Callback; }
			void SetCallback_ItemElementUpdate( std::function<UCallback_ItemElementUpdate> Callback ) { m_Callback_ItemElementUpdate = Callback; }
			int AddItem( const TData& Data );
			void UpdateItem( int nIndex, const TData& Data );
			void UpdateItem( int nIndex, std::function<void( TData* )> Func, int nUpdateEvent );
			int RemoveItemOne( int nIndex );
			int RemoveItems( const int* pIndices, int nCount );
			void RemoveItemsAll();
			int GetItemCount() const { return (int)m_Items.size(); }
			const TData* GetItem( int nIndex );
			int FindItem( std::function<bool( const TData* )>, int nOffset = 0 );
			void SetItemEnable( int nIndex, bool bEnable );
			
			

		protected:
			int m_nSelMaxCount;
			int m_nSel;
			std::vector<_ItemNode> m_Items;
			std::function<UCallback_ItemElementCreate> m_Callback_ItemElementCreate;
			std::function<UCallback_ItemElementUpdate> m_Callback_ItemElementUpdate;
		};


		typedef TListBox<CVariantDictionary> CListBoxDictionary;
	}
}






template < typename TData >
iberbar::Gui::TListBox<TData>::TListBox()
	: m_nSelMaxCount( -1 )
	, m_nSel( -1 )
	, m_Items()
	, m_Callback_ItemElementCreate()
	, m_Callback_ItemElementUpdate()
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
bool iberbar::Gui::TListBox<TData>::IsItemEnable( int nIndex ) const
{
	if ( nIndex < 0 || nIndex >= (int)m_Items.size() )
		return false;

	return m_Items[nIndex].bEnable;
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
void iberbar::Gui::TListBox<TData>::OnSetItemEnable( int nIndex, bool bEnable )
{
	CRenderElement* pElement = m_pRenderElementItemsRoot->GetElement( nIndex );
	if ( pElement == nullptr )
		return;
	if ( bEnable == false )
	{
		pElement->SetState( (int)UWidgetState::Disabled );
	}
	else
	{
		pElement->SetState( (int)UWidgetState::Normal );
	}
}


template < typename TData >
void iberbar::Gui::TListBox<TData>::OnSetItemVisible( int nIndex, bool bVisible )
{
	CRenderElement* pElement = m_pRenderElementItemsRoot->GetElement( nIndex );
	if ( pElement == nullptr )
		return;
	if ( bVisible == false )
	{
		pElement->SetVisible( bVisible );
	}
	else
	{
		pElement->SetVisible( bVisible );
		pElement->Refresh();
	}

	ResizeItemElements( nIndex );
}


template < typename TData >
int iberbar::Gui::TListBox<TData>::AddItem( const TData& Data )
{
	if ( m_pRenderElementItemsRoot == nullptr )
		return -1;

	PTR_CRenderElement pElement;
	if ( m_Callback_ItemElementAllocate )
	{
		pElement.attach( m_Callback_ItemElementAllocate() );
		if ( pElement == nullptr )
			return -1;
	}
	else
	{
		pElement = PTR_CRenderElement::_sNew();
	}

	_ItemNode item;
	item.Selected = false;
	item.bEnable = true;
	item.Data = Data;
	m_Items.push_back( item );

	int nIndex = (int)m_Items.size() - 1;

	m_pRenderElementItemsRoot->AddChildElement( pElement );
	ResizeItemElements( nIndex, 1 );

	if ( m_Callback_ItemElementCreate )
		m_Callback_ItemElementCreate( pElement, &Data, nIndex );

	pElement->SetState( (int)UWidgetState::Normal );
	pElement->Refresh();

	return nIndex;
}


template < typename TData >
void iberbar::Gui::TListBox<TData>::UpdateItem( int nIndex, const TData& Data )
{
	if ( nIndex < 0 || nIndex >= (int)m_Items.size() )
		return;

	_ItemNode* pItem = &m_Items[nIndex];
	CRenderElement* pElement = m_pRenderElementItemsRoot->GetElement( nIndex );

	pItem->Data = Data;

	if ( m_Callback_ItemElementUpdate )
		m_Callback_ItemElementUpdate( pElement, &pItem->Data, nIndex, 0 );
}


template < typename TData >
void iberbar::Gui::TListBox<TData>::UpdateItem( int nIndex, std::function<void( TData* )> Func, int nUpdateEvent )
{
	if ( nIndex < 0 || nIndex >= (int)m_Items.size() )
		return;

	_ItemNode* pItem = &m_Items[nIndex];
	CRenderElement* pElement = m_pRenderElementItemsRoot->GetElement( nIndex );

	if ( Func )
		Func( &pItem->Data );

	if ( m_Callback_ItemElementUpdate )
		m_Callback_ItemElementUpdate( pElement, &pItem->Data, nIndex, nUpdateEvent );
}


template < typename TData >
int iberbar::Gui::TListBox<TData>::RemoveItemOne( int nIndex )
{
	if ( nIndex < 0 || nIndex >= (int)m_Items.size() )
		return 0;

	m_Items.erase( m_Items.begin() + nIndex );

	// 自定义释放方法
	if ( m_Callback_ItemElementRelease )
	{
		PTR_CRenderElement pElementTemp;
		m_pRenderElementItemsRoot->GetChildElement( nIndex, &pElementTemp );
		if ( pElementTemp )
		{
			m_Callback_ItemElementRelease( pElementTemp );
		}
	}

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


template < typename TData >
void iberbar::Gui::TListBox<TData>::SetItemEnable( int nIndex, bool bEnable )
{
	if ( nIndex < 0 || nIndex >= (int)m_Items.size() )
		return;

	m_Items[nIndex].bEnable = bEnable;
	CRenderElement* pElement = m_pRenderElementItemsRoot->GetElement( nIndex );
	if ( pElement )
	{
		if ( bEnable == true )
		{
			pElement->SetState( (int)UWidgetState::Normal );
			pElement->Refresh();
		}
		else
		{
			pElement->SetState( (int)UWidgetState::Disabled );
		}
	}
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

