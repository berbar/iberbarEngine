

#include <iberbar/Gui/Xml/XmlState.h>
#include <iberbar/Gui/Widget.h>
#include <iberbar/Gui/RenderElement.h>
#include <iberbar/Gui/Dialog.h>
#include <iberbar/Utility/Xml/RapidXml.h>
#include <iberbar/Utility/Log/OutputDevice.h>

#include <stack>



namespace iberbar
{
	namespace Gui
	{
		template < typename TFunc >
		struct TXmlStateProcNode
		{
			std::string strType;
			std::function<TFunc> func;
		};





		class CXmlState
		{
		public:
			CXmlState();

		public:
			void SetDialog( CDialog* pDialog ) { m_pDialog = pDialog; }
			void BuildContext();
			//void SetWidget( CWidget* pWidget ) { m_pWidget = pWidget; }
			void ReadWidget( Xml::CNodeA* pXmlNode, CWidget* pWidget );
			void ReadElement( Xml::CNodeA* pXmlNode, CRenderElement* pElement, CRenderElement* pElementParent );
			void ReadWidgetList( Xml::CNodeListA* pXmlNodeList );
			void ReadElementList( Xml::CNodeListA* pXmlNodeList, CRenderElement* pElement );
			//void ReadStyles( Xml::CNodeListA* pXmlNodeList, CRenderElement* pRenderElement );
			void PushWidget( CWidget* pWidget ) { m_WidgetStack.push( pWidget ); }
			void PopWidget() { m_WidgetStack.pop(); }

		private:
			PTR_CWidget CreateWidget( const char* strType );

		public:
			Logging::CLogger* m_pLogger;
			std::vector<TXmlStateProcNode<UCallbackReadProc_Widget>> m_ReadProcList_Widget;
			std::vector<TXmlStateProcNode<UCallbackReadProc_Element>> m_ReadProcList_Element;
			std::vector<TXmlStateProcNode<UCallbackCreateProc_Widget>> m_CreateProcList_Widget;
			std::vector<TXmlStateProcNode<UCallbackCreateProc_Element>> m_CreateProcList_Element;
			std::function<UCallbackXmlGetTexture> m_GetTexture;
			std::function<UCallbackXmlGetFont> m_GetFont;
			std::stack<PTR_CWidget> m_WidgetStack;
			UXmlParserContext m_Context;

		private:
			CDialog* m_pDialog;


		public:
			template < typename TFunc >
			static inline TXmlStateProcNode<TFunc>* FindFunc( std::vector<TXmlStateProcNode<TFunc>>& v, const char* str )
			{
				for ( size_t i = 0, s = v.size(); i < s; i++ )
				{
					if ( strcmp( str, v[i].strType.c_str() ) == 0 )
					{
						return &v[ i ];
					}
				}
				return nullptr;
			}
		};
	}
}



iberbar::Gui::CXmlParser::CXmlParser()
	: m_pState( new CXmlState() )
	, m_Logger( nullptr )
{
}


iberbar::Gui::CXmlParser::~CXmlParser()
{
	SAFE_DELETE( m_pState );
}


iberbar::CResult iberbar::Gui::CXmlParser::ReadFile( const char* strFile, CDialog* pDialog )
{
	if ( StringIsNullOrEmpty( strFile ) == true )
		return MakeResult( ResultCode::Bad, "Invalid File" );

	Xml::PTR_CDocumentA pDoc = Xml::CreateRapidXmlDocumentA();

	CResult ret = pDoc->LoadFromFile( strFile );
	if ( ret.IsOK() == false )
		return ret;

	Xml::PTR_CNodeA pNode_Root;
	pDoc->GetRoot( &pNode_Root );

	if ( strcmp( pNode_Root->NameText(), "iberbarGui" ) != 0 )
		return MakeResult( ResultCode::Bad, "No root named <iberbarGui>" );

	Xml::PTR_CNodeListA pNodeList_Widgets;
	pNode_Root->SelectNodes( "Widget", &pNodeList_Widgets );
	if ( pNodeList_Widgets != nullptr )
	{
		m_pState->PushWidget( pDialog->GetWidgetRoot() );
		m_pState->BuildContext();
		m_pState->ReadWidgetList( pNodeList_Widgets );
		m_pState->PopWidget();
	}

	return CResult();
}
//
//
//void iberbar::Gui::CXmlParser::PushWidget( CWidget* pWidget )
//{
//	m_pState->m_WidgetStack.push_back( pWidget );
//}
//
//
//void iberbar::Gui::CXmlParser::PopWidget()
//{
//	if ( m_pState->m_WidgetStack.empty() == true )
//		return;
//
//	m_pState->m_WidgetStack.pop_back();
//}
//
//
//void iberbar::Gui::CXmlParser::ClearWidgetStack()
//{
//	m_pState->m_WidgetStack.clear();
//}


void iberbar::Gui::CXmlParser::SetLogOutputDevice( Logging::COutputDevice* pLogOutputDevice )
{
	m_Logger = Logging::CLogger( pLogOutputDevice );

	m_pState->m_pLogger = &m_Logger;
}


void iberbar::Gui::CXmlParser::RegisterReadProc_Widget( const char* strType, std::function<UCallbackReadProc_Widget> func )
{
	TXmlStateProcNode<UCallbackReadProc_Widget> node { strType, func };
	m_pState->m_ReadProcList_Widget.push_back( node );
}


void iberbar::Gui::CXmlParser::RegisterReadProc_Element( const char* strType, std::function<UCallbackReadProc_Element> func )
{
	TXmlStateProcNode<UCallbackReadProc_Element> node{ strType, func };
	m_pState->m_ReadProcList_Element.push_back( node );
}


void iberbar::Gui::CXmlParser::RegisterCreateProc_Widget( const char* strType, std::function<UCallbackCreateProc_Widget> func )
{
	TXmlStateProcNode<UCallbackCreateProc_Widget> node{ strType, func };
	m_pState->m_CreateProcList_Widget.push_back( node );
}


void iberbar::Gui::CXmlParser::RegisterCreateProc_Element( const char* strType, std::function<UCallbackCreateProc_Element> func )
{
	TXmlStateProcNode<UCallbackCreateProc_Element> node{ strType, func };
	m_pState->m_CreateProcList_Element.push_back( node );
}


void iberbar::Gui::CXmlParser::RegisterGetTexture( std::function<UCallbackXmlGetTexture> GetTexture )
{
	m_pState->m_GetTexture = GetTexture;
}


void iberbar::Gui::CXmlParser::RegisterGetFont( std::function<UCallbackXmlGetFont> GetFont )
{
	m_pState->m_GetFont = GetFont;
}










iberbar::Gui::CXmlState::CXmlState()
	: m_pLogger( nullptr )
	, m_ReadProcList_Widget()
	, m_ReadProcList_Element()
	, m_CreateProcList_Widget()
	, m_CreateProcList_Element()
	, m_GetTexture()
	, m_GetFont()
	, m_WidgetStack()
	, m_Context()
	, m_pDialog( nullptr )
{

}


void iberbar::Gui::CXmlState::BuildContext()
{
	m_Context.pLogger = m_pLogger;
	m_Context.GetTexture = m_GetTexture;
	m_Context.GetFont = m_GetFont;
}


void iberbar::Gui::CXmlState::ReadWidget( Xml::CNodeA* pXmlNode, CWidget* pWidget )
{
	PTR_CRenderElement pElement = PTR_CRenderElement::_sNew();
	pElement->SetPercentW( true );
	pElement->SetPercentH( true );
	pElement->SetSize( CSize2i( 100, 100 ) );
	pWidget->SetRenderElement( pElement );

	Xml::PTR_CNodeA pNode_Styles;
	pXmlNode->SelectSingleNode( "Styles", &pNode_Styles );
	if ( pNode_Styles != nullptr )
	{
		Xml::PTR_CNodeListA pNodeList_Elements;
		pNode_Styles->SelectNodes( "Element", &pNodeList_Elements );
		if ( pNodeList_Elements != nullptr && pNodeList_Elements->GetNodeCount() > 0 )
		{
			ReadElementList( pNodeList_Elements, pElement );
		}
	}

	const char* strType = pXmlNode->GetAttribute( "Type" );
	if ( StringIsNullOrEmpty( strType ) == true )
		strType = "Default";

	auto pReadProcNode = FindFunc( m_ReadProcList_Widget, strType );
	if ( pReadProcNode != nullptr )
		pReadProcNode->func( &m_Context, pXmlNode, pWidget, pElement );

	Xml::PTR_CNodeListA pNodeList_ChildWidgets;
	pXmlNode->SelectNodes( "Widget", &pNodeList_ChildWidgets );
	if ( pNodeList_ChildWidgets != nullptr )
	{
		PushWidget( pWidget );
		ReadWidgetList( pNodeList_ChildWidgets );
		PopWidget();
	}

}


void iberbar::Gui::CXmlState::ReadElement( Xml::CNodeA* pXmlNode, CRenderElement* pElement, CRenderElement* pElementParent )
{
	if ( pElementParent != nullptr )
	{
		pElementParent->AddChildElement( pElement );
	}
	pElement->Init();

	const char* strType = pXmlNode->GetAttribute( "Type" );
	if ( StringIsNullOrEmpty( strType ) == true )
		strType = "Default";

	auto pReadProcNode = FindFunc( m_ReadProcList_Element, strType );
	if ( pReadProcNode != nullptr )
		pReadProcNode->func( &m_Context, pXmlNode, pElement );

	Xml::PTR_CNodeListA pNodeList_Elements;
	pXmlNode->SelectNodes( "Element", &pNodeList_Elements );
	if ( pNodeList_Elements != nullptr && pNodeList_Elements->GetNodeCount() > 0 )
	{
		ReadElementList( pNodeList_Elements, pElement );
	}
}


void iberbar::Gui::CXmlState::ReadWidgetList( Xml::CNodeListA* pXmlNodeList )
{
	Xml::PTR_CNodeA pNode;
	const char* strType = nullptr;
	PTR_CWidget pWidget;
	for ( int i = 0, s = pXmlNodeList->GetNodeCount(); i < s; i++ )
	{
		pWidget = nullptr;

		pXmlNodeList->GetNodeAt( i, &pNode );
		if ( pNode == nullptr )
			continue;

		strType = pNode->GetAttribute( "Type" );
		if ( StringIsNullOrEmpty( strType ) == true )
			strType = "Default";

		auto pCreateProcNode = FindFunc( m_CreateProcList_Widget, strType );
		if ( pCreateProcNode != nullptr )
			pWidget.attach( pCreateProcNode->func( strType ) );

		if ( pWidget == nullptr )
			continue;

		m_WidgetStack.top()->AddWidget( pWidget );

		ReadWidget( pNode, pWidget );
	}
}


void iberbar::Gui::CXmlState::ReadElementList( Xml::CNodeListA* pXmlNodeList, CRenderElement* pElementParent )
{
	Xml::PTR_CNodeA pNode;
	const char* strType = nullptr;
	PTR_CRenderElement pElement;
	for ( int i = 0, s = pXmlNodeList->GetNodeCount(); i < s; i++ )
	{
		pElement = nullptr;

		pXmlNodeList->GetNodeAt( i, &pNode );
		if ( pNode == nullptr )
			continue;

		strType = pNode->GetAttribute( "Type" );
		if ( StringIsNullOrEmpty( strType ) == true )
			strType = "Default";

		auto pCreateProcNode = FindFunc( m_CreateProcList_Element, strType );
		if ( pCreateProcNode != nullptr )
			pElement.attach( pCreateProcNode->func( strType ) );

		if ( pElement == nullptr )
			continue;

		ReadElement( pNode, pElement, pElementParent );
	}
}


iberbar::Gui::PTR_CWidget iberbar::Gui::CXmlState::CreateWidget( const char* strType )
{
	PTR_CWidget pWidget;
	if ( StringIsNullOrEmpty( strType ) )
	{
		pWidget.attach( new CWidget() );
	}
	else
	{
		auto pNode = FindFunc( m_CreateProcList_Widget, strType );
		if ( pNode )
		{
			pWidget.attach( pNode->func( strType ) );
		}
	}

	return pWidget;
}
