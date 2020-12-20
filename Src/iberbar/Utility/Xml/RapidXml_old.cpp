


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
		typedef rapidxml::xml_node< TCHAR >     RapidContextNode;
		typedef rapidxml::file< CHAR >          RapidXmlFileContext;
		typedef rapidxml::xml_document< TCHAR > RapidXmlDocumentContext;

		class CRapidXmlDocument;
		class CRapidXmlNode;
		class CRapidXmlNodeList;

		class CRapidXmlDocument
			: public CDocument
		{
		public:

			CRapidXmlDocument();
			virtual ~CRapidXmlDocument();

		public:
			virtual CResult create( const TCHAR* rootnode );
			virtual CResult loadFromFile( const TCHAR* filename );
			virtual CResult loadFromString( const TCHAR* source );
			virtual CResult save( const TCHAR* filename );
			virtual void   close();

			virtual CResult getRoot( CNode** ppRootNode );

		protected:
#ifdef UNICODE
			PTR_PointerCharW m_pData;
#endif
			RapidXmlFileContext*      m_file;
			RapidXmlDocumentContext*  m_doc;
		};


		class CRapidXmlNode
			: public CNode
		{
		public:
			CRapidXmlNode( CRapidXmlDocument* parent, RapidContextNode* node );
			virtual ~CRapidXmlNode() override;

		public:
			virtual CResult selectSimgleNode( const TCHAR* pzTagName, CNode** ppNode ) override;
			virtual CResult selectNodes( const TCHAR* pzTagName, CNodeList** ppNodeList ) override;
			virtual CResult appendChild( const TCHAR* pzTagName, CNode** ppNode ) override;
			virtual CResult deleteChild( CNode* pNode ) override;
			virtual CResult       setAttribute( const TCHAR* pzAttrName, const TCHAR* pzAttrValue ) override;
			virtual const TCHAR* getAttribute( const TCHAR* pzAttrName ) override;
			virtual CResult       setValueText( const TCHAR* value ) override;
			virtual const TCHAR* getValueText() override;
			virtual const TCHAR* nameText() override;

		private:
			CRapidXmlDocument* m_document;
			RapidContextNode* m_node_context;
		};



		class CRapidXmlNodeList
			: public CNodeList
		{
		public:
			CRapidXmlNodeList( CRapidXmlDocument* document );
			virtual ~CRapidXmlNodeList() override;

		public:
			virtual void getNodeAt( int nIndex, CNode** ppNode ) override;
			virtual int  getNodeCount() override;

			void addNode( CRapidXmlNode* pNode );

		private:
			CRapidXmlDocument * m_document;
			std::vector< CRapidXmlNode* > m_nodes;
		};
	}
}


iberbar::Xml::CRapidXmlDocument::CRapidXmlDocument()
	: CDocument()
	, m_file( nullptr )
	, m_doc( nullptr )
{
}


iberbar::Xml::CRapidXmlDocument::~CRapidXmlDocument()
{
	SAFE_DELETE( this->m_file );
	SAFE_DELETE( this->m_doc );
}

iberbar::CResult iberbar::Xml::CRapidXmlDocument::create( const TCHAR* rootnode )
{
	return CResult();
}

iberbar::CResult iberbar::Xml::CRapidXmlDocument::loadFromFile( const TCHAR* filepath )
{
	assert( m_file == nullptr );
	assert( m_doc == nullptr );

	assert( filepath && filepath[ 0 ] != 0 );

	try
	{
#ifdef UNICODE
		PTR_PointerCharA filepath_0 = UnicodeToUtf8_Pointer( filepath );
		m_file = new RapidXmlFileContext( filepath_0->getPointerConst() );
		m_doc = new RapidXmlDocumentContext();
		m_pData = Utf8ToUnicode_Pointer( m_file->data() );
		m_doc->parse<0>( m_pData->GetPointer() );
#else
		m_file = new RapidXmlFileContext( filepath );
		m_doc = new RapidXmlDocumentContext();
		m_doc->parse<0>( m_file->data() );
#endif
	}
	catch ( std::exception exc )
	{
		CResult result;
		result.code = ResultCode::Bad;
		result.Format( "Exception of parsing: %s", exc.what() );
		return result;
	}

	return CResult();
}

iberbar::CResult iberbar::Xml::CRapidXmlDocument::loadFromString( const TCHAR* )
{
	return CResult();
}

iberbar::CResult iberbar::Xml::CRapidXmlDocument::save( const TCHAR* filepath )
{
	return CResult();
}

void iberbar::Xml::CRapidXmlDocument::close()
{

}

iberbar::CResult iberbar::Xml::CRapidXmlDocument::getRoot( iberbar::Xml::CNode** ppNode )
{
	CResult result;

	RapidContextNode* node_context = this->m_doc->first_node();

	PTR_CNode node;
	if ( node_context )
		node.attach( new CRapidXmlNode( this, node_context ) );
	else
		result = CResult( ResultCode::Bad, "" );

	if ( ppNode )
	{
		if ( *ppNode )
			(*ppNode)->release();
		(*ppNode) = node;
		if ( *ppNode )
			(*ppNode)->addRef();
	}

	return result;
}





iberbar::Xml::CRapidXmlNode::CRapidXmlNode( CRapidXmlDocument* parent, RapidContextNode* node_context )
	: CNode()
	, m_document( parent )
	, m_node_context( node_context )
{
	this->m_document->addRef();
}

iberbar::Xml::CRapidXmlNode::~CRapidXmlNode()
{
	UNKNOWN_SAFE_RELEASE_NULL( this->m_document );
}

iberbar::CResult iberbar::Xml::CRapidXmlNode::selectSimgleNode( const TCHAR* tag, CNode** ppNode )
{
	assert( tag && tag[ 0 ] != 0 );

	CResult result;

	RapidContextNode* node_context = this->m_node_context->first_node( tag );

	PTR_CNode node = nullptr;
	if ( node_context )
		node.attach( new CRapidXmlNode( this->m_document, node_context ) );
	else
		result = CResult( ResultCode::Bad, "" );


	if ( ppNode )
	{
		if ( *ppNode )
			(*ppNode)->release();
		(*ppNode) = node;
		if ( *ppNode )
			(*ppNode)->addRef();
	}

	return result;
}

iberbar::CResult iberbar::Xml::CRapidXmlNode::selectNodes( const TCHAR* tag, CNodeList** ppNodeList )
{
	assert( tag && tag[ 0 ] != 0 );

	if ( ppNodeList )
		UNKNOWN_SAFE_RELEASE_NULL( *ppNodeList );

	RapidContextNode* node_context = this->m_node_context->first_node( tag );
	if ( node_context == nullptr )
		return CResult( ResultCode::Bad, "" );

	PTR_CNodeList nodelist;
	nodelist.attach( new CRapidXmlNodeList( this->m_document ) );

	CRapidXmlNodeList* nodelist_ptr = (CRapidXmlNodeList*)((CNodeList*)nodelist);
	
	while ( node_context )
	{
		nodelist_ptr->addNode( new CRapidXmlNode( this->m_document, node_context ) );
		node_context = node_context->next_sibling( tag );
	}

	if ( ppNodeList )
	{
		(*ppNodeList) = nodelist;
		(*ppNodeList)->addRef();
	}

	return CResult();
}

iberbar::CResult iberbar::Xml::CRapidXmlNode::appendChild( const TCHAR* tag, CNode** ppNode )
{
	return CResult();
}

iberbar::CResult iberbar::Xml::CRapidXmlNode::deleteChild( CNode* node )
{
	return CResult();
}

iberbar::CResult iberbar::Xml::CRapidXmlNode::setAttribute( const TCHAR* attrName, const TCHAR* attrValue )
{
	return CResult();
}

const TCHAR* iberbar::Xml::CRapidXmlNode::getAttribute( const TCHAR* attrName )
{
	rapidxml::xml_attribute< TCHAR >* attr_context = this->m_node_context->first_attribute( attrName );
	if ( attr_context == nullptr )
		return nullptr;
	return attr_context->value();
}

iberbar::CResult iberbar::Xml::CRapidXmlNode::setValueText( const TCHAR* value )
{
	return CResult();
}

const TCHAR* iberbar::Xml::CRapidXmlNode::getValueText()
{
	return nullptr;
}

const TCHAR* iberbar::Xml::CRapidXmlNode::nameText()
{
	return nullptr;
}






iberbar::Xml::CRapidXmlNodeList::CRapidXmlNodeList( CRapidXmlDocument* document )
	: CNodeList()
	, m_document( document )
	, m_nodes()
{
	this->m_document->addRef();
}

iberbar::Xml::CRapidXmlNodeList::~CRapidXmlNodeList()
{
	std::vector< CRapidXmlNode* >::iterator lc_iter = m_nodes.begin();
	std::vector< CRapidXmlNode* >::iterator lc_end  = m_nodes.end();
	for ( ; lc_iter != lc_end; lc_iter ++ )
		UNKNOWN_SAFE_RELEASE_NULL( *lc_iter );
	UNKNOWN_SAFE_RELEASE_NULL( this->m_document );
}

void iberbar::Xml::CRapidXmlNodeList::getNodeAt( int index, CNode** ppNode )
{
	assert( index >= 0 && index < this->getNodeCount() );
	(*ppNode) = m_nodes[ index ];
	(*ppNode)->addRef();
}

int iberbar::Xml::CRapidXmlNodeList::getNodeCount()
{
	return (int)this->m_nodes.size();
}

void iberbar::Xml::CRapidXmlNodeList::addNode( CRapidXmlNode* node )
{
	assert( node );
	this->m_nodes.push_back( node );
}




iberbar::Xml::PTR_CDocument iberbar::Xml::CreateRapidXmlDocument()
{
	PTR_CDocument doc;
	doc.attach( new CRapidXmlDocument() );
	return doc;
}



