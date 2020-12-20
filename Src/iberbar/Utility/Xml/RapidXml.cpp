
#include <iberbar/Utility/Xml/RapidXml.h>
#include <iberbar/Utility/String.h>

#include <iostream>
#include <rapidxml.hpp>
#include <rapidxml_print.hpp>
#include <rapidxml_utils.hpp>



namespace iberbar
{
	namespace Xml
	{
		typedef rapidxml::xml_node< char >      RapidNodeContextA;
		typedef rapidxml::file< char >          RapidFileContextA;
		typedef rapidxml::xml_document< char >  RapidDocumentContextA;

		class CRapidDocumentA : public CDocumentA
		{
		public:
			CRapidDocumentA();
			virtual ~CRapidDocumentA();

		public:
			virtual CResult Create( const char* rootnode ) override;
			virtual CResult LoadFromFile( const char* filepath ) override;
			virtual CResult Load( const char* source ) override;
			virtual CResult Save( const char* filepath ) override;
			virtual void GetRoot( CNodeA** ppRootNode ) override;

		private:
			RapidFileContextA* m_file;
			RapidDocumentContextA* m_doc;
		};

		class CRapidNodeA : public CNodeA
		{
		public:
			CRapidNodeA( CRapidDocumentA* document, RapidNodeContextA* nodeContext );
			virtual ~CRapidNodeA();

			virtual void SelectSingleNode( const char* tagName, CNodeA** ppNode ) override;
			virtual void SelectNodes( const char* tagName, CNodeListA** ppNodeList ) override;

			virtual void AppendChild( const char* tagName, CNodeA** ppNode ) override;
			virtual void DeleteChild( CNodeA* pNode ) override;

			virtual void SetAttribute( const char* attrName, const char* attrValue ) override;
			virtual const char* GetAttribute( const char* attrName ) override;

			virtual void SetValueText( const char* value ) override;
			virtual const char* GetValueText() override;

			virtual const char* NameText() override;

		private:
			CRapidDocumentA* m_document;
			RapidNodeContextA* m_nodeContext;
		};

		class CRapidNodeListA : public CNodeListA
		{
		public:
			CRapidNodeListA( CRapidDocumentA* document );
			virtual ~CRapidNodeListA();

		public:
			virtual void GetNodeAt( int index, CNodeA** ppNode ) override;
			virtual int  GetNodeCount() override;

			void AddNode( CRapidNodeA* pNode );

		private:
			CRapidDocumentA* m_document;
			std::vector< CRapidNodeA* > m_nodes;
		};
	}
}




iberbar::Xml::PTR_CDocumentA iberbar::Xml::CreateRapidXmlDocumentA()
{
	PTR_CDocumentA document;
	document.attach( new CRapidDocumentA() );
	return document;
}




iberbar::Xml::CRapidDocumentA::CRapidDocumentA()
	: CDocumentA()
	, m_file( nullptr )
	, m_doc( nullptr )
{
}

iberbar::Xml::CRapidDocumentA::~CRapidDocumentA()
{
	SAFE_DELETE( m_file );
	SAFE_DELETE( m_doc );
}

iberbar::CResult iberbar::Xml::CRapidDocumentA::Create( const char* rootNodeName )
{
	return CResult();
}

iberbar::CResult iberbar::Xml::CRapidDocumentA::LoadFromFile( const char* filepath )
{
	try
	{
		m_file = new RapidFileContextA( filepath );
		m_doc = new RapidDocumentContextA();
		m_doc->parse<0>( m_file->data() );
	}
	catch ( std::exception exception )
	{
		return MakeResult( ResultCode::Bad, "Exception of parsing: %s", exception.what() );
	}
	return CResult();
}

iberbar::CResult iberbar::Xml::CRapidDocumentA::Load( const char* source )
{
	return CResult( ResultCode::Bad, "NotImplements" );
}

iberbar::CResult iberbar::Xml::CRapidDocumentA::Save( const char* filepath )
{
	return CResult( ResultCode::Bad, "NotImplements" );
}

void iberbar::Xml::CRapidDocumentA::GetRoot( CNodeA** ppRootNode )
{
	RapidNodeContextA* node_context = this->m_doc->first_node();

	PTR_CNodeA node;
	if ( node_context )
		node.attach( new CRapidNodeA( this, node_context ) );

	if ( ppRootNode )
	{
		if ( *ppRootNode )
			(*ppRootNode)->Release();
		(*ppRootNode) = node;
		if ( *ppRootNode )
			(*ppRootNode)->AddRef();
	}
}






iberbar::Xml::CRapidNodeA::CRapidNodeA( CRapidDocumentA* document, RapidNodeContextA* nodeContext )
	: CNodeA()
	, m_document( document )
	, m_nodeContext( nodeContext )
{
	this->m_document->AddRef();
}


iberbar::Xml::CRapidNodeA::~CRapidNodeA()
{
	UNKNOWN_SAFE_RELEASE_NULL( this->m_document );
}

void iberbar::Xml::CRapidNodeA::SelectSingleNode( const char* tagName, CNodeA** ppNode )
{
	assert( StringIsNullOrEmpty( tagName ) == false );

	RapidNodeContextA* nodeContext = m_nodeContext->first_node( tagName );

	PTR_CNodeA node = nullptr;
	if ( nodeContext != nullptr )
	{
		node.attach( new CRapidNodeA( this->m_document, nodeContext ) );
	}

	if ( ppNode )
	{
		if ( *ppNode )
			(*ppNode)->Release();
		(*ppNode) = node;
		if ( *ppNode )
			(*ppNode)->AddRef();
	}
}

void iberbar::Xml::CRapidNodeA::SelectNodes( const char* tagName, CNodeListA** ppNodeList )
{
	assert( StringIsNullOrEmpty( tagName ) == false );

	if ( ppNodeList )
		UNKNOWN_SAFE_RELEASE_NULL( *ppNodeList );

	RapidNodeContextA* nodeContext = m_nodeContext->first_node( tagName );
	if ( nodeContext != nullptr )
	{
		PTR_CNodeListA nodelist;
		nodelist.attach( new CRapidNodeListA( this->m_document ) );

		CRapidNodeListA* nodelist_ptr = (CRapidNodeListA*)((CNodeListA*)nodelist);

		while ( nodeContext )
		{
			nodelist_ptr->AddNode( new CRapidNodeA( this->m_document, nodeContext ) );
			nodeContext = nodeContext->next_sibling( tagName );
		}

		if ( ppNodeList )
		{
			(*ppNodeList) = nodelist;
			(*ppNodeList)->AddRef();
		}
	}
}

void iberbar::Xml::CRapidNodeA::AppendChild( const char* tagName, CNodeA** ppNode )
{
}

void iberbar::Xml::CRapidNodeA::DeleteChild( CNodeA* node )
{
}

void iberbar::Xml::CRapidNodeA::SetAttribute( const char* attrName, const char* attrValue )
{
}

const char* iberbar::Xml::CRapidNodeA::GetAttribute( const char* attrName )
{
	rapidxml::xml_attribute< char >* attr_context = m_nodeContext->first_attribute( attrName );
	if ( attr_context == nullptr )
		return nullptr;
	return attr_context->value();
}

void iberbar::Xml::CRapidNodeA::SetValueText( const char* value )
{
}

const char* iberbar::Xml::CRapidNodeA::GetValueText()
{
	return m_nodeContext->value();
}

const char* iberbar::Xml::CRapidNodeA::NameText()
{
	return m_nodeContext->name();
}





iberbar::Xml::CRapidNodeListA::CRapidNodeListA( CRapidDocumentA* document )
	: CNodeListA()
	, m_document( document )
{
	if ( m_document )
		m_document->AddRef();
}

iberbar::Xml::CRapidNodeListA::~CRapidNodeListA()
{
	auto iter = m_nodes.begin();
	auto end = m_nodes.end();
	for ( ; iter != end; iter++ )
		UNKNOWN_SAFE_RELEASE_NULL( *iter );
	UNKNOWN_SAFE_RELEASE_NULL( m_document );
}

void iberbar::Xml::CRapidNodeListA::GetNodeAt( int index, CNodeA** ppNode )
{
	assert( index >= 0 && index < this->GetNodeCount() );
	assert( ppNode );
	if ( *ppNode )
		(*ppNode)->Release();
	(*ppNode) = m_nodes[index];
	(*ppNode)->AddRef();
}

int iberbar::Xml::CRapidNodeListA::GetNodeCount()
{
	return (int)m_nodes.size();
}

void iberbar::Xml::CRapidNodeListA::AddNode( CRapidNodeA* pNode )
{
	m_nodes.push_back( pNode );
}


