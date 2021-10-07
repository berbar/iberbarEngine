
#pragma once

#include <iberbar/Gui/Headers.h>
#include <iberbar/Utility/Stack.h>
#include <iberbar/Utility/Result.h>
#include <iberbar/Utility/Xml/Base.h>
#include <iberbar/Utility/Log/Logger.h>
#include <iberbar/Font/Types.h>
#include <functional>




#ifdef _UNICODE
#define ELYQ_GUIXML_MAKE_NODE_NAME(name)       L(name)
#else
#define ELYQ_GUIXML_MAKE_NODE_NAME(name)       name
#endif

#define ELYQ_GUIXML_UTAG_WIDGET    ELYQ_GUIXML_MAKE_NODE_NAME("Widget")
#define ELYQ_GUIXML_UATR_X         ELYQ_GUIXML_MAKE_NODE_NAME("x")



#define ELYQ_GUI_XML_TAG_ROOT      "iberbarGui"
#define ELYQ_GUI_XML_ATR_VERSION   "version"
#define ELYQ_GUI_XML_TAG_WIDGET    "Widget"





namespace iberbar
{
	namespace RHI
	{
		class ITexture;
	}

	namespace Renderer
	{
		class CFont;
	}

	namespace Gui
	{
		class CWidget;
		class CRenderElement;
		class CDialog;

		class CXmlParser;
		class CXmlState;
		struct UXmlParserContext;


		typedef void UCallbackReadProc_Widget( const UXmlParserContext* pContext, Xml::CNodeA* pXmlNode, CWidget* pWidget, CRenderElement* pElement );
		typedef void UCallbackReadProc_Element( const UXmlParserContext* pContext, Xml::CNodeA* pXmlNode, CRenderElement* pRenderElement );
		typedef CWidget* UCallbackCreateProc_Widget( const char* strType );
		typedef CRenderElement* UCallbackCreateProc_Element( const char* strType );
		typedef CResult UCallbackXmlGetTexture( const char* strText, RHI::ITexture** ppOutTexture );
		typedef CResult UCallbackXmlGetFont( Renderer::CFont** ppOutFont, const UFontDesc& FontDesc );


		struct UXmlParserContext
		{
			Logging::CLogger* pLogger;
			std::function<UCallbackXmlGetTexture> GetTexture;
			std::function<UCallbackXmlGetFont> GetFont;
		};



		class __iberbarGuiApi__ CXmlParser
		{
		public:
			CXmlParser();
			~CXmlParser();

		public:
			/*
			 * 读写之前需要压入需要配置的父级控件对象；在读写完成后，若不再需要的时候弹出
			 */
			CResult ReadFile( const char* strFileName, CDialog* pDialog );

		public:
			void SetLogOutputDevice( Logging::COutputDevice* pLogOutputDevice );
			void RegisterReadProc_Widget( const char* strType, std::function<UCallbackReadProc_Widget> func );
			void RegisterReadProc_Element( const char* strType, std::function<UCallbackReadProc_Element> func);
			void RegisterCreateProc_Widget( const char* strType, std::function<UCallbackCreateProc_Widget> func );
			void RegisterCreateProc_Element( const char* strType, std::function<UCallbackCreateProc_Element> func );
			void RegisterGetTexture( std::function<UCallbackXmlGetTexture> func );
			void RegisterGetFont( std::function<UCallbackXmlGetFont> func );

		private:
			CXmlState* m_pState;
			Logging::CLogger m_Logger;
		};
	}


}
