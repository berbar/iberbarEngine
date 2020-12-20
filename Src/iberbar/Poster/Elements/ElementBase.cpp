
#include <iberbar/Poster/Elements/ElementBase.h>


iberbar::Poster::CElementBase::CElementBase()
    : m_position( 0, 0 )
    , m_nDataSize( 0, 0 )
    , m_alignStyle( UAlignStyle::Inner )
    , m_alignHorizental( UAlignHorizental::Left )
    , m_alignVertical( UAlignVertical::Top )
    , m_parentElement( nullptr )
    , m_subElements()
{
}

//iberbar::Poster::CElementBase* iberbar::Poster::CElementBase::Clone() const
//{
//    CElementBase* element = new CElementBase( this );
//}

bool iberbar::Poster::CElementBase::AddSubElement( CElementBase* element )
{
    if ( element->m_parentElement != nullptr )
        return false;
    m_subElements.push_back( element );
    element->m_parentElement = this;
    return true;
}

void iberbar::Poster::CElementBase::UpdateBounding()
{
    if ( m_parentElement == nullptr )
    {
        m_bounding = CRect2i( m_position, m_nDataSize );
    }
    else
    {
        RectAlignWith( &m_bounding, m_position, m_nDataSize, m_alignHorizental, m_alignVertical, &m_parentElement->m_bounding );
    }
}

void iberbar::Poster::CElementBase::Render( CSurface* target )
{
    UpdateBounding();
    RenderSelf( target );
    RenderSubElements( target );
}

void iberbar::Poster::CElementBase::RenderSubElements( CSurface* target )
{
    auto iter = m_subElements.begin();
    auto end = m_subElements.end();
    for ( ; iter != end; iter++ )
    {
        (*iter)->Render( target );
    }
}
