#pragma once

#include <iberbar/Gui/Headers.h>
#include <iberbar/Utility/Rect.h>
#include <iberbar/Utility/Color.h>
#include <iberbar/Utility/Xml/Base.h>

namespace iberbar
{
	struct UFontDesc;

	namespace Gui
	{
		namespace XmlProcHelper
		{
			bool XmlAttributeConvertToAlignHorizental( const char* strValue, UAlignHorizental* pAlign );
			bool XmlAttributeConvertToAlignVertical( const char* strValue, UAlignVertical* pAlign );

			__iberbarGuiApi__ bool XmlNodeReadFontDesc( Xml::CNodeA* pXmlNode, UFontDesc* pFontDesc );
		}
	}
}






inline bool iberbar::Gui::XmlProcHelper::XmlAttributeConvertToAlignHorizental( const char* strValue, UAlignHorizental* pAlign )
{
	if ( strcmp( strValue, "Left" ) == 0 )
	{
		*pAlign = UAlignHorizental::Left;
		return true;
	}
	else if ( strcmp( strValue, "Center" ) == 0 )
	{
		*pAlign = UAlignHorizental::Center;
		return true;
	}
	else if ( strcmp( strValue, "Right" ) == 0 )
	{
		*pAlign = UAlignHorizental::Right;
		return true;
	}

	return false;
}


inline bool iberbar::Gui::XmlProcHelper::XmlAttributeConvertToAlignVertical( const char* strValue, UAlignVertical* pAlign )
{
	if ( strcmp( strValue, "Top" ) == 0 )
	{
		*pAlign = UAlignVertical::Top;
		return true;
	}
	else if ( strcmp( strValue, "Center" ) == 0 )
	{
		*pAlign = UAlignVertical::Center;
		return true;
	}
	else if ( strcmp( strValue, "Bottom" ) == 0 )
	{
		*pAlign = UAlignVertical::Bottom;
		return true;
	}

	return false;
}