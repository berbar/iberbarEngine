
#include <iberbar/Gui/Xml/XmlProcHelper.h>
#include <iberbar/Font/Types.h>
#include <iberbar/Utility/Xml/Base.h>
#include <iberbar/Utility/String.h>



bool iberbar::Gui::XmlProcHelper::XmlNodeReadFontDesc( Xml::CNodeA* pXmlNode, UFontDesc* pFontDesc )
{
	const char* strFontFamily = pXmlNode->GetAttribute( "Family" );
	if ( StringIsNullOrEmpty( strFontFamily ) == false )
	{
		pFontDesc->FamilyName = strFontFamily;
		pFontDesc->Size = Xml::GetAttributeUInt32( pXmlNode, "Size", 20 );
		const char* strWeight = pXmlNode->GetAttribute( "Weight" );
		if ( strWeight != nullptr )
		{
			if ( strcmp( strWeight, "Bold" ) == 0 )
				pFontDesc->Weight = 1000;
			else if ( strcmp( strWeight, "Normal" ) == 0 )
				pFontDesc->Weight = 500;
		}

		return true;
	}

	return false;
}