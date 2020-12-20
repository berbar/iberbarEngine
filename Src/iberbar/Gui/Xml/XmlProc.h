#pragma once

#include <iberbar/Gui/Widget.h>
#include <iberbar/Gui/RenderElement.h>
#include <iberbar/Utility/Xml/Base.h>
#include <iberbar/Utility/Color.h>


namespace iberbar
{
	namespace Gui
	{
		struct UXmlParserContext;

		__iberbarGuiApi__ void XmlReadProc_Object( Xml::CNodeA* pXmlNode, CObject* pObject );

		__iberbarGuiApi__ void XmlReadProc_Widget( const UXmlParserContext* pContext, Xml::CNodeA* pXmlNode, CWidget* pWidget, CRenderElement* pRenderElement );
		__iberbarGuiApi__ void XmlReadProc_Widget_Button( const UXmlParserContext* pContext, Xml::CNodeA* pXmlNode, CWidget* pWidget, CRenderElement* pRenderElement );
		__iberbarGuiApi__ void XmlReadProc_Widget_CheckBox( const UXmlParserContext* pContext, Xml::CNodeA* pXmlNode, CWidget* pWidget, CRenderElement* pRenderElement );
		__iberbarGuiApi__ void XmlReadProc_Widget_RadioBox( const UXmlParserContext* pContext, Xml::CNodeA* pXmlNode, CWidget* pWidget, CRenderElement* pRenderElement );
		__iberbarGuiApi__ void XmlReadProc_Widget_ListBox( const UXmlParserContext* pContext, Xml::CNodeA* pXmlNode, CWidget* pWidget, CRenderElement* pRenderElement );
		__iberbarGuiApi__ void XmlReadProc_Widget_EditBox( const UXmlParserContext* pContext, Xml::CNodeA* pXmlNode, CWidget* pWidget, CRenderElement* pRenderElement );

		__iberbarGuiApi__ CWidget* XmlCreateProc_Widget( const char* strType );
		__iberbarGuiApi__ CWidget* XmlCreateProc_Widget_Button( const char* strType );
		__iberbarGuiApi__ CWidget* XmlCreateProc_Widget_CheckBox( const char* strType );
		__iberbarGuiApi__ CWidget* XmlCreateProc_Widget_RadioBox( const char* strType );
		__iberbarGuiApi__ CWidget* XmlCreateProc_Widget_ListBox( const char* strType );
		__iberbarGuiApi__ CWidget* XmlCreateProc_Widget_EditBox( const char* strType );

		__iberbarGuiApi__ void XmlReadProc_Element( const UXmlParserContext* pContext, Xml::CNodeA* pXmlNode, CRenderElement* pElement );
		__iberbarGuiApi__ void XmlReadProc_Element_ColorRect( const UXmlParserContext* pContext, Xml::CNodeA* pXmlNode, CRenderElement* pElement );
		__iberbarGuiApi__ void XmlReadProc_Element_StateTexture( const UXmlParserContext* pContext, Xml::CNodeA* pXmlNode, CRenderElement* pElement );
		__iberbarGuiApi__ void XmlReadProc_Element_StateLabel( const UXmlParserContext* pContext, Xml::CNodeA* pXmlNode, CRenderElement* pElement );
		__iberbarGuiApi__ void XmlReadProc_Element_EditBoxText( const UXmlParserContext* pContext, Xml::CNodeA* pXmlNode, CRenderElement* pElement );

		__iberbarGuiApi__ CRenderElement* XmlCreateProc_Element( const char* strType );
		__iberbarGuiApi__ CRenderElement* XmlCreateProc_Element_ColorRect( const char* strType );
		__iberbarGuiApi__ CRenderElement* XmlCreateProc_Element_StateTexture( const char* strType );
		__iberbarGuiApi__ CRenderElement* XmlCreateProc_Element_StateLabel( const char* strType );
		__iberbarGuiApi__ CRenderElement* XmlCreateProc_Element_EditBoxText( const char* strType );


		__iberbarGuiApi__ int XmlAttributeConvertToWidgetState( const char* strStateName );
		__iberbarGuiApi__ CColor4B XmlAttributeConvertToColor4B( const char* strColor );
		__iberbarGuiApi__ CRect2f XmlAttributeConvertToUV( const char* strUV );
	}
}



