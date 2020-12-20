
#include <iberbar/Poster/Elements/XmlEngine.h>
#include <iberbar/Poster/ResourcesManager.h>
#include <iberbar/Utility/Xml/RapidXml.h>

using namespace iberbar::Xml;

namespace iberbar
{
	namespace Poster
	{
		class CXmlReader
		{
		public:
			CXmlReader( CResourcesManager* resourcesManager, const CXmlReadProcFactory* readProcFactory, CXmlProcMessageBuffer* messages );

		public:
			CResult ReadFromString( const std::string& str, CElementBase** ppOutElementRoot );
			CResult ReadFromFile( const std::string& filepath, CElementBase** ppOutElementRoot );

		private:
			void ReadNode( CElementBase* elementParent, PTR_CNodeA node );
			void ReadList( CElementBase* elementParent, PTR_CNodeListA nodelist );
			void AddMessage( int code, const char* format, ... );

		private:
			CResourcesManager* m_resourcesManager;
			const CXmlReadProcFactory* m_readProcFactory;
			CXmlProcMessageBuffer* m_messages;
		};
	}
}


iberbar::Poster::CXmlEngine::CXmlEngine()
	: m_resourcesManager( nullptr )
{
}

iberbar::CResult iberbar::Poster::CXmlEngine::Read( const std::string& str, iberbar::Poster::CElementBase** ppOutElementRoot, CXmlProcMessageBuffer* pOutMessages )
{
	CXmlReader reader( m_resourcesManager, &m_readProcFactory, pOutMessages );
	return reader.ReadFromString( str, ppOutElementRoot );
}

iberbar::CResult iberbar::Poster::CXmlEngine::ReadFromFile( const std::string& filepath, iberbar::Poster::CElementBase** ppOutElementRoot, CXmlProcMessageBuffer* pOutMessages )
{
	CXmlReader reader( m_resourcesManager, &m_readProcFactory, pOutMessages );
	return reader.ReadFromFile( filepath, ppOutElementRoot );
}


iberbar::Poster::CXmlReader::CXmlReader( CResourcesManager* resourcesManager, const CXmlReadProcFactory* readProcFactory, CXmlProcMessageBuffer* messages )
	: m_resourcesManager( resourcesManager )
	, m_readProcFactory( readProcFactory )
	, m_messages( messages )
{
}

iberbar::CResult iberbar::Poster::CXmlReader::ReadFromString( const std::string& str, iberbar::Poster::CElementBase** ppOutElementRoot )
{
	return CResult();
}

iberbar::CResult iberbar::Poster::CXmlReader::ReadFromFile( const std::string& filepath, iberbar::Poster::CElementBase** ppOutElementRoot )
{
	CResult ret;

	PTR_CDocumentA document = CreateRapidXmlDocumentA();
	ret = document->LoadFromFile( filepath.c_str() );
	if ( ret.IsOK() == false )
		return ret;

	PTR_CNodeA nodeRoot;
	document->GetRoot( &nodeRoot );
	if ( nodeRoot == nullptr || strcmp( nodeRoot->NameText(), "Poster" ) != 0 )
		return CResult( ResultCode::Bad, "No node named <Poster> in the xml." );

	PTR_CNodeA nodeRender;
	nodeRoot->SelectSingleNode( "Render", &nodeRender );
	if ( nodeRender == nullptr )
	{
		return CResult( ResultCode::Bad, "No node named <Poster::Render> in the xml." );
	}

	(*ppOutElementRoot) = new CElementEmpty();

	PTR_CNodeListA nodeList_Elements;
	nodeRender->SelectNodes( "Element", &nodeList_Elements );
	if ( nodeList_Elements == nullptr || nodeList_Elements->GetNodeCount() == 0 )
	{
		m_messages->AddMessage( UXmlProcMessageCode::Error, "No node named <Poster:Render:Element> in the xml." );
		return CResult();
	}
	ReadList( (*ppOutElementRoot), nodeList_Elements );

	return CResult();
}

void iberbar::Poster::CXmlReader::ReadNode( iberbar::Poster::CElementBase* elementParent, iberbar::Xml::PTR_CNodeA node )
{
	// read
	const char* elementType = node->GetAttribute( "type" );
	if ( elementType == nullptr )
		elementType = "empty";

	PCallbackXmlReadElementProc proc = m_readProcFactory->GetReadElementProc( elementType );
	if ( proc == nullptr )
	{
		m_messages->AddMessage( UXmlProcMessageCode::Warning, "No reading process for type<%s>", elementType );
	}
	else
	{
		PTR_CElementBase element;
		UXmlReadProcContext context;
		context.pResources = m_resourcesManager;
		context.ppOutElement = &element;
		context.pMessagesBuffer = nullptr;
		context.node = node;
		CResult retProc = proc( context );
		if ( retProc.IsOK() )
		{
			if ( element != nullptr )
				elementParent->AddSubElement( element );
		}
		else
		{
			m_messages->AddMessage( UXmlProcMessageCode::Error, "Some errors happened when executed reading process: %s", retProc.data );
		}
	}

	// readlist
	PTR_CNodeListA nodeList_Elements;
	node->SelectNodes( "Element", &nodeList_Elements );
	ReadList( elementParent, nodeList_Elements );
}

void iberbar::Poster::CXmlReader::ReadList( iberbar::Poster::CElementBase* elementParent, iberbar::Xml::PTR_CNodeListA nodelist )
{
	if ( nodelist == nullptr )
		return;

	int nodeCount = nodelist->GetNodeCount();
	if ( nodeCount == 0 )
		return;

	PTR_CNodeA node;
	for ( int i = 0; i < nodeCount; i++ )
	{
		nodelist->GetNodeAt( i, &node );
		ReadNode( elementParent, node );
	}
}

