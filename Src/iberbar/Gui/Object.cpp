
#include <iberbar/Gui/Object.h>





iberbar::Gui::CObject::CObject()
	: m_nId( "" )
	, m_strName( "" )
	, m_bVisible( true )
	, m_bEnable( true )
	, m_pTransform( PTR_CTransform2D::_sNew() )
{
}


iberbar::Gui::CObject::CObject( const CObject& obj )
	: m_nId( obj.m_nId )
	, m_strName( obj.m_strName )
	, m_bVisible( obj.m_bVisible )
	, m_bEnable( obj.m_bEnable )
	, m_pTransform( PTR_CTransform2D::_sNew() )
{
	m_pTransform->UpdateTransform();
}


iberbar::Gui::CObject::~CObject()
{
	m_pTransform->SetParentTransform( nullptr );
}


void iberbar::Gui::CObject::SetVisible( bool bVisible /* = true */ )
{
	if ( m_bVisible != bVisible )
	{
		m_bVisible = bVisible;
		if ( m_bVisible )
			OnShow();
		else
			OnHide();
	}
}


void iberbar::Gui::CObject::SetEnable( bool bEnable /* = true */ )
{
	if ( m_bEnable != bEnable )
	{
		m_bEnable = bEnable;
		if ( m_bEnable )
			OnEnable();
		else
			OnDisable();
	}
}


bool iberbar::Gui::CObject::HitTest( const CPoint2i& point ) const
{
	return m_pTransform->HitTest( point );
}