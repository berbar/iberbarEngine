
#include <iberbar\Utility\Transform.h>


iberbar::CTransform2D::CTransform2D( void )
	: CRef()
	, m_bPercentX( false )
	, m_bPercentY( false )
	, m_bPercentW( false )
	, m_bPercentH( false )
	, m_alignStyle( UAlignStyle::Inner )
	, m_alignHorizental( UAlignHorizental::Left )
	, m_alignVertical( UAlignVertical::Top )
	, m_Position()
	, m_Size()
	, m_ParentTransform( nullptr )
	, m_rcBounding( 0, 0, 0, 0 )
	, m_ChildTransformArray()
{
}



iberbar::CTransform2D::CTransform2D(const CTransform2D& trans )
	: CRef()
	, m_bPercentX( trans.m_bPercentX )
	, m_bPercentY( trans.m_bPercentY )
	, m_bPercentW( trans.m_bPercentW )
	, m_bPercentH( trans.m_bPercentH )
	, m_alignHorizental( trans.m_alignHorizental )
	, m_alignVertical( trans.m_alignVertical )
	, m_Position( trans.m_Position )
	, m_Size( trans.m_Size )
	, m_alignStyle( trans.m_alignStyle )
	, m_ParentTransform( nullptr )
	, m_rcBounding( 0, 0, 0, 0 )
	, m_ChildTransformArray()
{
}



iberbar::CTransform2D::~CTransform2D()
{
	if ( m_ChildTransformArray.empty() == false )
	{
		Array_Transfrom2D::iterator lc_iter = m_ChildTransformArray.begin();
		Array_Transfrom2D::iterator lc_end  = m_ChildTransformArray.end();
		for ( ; lc_iter != lc_end; lc_iter ++ )
		{
			(*lc_iter)->m_ParentTransform = nullptr;
		}
		m_ChildTransformArray.clear();
	}
}



void iberbar::CTransform2D::UpdateTransform()
{
	if ( m_ParentTransform )
	{
		const CRect2i& rectParent = m_ParentTransform->GetBounding();
		CPoint2i position = m_Position;
		CSize2i size = m_Size;
		if ( m_bPercentX )
			position.x = rectParent.Width() * m_Position.x / 100;
		if ( m_bPercentY )
			position.y = rectParent.Height() * m_Position.y / 100;
		if ( m_bPercentW )
			size.w = rectParent.Width() * size.w / 100;
		if ( m_bPercentH )
			size.h = rectParent.Height() * size.h / 100;

		if ( m_alignStyle == UAlignStyle::Inner )
			RectAlignWith( &m_rcBounding, position, size, m_alignHorizental, m_alignVertical, &rectParent );
		else
			RectAlignWith( &m_rcBounding, position, size, m_alignHorizental, m_alignVertical, &rectParent );
	}
	else
	{
		m_rcBounding = CRect2i( m_Position, m_Size );
	}

	if ( m_ChildTransformArray.empty() == false )
	{
		Array_Transfrom2D::iterator lc_iter = m_ChildTransformArray.begin();
		Array_Transfrom2D::iterator lc_end  = m_ChildTransformArray.end();
		for ( ; lc_iter != lc_end; lc_iter ++ )
		{
			(*lc_iter)->UpdateTransform();
		}
	}
}

iberbar::CTransform2D* iberbar::CTransform2D::Clone() const
{
	CTransform2D* transNew = new CTransform2D( *this );
	transNew->SetParentTransform( this->GetParentTransform() );
	transNew->UpdateTransform();
	return transNew;
}


void iberbar::CTransform2D::SetParentTransform( CTransform2D* parent )
{
	if ( parent == m_ParentTransform )
		return ;

	if ( m_ParentTransform )
	{
		Array_Transfrom2D::iterator lc_iter = m_ParentTransform->m_ChildTransformArray.begin();
		Array_Transfrom2D::iterator lc_end  = m_ParentTransform->m_ChildTransformArray.end();
		for ( ; lc_iter != lc_end; lc_iter ++ )
		{
			if ( this == ( *lc_iter ) )
			{
				m_ParentTransform->m_ChildTransformArray.erase( lc_iter );
				break;
			}
		}
	}

	m_ParentTransform = parent;

	if ( m_ParentTransform )
	{
		m_ParentTransform->m_ChildTransformArray.push_back( this );
	}

	// 暂时使用update来改变bounding，不从bounding获得position
	UpdateTransform();
}

