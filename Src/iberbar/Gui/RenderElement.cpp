
#include <iberbar/Gui/RenderElement.h>
#include <iberbar/Gui/Engine.h>
#include <iberbar/Renderer/Renderer.h>
#include <iberbar/Renderer/RendererSprite.h>
#include <iberbar/RHI/ShaderState.h>




iberbar::Gui::CRenderElement::CRenderElement( void )
	: m_nZOrder( 0 )
	, m_pShaderState( nullptr )
	, m_pShaderVariableTables()
	, m_ChildElements()
	, m_nState( 0 )
{
}

iberbar::Gui::CRenderElement::CRenderElement( const CRenderElement& elem )
	: CObject( elem )
	, m_nZOrder( elem.m_nZOrder )
	, m_pShaderState( nullptr )
	, m_pShaderVariableTables()
	, m_ChildElements()
	, m_nState( 0 )
{
}

iberbar::Gui::CRenderElement::~CRenderElement()
{
	UNKNOWN_SAFE_RELEASE_NULL( m_pShaderState );
	m_ChildElements.clear();
}

iberbar::Gui::CRenderElement* iberbar::Gui::CRenderElement::Clone() const
{
	return new CRenderElement( *this );
}

int iberbar::Gui::CRenderElement::AddChildElement( CRenderElement* pElement )
{
	assert( pElement != this );

	if ( pElement == nullptr )
		return -1;

	pElement->GetTransform()->SetParentTransform( this->GetTransform() );
	m_ChildElements.push_back( pElement );

	return (int)(m_ChildElements.size() - 1);
}

int iberbar::Gui::CRenderElement::AddChildElement( int index, CRenderElement* pElement )
{
	assert( pElement != this );

	if ( pElement == nullptr )
		return -1;

	if ( index < 0 || index >= (int)m_ChildElements.size() )
		return -1;

	pElement->GetTransform()->SetParentTransform( this->GetTransform() );
	m_ChildElements.insert( m_ChildElements.begin() + index, pElement );

	return index;
}

bool iberbar::Gui::CRenderElement::FindElement( const char* strName, CRenderElement** ppOutElement )
{
	for ( size_t i = 0, s = m_ChildElements.size(); i < s; i++ )
	{
		if ( strcmp( m_ChildElements[ i ]->GetId().c_str(), strName ) == 0 )
		{
			if ( ppOutElement )
			{
				if ( *ppOutElement )
					(*ppOutElement)->Release();
				(*ppOutElement) = m_ChildElements[ i ];
				if ( *ppOutElement )
					(*ppOutElement)->AddRef();
			}
			return true;
		}

		if ( m_ChildElements[ i ]->FindElement( strName, ppOutElement ) == true )
			return true;
	}

	return false;
}

int iberbar::Gui::CRenderElement::ReplaceChildElement( int index, CRenderElement* pNew, CRenderElement** ppOld )
{
	assert( pNew != this );

	if ( index < 0 || index >= (int)m_ChildElements.size() )
		return -1;

	if ( ppOld )
	{
		UNKNOWN_SAFE_RELEASE_NULL( *ppOld );
		*ppOld = m_ChildElements[ index ];
		UNKNOWN_SAFE_ADDREF( *ppOld );
		(*ppOld)->GetTransform()->SetParentTransform( nullptr );
	}

	PTR_CRenderElement lc_pElem;
	if ( pNew == nullptr )
		lc_pElem = PTR_CGuiElemBlank::_sNew();
	else
		lc_pElem = pNew;

	lc_pElem->GetTransform()->SetParentTransform( this->GetTransform() );
	m_ChildElements[ index ] = lc_pElem;

	return index;
}

int iberbar::Gui::CRenderElement::GetChildElementCount() const
{
	if ( m_ChildElements.empty() )
		return 0;
	return (int)m_ChildElements.size();
}

void iberbar::Gui::CRenderElement::RemoveChildElement( int index )
{
	if ( index < 0 || index >= GetChildElementCount() )
		return;

	m_ChildElements.erase( m_ChildElements.begin() + index );
}

void iberbar::Gui::CRenderElement::RemoveChildElementsAll()
{
	m_ChildElements.clear();
}

bool iberbar::Gui::CRenderElement::GetChildElement( int index, CRenderElement** ppElement ) const
{
	if ( index < 0 || (size_t)index >= m_ChildElements.size() )
		return false;

	if ( ppElement )
	{
		UNKNOWN_SAFE_RELEASE_NULL( *ppElement );
		*ppElement = m_ChildElements[ index ];
		UNKNOWN_SAFE_ADDREF( *ppElement );
	}

	return true;
}

void iberbar::Gui::CRenderElement::CopyChildElementsFormOther( CRenderElement* pSrcRoot )
{
	m_ChildElements.clear();

	if ( pSrcRoot->m_ChildElements.empty() == false )
	{
		size_t lc_count = pSrcRoot->m_ChildElements.size();
		for ( size_t lc_i = 0; lc_i < lc_count; lc_i++ )
		{
			AddChildElement( PTR_CRenderElement::_sClone( pSrcRoot->m_ChildElements[ lc_i ] ) );
		}
	}
}

void iberbar::Gui::CRenderElement::SetState( int nState )
{
	if ( m_nState == nState )
		return;

	m_nState = nState;

	if ( m_ChildElements.empty() == true )
		return;

	for ( uint32 lc_i = 0; lc_i < m_ChildElements.size(); lc_i++ )
	{
		m_ChildElements[ lc_i ]->SetState( nState );
	}
}


void iberbar::Gui::CRenderElement::Init()
{
	CEngine::sGetInstance()->GetRendererSprite()->GetDefaultShaderState( &m_pShaderState );
	CEngine::sGetInstance()->GetRendererSprite()->CreateDefaultShaderVariableTableUnion( &m_pShaderVariableTables );
	m_pShaderVariableTables.GetVariableTableForVertexShader()->SetBool( Renderer::s_strShaderVarName_RHW, true );
}


void iberbar::Gui::CRenderElement::UpdateRect()
{
	if ( m_ChildElements.empty() == true )
		return;

	for ( uint32 lc_i = 0; lc_i < m_ChildElements.size(); lc_i++ )
	{
		m_ChildElements[ lc_i ]->UpdateRect();
	}
}

void iberbar::Gui::CRenderElement::Update( float nElapsedTime )
{
	if ( m_ChildElements.empty() == true )
		return;

	for ( uint32 lc_i = 0; lc_i < m_ChildElements.size(); lc_i++ )
	{
		m_ChildElements[ lc_i ]->Update( nElapsedTime );
	}
}

void iberbar::Gui::CRenderElement::Refresh()
{
	if ( m_ChildElements.empty() == true )
		return;

	for ( uint32 lc_i = 0; lc_i < m_ChildElements.size(); lc_i++ )
	{
		m_ChildElements[ lc_i ]->Refresh();
	}
}

void iberbar::Gui::CRenderElement::Render()
{
	if ( m_ChildElements.empty() == true )
		return;

	for ( int i = 0, s = (int)m_ChildElements.size(); i < s; i++ )
	{
		if ( m_ChildElements[i]->GetVisible() == false )
			continue;
		m_ChildElements[ i ]->Render();
	}
}


iberbar::Gui::CRenderElement* iberbar::Gui::CRenderElement::sCloneRenderElementRecurse( CRenderElement* pElementSrc )
{
	if ( pElementSrc == nullptr )
		return nullptr;

	CRenderElement* pElementNew = pElementSrc->Clone();
	if ( pElementNew == nullptr )
		return nullptr;

	if ( pElementSrc->m_ChildElements.empty() == false )
	{
		for ( auto pElementChild : pElementSrc->m_ChildElements )
		{
			pElementNew->AddChildElement( sCloneRenderElementRecurse( pElementChild ) );
		}
	}

	pElementNew->Init();

	return pElementNew;
}



